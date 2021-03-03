
void printf(char*, int);

int result(int a){
    return a;
}

int main(void) {
    int a = 9;
    printf("%d\n",a + result(a = a + 1));
    return 0;
}
