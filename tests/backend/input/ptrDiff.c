void printf(char*,int);

int main(void) {
    void *null = 0;

    int *a = 0;
    char *b = 0;
    void *c = 0;

    void *r = 0;

    r = a + 1;
    printf("%d\n" , r - null);

    r = b + 1;
    printf("%d\n" , r - null);

    r = c + 1;
    printf("%d\n" , r - null);

    return 0;
}