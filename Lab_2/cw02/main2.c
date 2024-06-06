#include <dlfcn.h>
#include <stdio.h>

int main (void){

    void *catch = dlopen("./libbibl1.so", RTLD_LAZY);
    if(!catch){printf("Open library error\n"); return 0;}

    int (*f2)(int, int);
    f2 = dlsym(catch, "test_collatz_convergence");
    if(dlerror() != 0){printf("Function 2 error\n"); return 0;}

    int input, max_iter, output;
    printf("Enter start value and maximum iteration number: \n");
    scanf("%d %d", &input, &max_iter);
    output = f2(input, max_iter);
    printf("%d\n", output);

    dlclose(catch);
    return 0;
}