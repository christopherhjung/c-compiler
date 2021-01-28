#include "../lib/c4lib/c4lib.h"

struct node {
    int data;
    int key;
    struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//display the list
void printList(void) {
    struct node *ptr = head;
    c4_print_str("\n[ ");

    //start from the beginning
    while(ptr != NULL) {
        c4_printf_int_int("(%d,%d) ",ptr->key,ptr->data);
        ptr = ptr->next;
    }

    c4_print_str(" ]");
}

//insert link at the first location
void insertFirst(int key, int data) {
    //create a link
    struct node *link = c4_malloc(sizeof(struct node));

    link->key = key;
    link->data = data;

    //point it to old first node
    link->next = head;

    //point first to new first node
    head = link;
}

//delete first item
struct node* deleteFirst(void) {

    //save reference to first link
    struct node *tempLink = head;

    //mark next to first link as first
    head = head->next;

    //return the deleted link
    return tempLink;
}

//is list empty
int isEmpty(void) {
    return head == NULL;
}

int length(void) {
    int length = 0;
    struct node *current;
    current = head;
    while(current != NULL){
        length=length+1;
        current = current->next;
    }

    return length;
}

//find a link with given key
struct node* find(int key) {

    //start from the first link
    struct node* current = head;

    //if list is empty
    if(head == NULL) {
        return NULL;
    }

    //navigate through list
    while(current->key != key) {

        //if it is last node
        if(current->next == NULL) {
            return NULL;
        } else {
            //go to next link
            current = current->next;
        }
    }

    //if data found, return the current Link
    return current;
}

//delete a link with given key
struct node* delete(int key) {

    //start from the first link
    struct node* current = head;
    struct node* previous = NULL;

    //if list is empty
    if(head == NULL) {
        return NULL;
    }

    //navigate through list
    while(current->key != key) {

        //if it is last node
        if(current->next == NULL) {
            return NULL;
        } else {
            //store reference to current link
            previous = current;
            //move to next link
            current = current->next;
        }
    }

    //found a match, update the link
    if(current == head) {
        //change first to point to next link
        head = head->next;
    } else {
        //bypass the current link
        previous->next = current->next;
    }

    return current;
}

void sort(void) {

    int i;
    int j;
    int k;
    int tempKey;
    int tempData;
    struct node *current;
    struct node *next;

    int size = length();
    k = size ;
    i = 0;
    while ( i < size - 1   ) {
        current = head;
        next = head->next;
        j = 1;
        while (  j < k  ) {

            if ( current->data > next->data ) {
                tempData = current->data;
                current->data = next->data;
                next->data = tempData;

                tempKey = current->key;
                current->key = next->key;
                next->key = tempKey;
            }

            current = current->next;
            next = next->next;
            j=j+1;
        }
        i=i+1;
        k=k-1;
    }
}

void reverse(struct node** head_ref) {
    struct node* prev   = NULL;
    struct node* current = *head_ref;
    struct node* next;

    while (current != NULL) {
        next  = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    *head_ref = prev;
}

int main(void) {
    insertFirst(1,10);
    insertFirst(2,20);
    insertFirst(3,30);
    insertFirst(4,1);
    insertFirst(5,40);
    insertFirst(6,56);

    c4_print_str("Original List: ");

    //print list
    printList();

    while(!isEmpty()) {
        struct node *temp = deleteFirst();
        c4_print_str("\nDeleted value:");
        c4_printf_int_int("(%d,%d) ",temp->key,temp->data);
    }

    c4_print_str("\nList after deleting all items: ");
    printList();
    insertFirst(1,10);
    insertFirst(2,20);
    insertFirst(3,30);
    insertFirst(4,1);
    insertFirst(5,40);
    insertFirst(6,56);

    c4_print_str("\nRestored List: ");
    printList();
    c4_print_str("\n");

    struct node *foundLink = find(4);

    if(foundLink != NULL) {
        c4_print_str("Element found: ");
        c4_printf_int_int("(%d,%d) ",foundLink->key,foundLink->data);
        c4_print_str("\n");
    } else {
        c4_print_str("Element not found.");
    }

    delete(4);
    c4_print_str("List after deleting an item: ");
    printList();
    c4_print_str("\n");
    foundLink = find(4);

    if(foundLink != NULL) {
        c4_print_str("Element found: ");
        c4_printf_int_int("(%d,%d) ",foundLink->key,foundLink->data);
        c4_print_str("\n");
    } else {
        c4_print_str("Element not found.");
    }

    c4_print_str("\n");
    sort();

    c4_print_str("List after sorting the data: ");
    printList();

    reverse(&head);
    c4_print_str("\nList after reversing the data: ");
    printList();

    return 0;
}