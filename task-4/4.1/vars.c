#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>

int global_inited = 500;
int global_not_inited;
int global_not_inited_array[8192];
const int global_const_not_inited;
const int global_const_inited = 25;
char *global_pstr = "Global hello from pstr!";
char global_array[] = "Global hello from array!";

void f(void) {
    int local_not_inited;
    int local_inited = 10;
    static int static_not_inited;
    static int static_inited = 50;
    const int const_not_inited;
    const int const_inited = 123;
    char *local_pstr = "Local hello from pstr!";
    char local_array[] = "Local hello from array!";

    printf("\n\tFunction variables:\n");
    printf("Local not inited:   %p (!! %d !!)\n", &local_not_inited, local_not_inited);
    printf("Local inited:       %p (%d)\n", &local_inited, local_inited);
    printf("Static not inited:  %p (!! %d !!)\n", &static_not_inited, static_not_inited);
    printf("Static inited:      %p (%d)\n", &static_inited, static_inited);
    printf("Const not inited:   %p (!! %d !!)\n", &const_not_inited, const_not_inited);
    printf("Const inited:       %p (%d)\n", &const_inited, const_inited);
    printf("Local pstr:     %p %p (%s)\n", &local_pstr, local_pstr, local_pstr);
    printf("Local array     %p %p (%s)\n", &local_array, local_array, local_array);
}

int* local_address(void) {
	int local_inited = 42;
	int *address = &local_inited;
	return address;
}

void heap(void) {
    printf("\n\tBuffer in heap:\n");
    char *p = malloc(100);
    char string1[] = "hello world";
    char string2[] = "HELLO WORLD";
    memcpy(p, string1, 12);

    printf("Buffer before free: %s\n", p);
    free(p);
    printf("Buffer after free: %s\n", p);

    char *s = malloc(100);
    memcpy(s, string2, 12);

    printf("New buffer before free: %s\n", s);
	s+=50;
    free(s);	//ERROR INVALID POINTER!!!
	s-=50;
    printf("New buffer after free: %s\n", s);
}

void environment_var(void) {
	printf("\n\tEnvironment variable:\n");
	char* env_var = getenv("ENV_VAR");
	printf("ENV_VAR before change: %p (%s)\n", env_var, env_var);
	setenv("ENV_VAR", "WORLD", 1);
	env_var = getenv("ENV_VAR");
	printf("ENV_VAR after change: %p (%s)\n", env_var, env_var);
}

int main() {
    printf("Function printf() from libc:    %p\n", printf);
    printf("Function time() from vdso:      %p\n", time);
    printf("My function f():        %p\n", f);

    f();

    printf("\n\tGlobal variables:\n");
    printf("Global inited:      %p (%d)\n", &global_inited, global_inited);
    printf("Global not inited   %p (!! %d !!)\n", &global_not_inited, global_not_inited);
    printf("Global not inited array:  %p %p (!! %d !!)\n", &global_not_inited_array, &global_not_inited_array[8100], global_not_inited_array);
    printf("Global const inited:    %p (%d)\n", &global_const_inited, global_const_inited);
    printf("Global const not inited:    %p (!! %d !!)\n", &global_const_not_inited, global_const_inited);
    printf("Global pstr:    %p %p (%s)\n", &global_pstr, global_pstr, global_pstr);
    printf("Global char array:  %p %p (%s)\n", &global_array, global_array, global_array);

	printf("\npid:	%d\n\n", getpid());

	int* temp = local_address();
    printf("Address of local inited var in function:   %p (!! %d !!)\n", temp, *temp);

	environment_var();
	//heap();
    //sleep(100);
}
