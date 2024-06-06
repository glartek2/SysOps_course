extern int test_collatz_convergence(int, int);

#include <stdio.h>

int main (void){ 
    int input, max_iter, output;
    printf("Enter start value and maximum iteration number: \n");
    scanf("%d %d", &input, &max_iter);
    output = test_collatz_convergence(input, max_iter);
    printf("%d\n", output);

    return 0;
}