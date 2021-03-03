void printf(char*,int);


int main(void)
{
    int num1 = 9;
    int num2 = 7;
    int num3 = 11;
    int *p1;
    int *p2;
    int *p3;


    //assigning the address of input numbers to pointers
    p1 = &num1;
    p2 = &num2;
    p3 = &num3;
    if(*p1 > *p2)
    {
        if(*p1 > *p3)
        {
            printf("%d is the largest number", *p1);
        }
        else
        {
            printf("%d is the largest number", *p3);
        }
    }
    else
    {
        if(*p2 > *p3)
        {
            printf("%d is the largest number", *p2);
        }
        else
        {
            printf("%d is the largest number", *p3);
        }
    }
    return 0;
}