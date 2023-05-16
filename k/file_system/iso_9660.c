//
// Created by chalu on 5/10/2023.
//

#include "../include/k/iso9660.h"

static file_descriptor fds;
static struct iso_dir dir_root;
struct iso_path_table_le path_table;

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
    struct iso_prim_voldesc *pvd;

    memset(fds, 0, sizeof (fds));

    // read primary volume descriptor
    pvd = read_block(ISO_PRIM_VOLDESC_BLOCK);
    if (memcmp(pvd->std_identifier, "CD001", 5) != 0)
        return -1;

    dir_root = pvd->root_dir;
    //path_table.data_blk;
    return 0;

}

struct iso_dir *find_file(const char *pathname) {
    char *dir;
    //struct iso_dir start_dir;

    while ((dir = strchr(pathname, '/'))) {
        // Just file name
        if (dir == pathname)
            break;

    }
    return 0;

}

int open(const char *pathname, int flags) {
    int fd;
    struct iso_dir *dir;


    // Only readable
    if (flags != O_RDONLY)
        return -1;

    // Find the file
    dir = find_file(pathname);

    // Find free fd
    for (u16 i = 0; i < MAX_FILE_DESCRIPTOR; i++) {
        if (fds[i].fd == 0) {
            fd = i;
            fds[i].fd = 1;
        }
    }
    (void) fd;
    (void) dir;
    return 0;



}
ssize_t read(int fd, void *buf, size_t count) {
    if (fd > MAX_FILE_DESCRIPTOR)
        return -1;
    
    u32 have_to_read, left_in_the_sector;
    s32 total_read;

    while (count > 0)
    {
        have_to_read = count > (fds[fd].file_size - fds[fd].offset) ?
                 fds[fd].file_size - fds[fd].offset : count;
        if (have_to_read == 0)
            break;

        // How many bytes we have left in this sector
        left_in_the_sector = ISO_BLOCK_SIZE - (have_to_read % ISO_BLOCK_SIZE);

        // To modify
        if (left_in_the_sector == ISO_BLOCK_SIZE) {
            buf = read_block(count);
        }
        else {
            buf = read_block(count);
        }

        count -= have_to_read;
        fds[fd].offset += have_to_read;
        buf += have_to_read;
        total_read += have_to_read;
    }
    return total_read;
}

off_t seek(int fd, off_t offset, int whence) {
    if (fd > MAX_FILE_DESCRIPTOR)
        return -1;

    switch (whence) {
        case SEEK_SET:
            fds[fd].offset = offset;
            break;
        case SEEK_CUR:
            fds[fd].offset += offset;
            break;
        case SEEK_END:
            fds[fd].offset = fds[fd].file_size + offset;
            break;
        default:
            return -1;

    }

    // check bounce and read block ?


    return fds[fd].offset;
}

int close(int fd) {
    if (fd < MAX_FILE_DESCRIPTOR) {
        fds[fd].fd = 0;
        return 0;
    }
    return -1;
}

