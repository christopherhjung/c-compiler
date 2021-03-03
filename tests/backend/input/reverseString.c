


struct Test;

struct Test *c4_createStruct(void);
void c4_testStruct(struct Test *a);

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

void reverse_string(char*, int, int);

int main(void)
{
    char *str = "hallo palindrom";

    int size = c4_strlen(str);
    //This array would hold the string upto 150 char
    char *string_array = c4_malloc(size);

    c4_memcpy(string_array,str, size);

    string_array[1] = '2';

    //Calling our user defined function
    reverse_string(string_array, 0, c4_strlen(string_array)-1);
    c4_print_str("Reversed String is:");
    c4_print_str(string_array);

    return 0;
}

void reverse_string(char *x, int start, int end)
{
    char ch;
    if (start >= end)
        return;

    ch = *(x+start);
    *(x+start) = *(x+end);
    *(x+end) = ch;

    //Function calling itself: Recursion
    reverse_string(x, start + 1, end - 1);
}
