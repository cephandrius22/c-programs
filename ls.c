#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

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
        } else {
            printf("%-10lld", statbuf.st_size);
        }

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
            printf("%-10s", grp->gr_name);
        }

        printf("%-20s\n", dirent->d_name);
    }
}