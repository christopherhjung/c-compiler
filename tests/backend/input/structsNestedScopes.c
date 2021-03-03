void printf(char*, int);

struct T {
    int a;
};


int main(void)
{
    struct T a;
    a.a = 9;
    printf("Hello World %d", a.a);

    {
        struct T {
            int b;
        };

        struct T b;
        b.b = 42;
        printf("Hello World %d", b.b);
    }

    return 0;
}