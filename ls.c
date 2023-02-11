#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>


/*
 * Write the given file mode and permissions to a string buffer.
 *
 * Example: "drwxr-xr-x"
 * 
 * @param[in] st_mode Bitmask containing a file's mode and permissions.
 * @param[in] buf Buffer to be written to.
 * @param[in] buf_size Size of the buffer.
 * 
 * @return 0 upon success, 1 otherwise.
 */
static int fmt_perm_str(mode_t st_mode, char *buf, size_t buf_size) {
    // It's possible we should be returning the number of bytes written.
    if (buf_size < 11) {
        return 1;
    }

    // This just _feels_ a little off. There is probably a better way to
    // do this.
    buf[0] = S_ISDIR(st_mode) ? 'd' : '-';
    buf[1] = S_IRUSR & st_mode ? 'r' : '-';
    buf[2] = S_IWUSR & st_mode ? 'w' : '-';
    buf[3] = S_IXUSR & st_mode ? 'x' : '-';
    buf[4] = S_IRGRP & st_mode ? 'r' : '-';
    buf[5] = S_IWGRP & st_mode ? 'w' : '-';
    buf[6] = S_IXGRP & st_mode ? 'x' : '-';
    buf[7] = S_IROTH & st_mode ? 'r' : '-';
    buf[8] = S_IWOTH & st_mode ? 'w' : '-';
    buf[9] = S_IXOTH & st_mode ? 'x' : '-';
    buf[10] = '\0';

    return 0;
}

int main(int argc, char *argv[]) {
    // TODO: check arguments and flags
    DIR *dir;
    struct dirent *dirent;
    struct stat statbuf;
    struct passwd *pwd;
    struct group *grp;
    int rc;

    dir = opendir(".");
    if (dir == NULL) {
        perror("Failed to open directory");
        exit(1);
    }

    while ((dirent = readdir(dir)) != NULL) {
        rc = stat(dirent->d_name, &statbuf);
        if (rc != 0) {
            // TODO: print an error
            continue;
        }

        char perm_buf[11];
        rc = fmt_perm_str(statbuf.st_mode, perm_buf, sizeof(perm_buf));
        if (rc != 0) {
            continue;
        }
        printf("%-13s", perm_buf);

        pwd = getpwuid(statbuf.st_uid);
        if (pwd == NULL) {
            // TODO:
        } else {
            printf("%-15s", pwd->pw_name);
        }

        grp = getgrgid(statbuf.st_gid);
        if (grp == NULL) {
            // TODO:
        } else {
            printf("%-8s", grp->gr_name);
        }

        // I'm not sure about this method of left and right padding.
        printf("%8lld%2s", statbuf.st_size, " ");

        printf("%-20s\n", dirent->d_name);
    }
}