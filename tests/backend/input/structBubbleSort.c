#include "../../../lib/c4lib/c4lib.h"

struct T{
    int label;
    int value;
};

void BubbleSort(struct T *a, int array_size)
{
    int i;
    int j;
    struct T temp;

    // for (i = 0; i < (array_size - 1); ++i)
    i = 0;
    BEGINFOR:
    if (i >= (array_size - 1)) goto ENDFOR;

    // for (j = 0; j < array_size - 1 - i; ++j)
    j = 0;
    BEGININNERFOR:
    if (j >= array_size - 1 - i) goto ENDINNERFOR;

    if (a[j].value > a[j+1].value)
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

    struct T *a = c4_malloc(20 * sizeof(struct T));

    a[0].value = 97;
    a[0].label = 97;
    a[1].value = 83;
    a[1].label = 83;
    a[2].value = 62;
    a[2].label = 62;
    a[3].value = 13;
    a[3].label = 13;
    a[4].value = 51;
    a[4].label = 51;
    a[5].value = 83;
    a[5].label = 83;
    a[6].value = 15;
    a[6].label = 15;
    a[7].value = 78;
    a[7].label = 78;
    a[8].value = 91;
    a[8].label = 91;
    a[9].value = 40;
    a[9].label = 40;
    a[10].value = 36;
    a[10].label = 36;
    a[11].value = 13;
    a[11].label = 13;
    a[12].value = 61;
    a[12].label = 61;
    a[13].value = 99;
    a[13].label = 99999;
    a[14].value = 66;
    a[14].label = 66;
    a[15].value = 22;
    a[15].label = 22;
    a[16].value = 28;
    a[16].label = 28;
    a[17].value = 41;
    a[17].label = 41;
    a[18].value = 88;
    a[18].label = 88;
    a[19].value = 22;
    a[19].label = 22;

    BubbleSort(a, 20);

    int i = 0;
    while(i < 20){
        c4_print_int(a[i].label);
        c4_print_str("\n");
        i = i + 1;
    }

    return 0;
}