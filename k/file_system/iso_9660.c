//
// Created by chalu on 5/10/2023.
//

#include <stdio.h>
#include "../include/k/iso9660.h"

static file_descriptor file_table;
static struct iso_dir dir_root;
struct iso_path_table_le path_table;
struct iso_prim_voldesc *pvd;

char *strchr(const char *p, int ch)
{
    char c;

    c = ch;
    for (;; ++p) {
        if (*p == c)
            return ((char *)p);
        if (*p == '\0')
            return (NULL);
    }
}

u8 init_iso_9660() {

    // Init the fd table
    memset(file_table, 0, sizeof (file_table));
    printf("test file system\n");
    // read primary volume descriptor
    pvd = read_block(ISO_PRIM_VOLDESC_BLOCK);
    printf("test file system after pvd\n");

    if (memcmp(pvd->std_identifier, "CD001", 5) != 0)
        return -1;

    // Copy root directory
    // memcpy(&dir_root, &pvd->root_dir, sizeof(struct iso_dir));
    dir_root = pvd->root_dir;
    return 0;

}


int find_file(u32 current_size, u32 current_lba, const char *path) {
    int fd;

    // Search for the file in the final directory
    for (u32 offset = 0; offset < current_size; offset += ISO_BLOCK_SIZE) {
        // Read the directory entry
        struct iso_dir *entry;
        entry = read_block(current_lba + (offset / ISO_BLOCK_SIZE));

        // Compare the identifier with the file name
        if (strcmp(entry->idf, path) == 0) {
            // Found the file, assign the file descriptor and return it
            for (fd = 0; fd < MAX_FILE_DESCRIPTOR; fd++) {
                if (file_table[fd].fd == 0) {
                    file_table[fd].fd = 1;
                    file_table[fd].offset = 0;
                    file_table[fd].lba = entry->data_blk.le;
                    file_table[fd].file_size = entry->file_size.le;
                    break;
                }
            }

            // File descriptor assigned
            if (fd == MAX_FILE_DESCRIPTOR) {
                return -1; // No available file descriptor
            }

            return fd;
        }
    }

    // File not found
    return -1;
}


int find_dir(const char *pathname) {
    // Init for search from root directory
    u32 current_lba = dir_root.data_blk.le;
    u32 current_size = dir_root.file_size.le;
    u32 next_lba;
    u32 next_size;
    u32 offset;

    const char *path = pathname;
    struct iso_dir *entry;

    while (*path != '\0') {
        // Find the next path component
        const char *next_slash = strchr(path, '/');
        size_t component_len = (next_slash != NULL) ? (size_t)(next_slash - path) : strlen(path);

        // Search for the current component in the current directory
        next_lba = 0;
        next_size = 0;

        for (offset = 0; offset < current_size; offset += ISO_BLOCK_SIZE) {
            // Read the directory entry
            entry = read_block(current_lba + (offset / ISO_BLOCK_SIZE));

            // Compare the identifier with the current path component
            if (strncmp(entry->idf, path, component_len) == 0) {
                // Match found, update the next LBA and size
                next_lba = entry->data_blk.le;
                next_size = entry->file_size.le;
                break;
            }
        }

        // Check if the current component was found
        if (next_lba == 0 || next_size == 0) {
            return -1; // Component not found
        }

        // Move to the next component
        current_lba = next_lba;
        current_size = next_size;

        // Move to the next path component
        path = (next_slash != NULL) ? (next_slash + 1) : next_slash;
    }

    return find_file(current_size, current_lba, path);

}



int open(const char *pathname, int flags) {
    printf("pathname %s\n", pathname);

    // Only readable
    if (flags != O_RDONLY)
        return -1;

    // Find the file
    return find_dir(pathname);
}

ssize_t read(int fd, void *buf, size_t count) {
    // fd too high or fd not use
    if (fd > MAX_FILE_DESCRIPTOR || file_table[fd].fd == 0)
        return -1;

    u32 remaining_size, bytes_to_read, block_offset, blocks_to_read;

    remaining_size = file_table[fd].file_size - file_table[fd].offset;
    bytes_to_read = (remaining_size < count) ? remaining_size : count;

    if (bytes_to_read == 0) {
        // No more data to read
        return 0;
    }

    // Read the data from the ISO9660 file system
    block_offset = file_table[fd].offset % ISO_BLOCK_SIZE;
    blocks_to_read = (bytes_to_read + block_offset - 1) / ISO_BLOCK_SIZE;

    u8 *data_ptr = (u8 *)buf;

    for (u32 i = 0; i <= blocks_to_read; i++) {
        u32 block_index = (file_table[fd].lba + (file_table[fd].offset / ISO_BLOCK_SIZE)) + i;
        u8 *block_data = read_block(block_index);

        if (block_data == NULL) {
            // Error reading block
            return -1;
        }

        u32 bytes_copied = 0;

        if (i == 0 && block_offset > 0) {
            u32 bytes_from_offset = ISO_BLOCK_SIZE - block_offset;
            bytes_copied = (bytes_from_offset < bytes_to_read) ? bytes_from_offset : bytes_to_read;
            memcpy(data_ptr, block_data + block_offset, bytes_copied);
        } else {
            bytes_copied = (bytes_to_read - bytes_copied < ISO_BLOCK_SIZE) ? bytes_to_read - bytes_copied : ISO_BLOCK_SIZE;
            memcpy(data_ptr, block_data, bytes_copied);
        }

        data_ptr += bytes_copied;
        file_table[fd].offset += bytes_copied;
    }

    return bytes_to_read;
}

off_t seek(int fd, off_t offset, int whence) {
    // fd too high or fd not use
    if (fd > MAX_FILE_DESCRIPTOR || file_table[fd].fd == 0)
        return -1;

    off_t new_offset = 0;
    switch (whence) {
        case SEEK_SET:
            new_offset = offset;
            break;
        case SEEK_CUR:
            new_offset = file_table[fd].offset + offset;
            break;
        case SEEK_END:
            new_offset = file_table[fd].file_size + offset;
            break;
        default:
            return -1; // Invalid whence parameter

    }

    // check bounce and read block
    if (new_offset < 0 || new_offset > file_table[fd].file_size) {
        // Invalid offset
        return -1;
    }
    file_table[fd].offset = new_offset;

    // Update the LBA if necessary
    file_table[fd].lba = file_table[fd].lba + (file_table[fd].offset / ISO_BLOCK_SIZE);

    return new_offset;
}

int close(int fd) {
    // fd too high or fd not use
    if (fd > MAX_FILE_DESCRIPTOR || file_table[fd].fd == 0)
        return -1;

    memset(&file_table[fd], 0, sizeof(file_table[fd]));
    return 0;
}

