typedef struct function {
	char* name;
	int number;
} function;

enum functions {
	mk_dir,
	ls_dir,
	rm_dir,
	mk_file,
	cat_file,
	rm_file,
	ln_symlink,
	cat_symlink,
	cat_file_symlink,
	rm_symlink,
	ln_hardlink,
	rm_hardlink,
	stat_file,
	ch_mod
};

function functions[] = {
        {"mk_dir", mk_dir},
        {"ls_dir", ls_dir},
        {"rm_dir", rm_dir},
        {"mk_file", mk_file},
        {"cat_file", cat_file},
        {"rm_file", rm_file},
        {"ln_symlink", ln_symlink},
        {"cat_symlink", cat_symlink},
        {"cat_file_symlink", cat_file_symlink},
        {"rm_symlink", rm_symlink},
        {"ln_hardlink", ln_hardlink},
        {"rm_hardlink", rm_hardlink},
        {"stat_file", stat_file},
        {"ch_mod", ch_mod}
};
/*
a. создать каталог, указанный в аргументе;
b. вывести содержимое каталога, указанного в аргументе;
c. удалить каталог, указанный в аргументе;
d. создать файл, указанный в аргументе;
e. вывести содержимое файла, указанного в аргументе;
f. удалить файл, указанный в аргументе;
g. создать символьную ссылку на файл, указанный в аргументе;
h. вывести содержимое символьной ссылки, указанный в аргументе;
i. вывести содержимое файла, на который указывает символьная ссылка, указанная в аргументе;
j. удалить символьную ссылку на файл, указанный в аргументе;
k. создать жесткую ссылку на файл, указанный в аргументе;
l. удалить жесткую ссылку на файл, указанный в аргументе;
m. вывести права доступа к файлу, указанному в аргументе и количество жестких ссылок на него;
n. изменить права доступа к файлу, указанному в аргументе
*/