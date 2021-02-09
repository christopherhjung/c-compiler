#include "../../../lib/c4lib/c4lib.h"


void BubbleSort(int *a, int array_size)
{
    int i;
    int j;
    int temp;

    // for (i = 0; i < (array_size - 1); ++i)
    i = 0;
    BEGINFOR:
    if (i >= (array_size - 1)) goto ENDFOR;

    // for (j = 0; j < array_size - 1 - i; ++j)
    j = 0;
    BEGININNERFOR:
    if (j >= array_size - 1 - i) goto ENDINNERFOR;

    if (a[j] > a[j+1])
    {
        temp = a[j+1];
        a[j+1] = a[j];
        a[j] = temp;
    }

    j = j +1;
    goto BEGININNERFOR;
    ENDINNERFOR:

    i=i+1;
    goto BEGINFOR;
    ENDFOR:;
}

int main(void) {

    int *a = c4_malloc(20 * sizeof(int));

    a[0] = 97;
    a[1] = 83;
    a[2] = 62;
    a[3] = 13;
    a[4] = 51;
    a[5] = 83;
    a[6] = 15;
    a[7] = 78;
    a[8] = 91;
    a[9] = 40;
    a[10] = 36;
    a[11] = 13;
    a[12] = 61;
    a[13] = 99;
    a[14] = 66;
    a[15] = 22;
    a[16] = 28;
    a[17] = 41;
    a[18] = 88;
    a[19] = 22;

    BubbleSort(a, 20);

    int i = 0;
    while(i < 20){
        c4_print_int(a[i]);
        c4_print_str("\n");
        i = i + 1;
    }

    return 0;
}