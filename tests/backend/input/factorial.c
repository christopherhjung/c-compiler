void printf(char*,int);

int factorial(int n) {

    if(!n) {
        return 1;
    } else {
        return n * factorial(n-1);
    }
}

int fibbonacci(int n) {
    if(!n){
        return 0;
    } else if(n == 1) {
        return 1;
    } else {
        return (fibbonacci(n-1) + fibbonacci(n-2));
    }
}

int main(void) {
    int n = 5;
    int i = 0;

    printf("Factorial of %d" , !i );
    printf(": %d\n" , factorial(n) );
    printf("Fibbonacci of %d: " , n);

    while(i<n) {
        printf("%d ",fibbonacci(i));
        i = i + 1;
    }
}