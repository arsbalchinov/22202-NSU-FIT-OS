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

void update_path(char* orig_path, int op_length, char* invr_path, int ip_length) {
    orig_path[op_length] = '\0';
    invr_path[ip_length] = '\0';
}

int reverse_file(const char *from, const char *to) {
    int file_from;  //Файловые дескрипторы
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
        end = end - BUFFER_SIZE < 0 ? 0 : end - BUFFER_SIZE;    //Уменьшаем end от size до 0

        lseek(file_from, end, SEEK_SET);    //Смещаемся от конца на end байт

        int n = read(file_from, buffer, BUFFER_SIZE);   //Читаем BUFFER_SIZE байт в buffer от (size - end) до конца

        reverse(buffer, n);

        write(file_to, buffer, n);  //Записываем перевёрнутое содержимое файла
    }

    close(file_from);
    close(file_to);
    return 0;
}

int reverse_directory(const char* path, const char* reversed_path) {
    DIR *directory = opendir(path);
    if (directory == NULL) {
        return -1;
    }
    if (mkdir(reversed_path, S_IRWXU) != 0) {   //Создаём директорию
        perror("Create directory error");
        return -1;
    }

    int path_len = strlen(path);
    int revp_len = strlen(reversed_path);
    char orig_dir[STR_SIZE] = {0};
    char new_dir[STR_SIZE] = {0};

    strncpy(orig_dir, path, path_len);
    strncpy(new_dir, reversed_path, revp_len);

    int orig_end = strlen(orig_dir);
    int new_end = strlen(new_dir);

    struct dirent *file;

    while ((file = readdir(directory)) != NULL) {   //Проходим по всем файлам и директориям
        if (file->d_type != DT_REG && file->d_type != DT_DIR) {
            continue;
        }
        char *slash_ptr = strrchr(file->d_name, '/');   //Указатель на '/' или NULL
        int len = slash_ptr == NULL ? strlen(file->d_name) : strlen(slash_ptr) - 1;  //Длина названия без '/'
        if (file->d_name[len-1] == '~' || strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
            continue;
		}
        update_path(orig_dir, orig_end, new_dir, new_end);
        add_to_path(orig_dir, file->d_name);

        reverse(file->d_name, len);

        add_to_path(new_dir, file->d_name);

        if (file->d_type == DT_REG) {  //Проверка файла на регулярность
            reverse_file(orig_dir, new_dir);
        }
        else {
            reverse_directory(orig_dir, new_dir);
        }
	}
    closedir(directory);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 0;
    }
    char new_dir[STR_SIZE];

    if (new_dir != getcwd(new_dir, BUFFER_SIZE)) {
        perror("Read directory error");
        return -1;
    }
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

    reverse_directory(argv[1], new_dir);
}