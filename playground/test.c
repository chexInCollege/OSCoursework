#include <stdio.h>

int add (int a, int b) { return a + b; }
int sub (int a, int b) { return a - b; }

int calculate(int(*func)(int, int), int a, int b) {
    return func(a, b);
}

int main() {
    printf("%d\n", calculate(sub, 1, 5));
    printf("%d\n", calculate(add, 1, 5));
}