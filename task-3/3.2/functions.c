#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "functions.h"

#define BUFFER_SIZE 4096

int mk_dirf(char* path_to_dir) {
    if (mkdir(path_to_dir, 0777) == -1) {
        perror("Create directory error");
    }
    return 0;
}

int ls_dirf(char* path_to_dir) {
    DIR* dir = opendir(path_to_dir);
    if (dir == NULL) {
        perror("Open directory error");
    }
    struct dirent* cur_file = readdir(dir);

    while (cur_file != NULL) {
        printf("%s\n", cur_file->d_name);
        cur_file = readdir(dir);
    }
    return 0;
}

int rm_dirf(char* path_to_dir) {
    if (rmdir(path_to_dir) == -1) {
        perror("Remove directory error");
    }
    return 0;
}

int mk_filef(char* path_to_file) {
    int file = open(path_to_file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (file == -1) {
        perror("Create file error");
    }
    close(file);
    return 0;
}

int cat_filef(char* path_to_file) {
    int file_from = open(path_to_file, O_RDONLY);
    if (file_from == -1) {
        perror("Open file error");
    }
    int file_size = lseek(file_from, 0, SEEK_END);
    char buffer[BUFFER_SIZE] = {0};
    int ptr = 0;

    while (ptr != file_size) {
        lseek(file_from, ptr, SEEK_SET);

        int n = read(file_from, buffer, BUFFER_SIZE);

        write(1, buffer, n);

        ptr = ptr + n < file_size ? ptr + n : file_size;
    }
    close(file_from);
    return 0;

}

int rm_filef(char* path_to_file) {
    if (unlink(path_to_file) == -1) {
        perror("Remove file error");
    }
    return 0;
}


int ln_symlinkf(char* path_to_file, char* path_to_link) {
    if (symlink(path_to_file, path_to_link) == -1) {
        perror("Create symlink error");
    }
    return 0;
}

int cat_symlinkf(char* path_to_link) {
    char buffer[BUFFER_SIZE] = {0};
    if (readlink(path_to_link, buffer, BUFFER_SIZE) == -1) {
        perror("Read symlink error");
    }
    printf("%s\n", buffer);
    return 0;
}

int cat_file_symlinkf(char* path_to_link) {
    return cat_filef(path_to_link);
}

int rm_symlinkf(char* path_to_link) {
    if (unlink(path_to_link) == -1) {
        perror("Remove symlink error");
    }
    return 0;
}

int ln_hardlinkf(char* path_to_file, char* path_to_link) {
    if (link(path_to_file, path_to_link) == -1) {
        perror("Create hardlink error");
    }
    return 0;
}

int rm_hardlinkf(char* path_to_link) {
    if (unlink(path_to_link) == -1) {
        perror("Remove hardlink error");
    }
    return 0;
}

int stat_filef(char* path_to_file) {
    struct stat statf;
    if (stat(path_to_file, &statf) == -1) {
        perror("Stat error");
    }
    printf("File access rights: %o\n", statf.st_mode & 0777);
    printf("Count hard links: %ld\n", statf.st_nlink);
    return 0;
}

int ch_modf(char* path_to_file, char* mode) {
    int permissions = 0;
    for (int i = 0; i < 9; i++) {
        if (mode[i] == 'r') {
            if (i/3 == 0) {
                permissions |= S_IRUSR;
            }
            else if (i/3 == 1) {
                permissions |= S_IRGRP;
            }
            else {
                permissions |= S_IROTH;
            }
        }
        else if (mode[i] == 'w') {
            if (i/3 == 0) {
                permissions |= S_IWUSR;
            }
            else if (i/3 == 1) {
                permissions |= S_IWGRP;
            }
            else {
                permissions |= S_IWOTH;
            }
        }
        else if (mode[i] == 'x') {
            if (i/3 == 0) {
                permissions |= S_IXUSR;
            }
            else if (i/3 == 1) {
                permissions |= S_IXGRP;
            }
            else {
                permissions |= S_IXOTH;
            }
        }
        else if (mode[i] != '-') {
            perror("Invalid permission character");
            return -1;
        }
    }
    
    if (chmod(path_to_file, permissions) == -1) {
        perror("Change mode error");
    }
    return 0;
}

int get_function(char* name) {
    char *slash_ptr = strrchr(name, '/');
    if (slash_ptr != NULL) {
        name = slash_ptr + 1;
    }
    int funcs_num = sizeof(functions) / sizeof(functions[0]);
    for (int i = 0; i < funcs_num; i++) {
        if (strcmp(name, functions[i].name) == 0) {
            return functions[i].number;
        }
    }
    return -1;
}

int main(int argc, char *argv[]) {
    int func = get_function(argv[0]);

    switch (func) {
        case mk_dir:
            if (argc < 2) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return mk_dirf(argv[1]);

        case ls_dir:
            if (argc < 2) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return ls_dirf(argv[1]);

        case rm_dir:
            if (argc < 2) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return rm_dirf(argv[1]);

        case mk_file:
            if (argc < 2) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return mk_filef(argv[1]);

        case cat_file:
            if (argc < 2) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return cat_filef(argv[1]);

        case rm_file:
            if (argc < 2) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return rm_filef(argv[1]);

        case ln_symlink:
            if (argc < 3) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return ln_symlinkf(argv[1], argv[2]);

        case cat_symlink:
            if (argc < 2) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return cat_symlinkf(argv[1]);

        case cat_file_symlink:
            if (argc < 2) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return cat_file_symlinkf(argv[1]);

        case rm_symlink:
            if (argc < 2) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return rm_symlinkf(argv[1]);

        case ln_hardlink:
            if (argc < 3) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return ln_hardlinkf(argv[1], argv[2]);

        case rm_hardlink:
            if (argc < 2) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return rm_hardlinkf(argv[1]);

        case stat_file:
            if (argc < 2) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return stat_filef(argv[1]);

        case ch_mod:
            if (argc < 3) {
                printf("Bad number of arguments\n");
                return -1;
            }
            return ch_modf(argv[1], argv[2]);

        default:
            printf("Undefined function\n");
            return -1;
    }
}