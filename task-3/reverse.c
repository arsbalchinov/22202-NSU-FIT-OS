#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096
#define STR_SIZE 256

void reverse(char *str, int len) {
    if (!str) {
        return;
    }
    char temp;
    for (int i = 0; i < len/2; i++) {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void add_to_path(char *path, char *filename) {
    int path_len = strlen(path);
    int file_len = strlen(filename);
    path[path_len] = '/';
    path[path_len + 1] = '\0';
    strncat(path, filename, file_len);
}

int reverse_file(const char *from, const char *to) {
    int file_from;	//Файловые дескрипторы
    int file_to;
    file_from = open(from, O_RDONLY);
    if (file_from < 0) {
        perror("Source file open error");
        return -1;
    }
	struct stat statf;
    fstat(file_from, &statf);

    file_to = open(to, O_WRONLY | O_CREAT | O_TRUNC, statf.st_mode);
    if (file_to < 0) {
        perror("Destination file open error");
        return -1;
    }

    int end = statf.st_size - 1;
    char buffer[BUFFER_SIZE] = {0};

    while (end != 0) {
        end = end - BUFFER_SIZE < 0 ? 0 : end - BUFFER_SIZE;	//Уменьшаем end от size до 0

        lseek(file_from, end, SEEK_SET);	//Смещаемся от конца на end байт

        int n = read(file_from, buffer, BUFFER_SIZE);	//Читаем BUFFER_SIZE байт в buffer от (size - end) до конца

        reverse(buffer, n);

        write(file_to, buffer, n);	//Записываем перевёрнутое содержимое файла
    }

    close(file_from);
    close(file_to);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 0;
    }
    char orig_dir[STR_SIZE] = {0};
    char new_dir[STR_SIZE] = {0};

    if (new_dir != getcwd(new_dir, BUFFER_SIZE)) {
        perror("Read directory error");
        return -1;
    }

    DIR *directory = opendir(argv[1]);
    if (directory == NULL) {
        return -1;
    }

    strncpy(orig_dir, argv[1], strlen(argv[1]));
    char *slash = strrchr(argv[1], '/');
    char temp[STR_SIZE];
    int temp_len;

    if (slash != NULL) {
        temp_len = strlen(slash) - 1;
        strncpy(temp, slash + 1, temp_len);
    }
    else {
        temp_len = strlen(argv[1]);
        strncpy(temp, argv[1], temp_len);
    }

    reverse(temp, temp_len);
    add_to_path(new_dir, temp);

    int result = mkdir(new_dir, S_IRWXU);	//Создаём директорию
    if (result != 0) {
        perror("Create directory error");
        return -1;
    }

    int orig_end = strlen(orig_dir);
    int new_end = strlen(new_dir);

    struct dirent *file;
    while ((file = readdir(directory)) != NULL) {
        if (file->d_type == DT_REG) {  //Проверка файла на регулярность
            orig_dir[orig_end] = '\0';
            new_dir[new_end] = '\0';

            add_to_path(orig_dir, file->d_name);
            char *slash_ptr = strrchr(file->d_name, '/');
            int len = 0;

            if (slash_ptr != NULL) {
                len = slash_ptr - file->d_name;
            }
            else {
                len = strlen(file->d_name);
            }

            reverse(file->d_name, len);

            add_to_path(new_dir, file->d_name);

            int res = reverse_file(orig_dir, new_dir);
            if (res != 0) {
                return -1;
            }
        }
    }
    closedir(directory);
}