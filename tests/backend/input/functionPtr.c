


int compute(int x, int y, int (*func)(int, int))
{
    return (*func)(x, y);

    //printf("value is : %d\n", (*func)(x, y));
}

void printf(char*, int);


int add(int i, int j)
{
    return (i + j);
}

int sub(int i, int j)
{
    return (i - j);
}


int main(void)
{
    return compute(99,100, &add) - compute(1,3, &sub);
}