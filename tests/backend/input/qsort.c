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


void printf(char*, int);


void printArray(int* A, int size)
{
    int i = 0;
    while ( i < size ){
        if(i > 0){
            c4_print_str(" ");
        }
        c4_print_int(A[i]);
        i=i+1;
    }
}

int compare(void* ap, void *bp){
    int *aIp = ap;
    int *bIp = bp;

    return *aIp < *bIp;
}

// Driver code
int main(void)
{
    int* arr = c4_malloc(6 * sizeof(int));

    arr[0] = 12;
    arr[1] = 11;
    arr[2] = 13;
    arr[3] = 5;
    arr[4] = 6;
    arr[5] = 7;

    int arr_size = 6;

    c4_print_str( "Given array is\n");
    printArray(arr, arr_size);

    c4_qsort(arr, 6, 4, compare);

    c4_print_str( "\nSorted array is\n");
    printArray(arr, arr_size);
    return 0;
}