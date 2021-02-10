#include "../../../lib/c4lib/c4lib.h"

void printf(char*, int);

int main(void) {
    int a;
    char b;
    int *ap;
    char *bp;
    void *cp;

    printf("%d\n",sizeof(a));
    printf("%d\n",sizeof(b));
    printf("%d\n",sizeof(ap));
    printf("%d\n",sizeof(bp));
    printf("%d\n",sizeof(cp));

    printf("%d\n",sizeof('a'));
    printf("%d\n",sizeof("12345"));
    printf("%d\n",sizeof(8));
    printf("%d\n",sizeof(sizeof(1)));
    return 0;
}