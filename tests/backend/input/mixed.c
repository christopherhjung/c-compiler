void printf(char*, int);

struct T {
    int value;
    struct T *parent;
};

int getParentValue(struct T *t){
    return t->parent->value;
}

void simplePrint(void){
    printf("hello", 1);
}

int main(void){

    struct T parent;
    struct T node;
    node.parent = &parent;
    parent.value = 69;
    printf("hello %d", getParentValue(&node));
    simplePrint();
    return 0;
}