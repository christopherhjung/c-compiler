#include "../../../lib/c4lib/c4lib.h"

void printf(char*, int);

int main(void) {
    char a = 'S';
    int b = 4;
    printf("Size of variable a : %d\n",sizeof(a));
    printf("Size of an expression : %d\n",sizeof(a+b));
    int s = a+b;
    printf("Size of explicitly converted expression : %d\n",sizeof(s));
    return 0;
}