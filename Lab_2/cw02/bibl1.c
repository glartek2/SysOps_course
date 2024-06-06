#include<stdio.h>

int collatz_conjecture(int input){
    if (input % 2 == 0){
        input = input / 2;
        return input;
    } else {
        input = 3*input + 1;
        return input;
    }
}


int test_collatz_convergence(int input, int max_iter){
    for(int i=0; i < max_iter; i++){
        input = collatz_conjecture(input);
        if (input == 1){
            return i;
        }
    }
    return -1;
}