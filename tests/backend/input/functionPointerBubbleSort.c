void c4_exit(int x);
void c4_assert(int x);
void *c4_malloc(int sz);
void *c4_calloc(int num, int sz);
void *c4_realloc(void* p, int sz);
void c4_free(void *p);
void c4_print_str(char* str);
void c4_scan_int(char* str, int* o);
void c4_sprintf(char* buf, char* str, int x);
void c4_print_int(int x);
void c4_error_str(char* str);
void c4_error_int(int x);
int c4_strcmp(char *a, char *b);
int c4_strncmp(char *a, char *b, int n);
char* c4_strcpy(char *a, char *b);
void* c4_memcpy(void *a, void *b, int size);
void c4_qsort(void *base, int nmemb, int size, int (*compar)(void *, void *));
int c4_strlen(char *a);
int c4_file_open(char *fname, char *mode);
void c4_file_close(void);
int c4_file_getc(void);
int c4_abs(int x);
int c4_atoi(char* str);
void c4_printf_int(char* str, int i) ;
void c4_printf_int_int(char* str, int i, int i2) ;

struct T{
    int label;
    int value;
};

void BubbleSort(struct T *a, int array_size, int (*f)(int, int))
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

    if (f(a[j].value , a[j+1].value))
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

int less(int a, int b){
    return a < b;
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

    BubbleSort(a, 20, &less);

    int i = 0;
    while(i < 20){
        c4_print_int(a[i].label);
        c4_print_str("\n");
        i = i + 1;
    }

    return 0;
}