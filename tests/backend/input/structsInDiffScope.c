void printf(char*, int);

int main(void)
{

    {
        struct T {
            int b;
        };

        struct T b;
        b.b = 42;
        printf("Hello World %d", b.b);
    }


    {
        struct T {
            int a;
        };

        struct T b;
        b.a = 43;
        printf("Hello World %d", b.a);
    }

    return 0;
}
