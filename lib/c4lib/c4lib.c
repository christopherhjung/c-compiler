
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "c4lib.h"

struct Test{
    int i;
};

struct Test *c4_createStruct(void){
    struct Test *test = (struct Test*)c4_malloc(sizeof(struct Test));
    test->i = 999;
    return test;
}

void c4_testStruct(struct Test *a){
    c4_print_int(a->i);
}

void c4_exit(int x) {
    exit(x);
}

void c4_assert(int x) {
    assert(x);
}

void *c4_malloc(int sz) {
    return malloc(sz);
}

void *c4_calloc(int num, int sz) {
    return calloc(num, sz);
}

void c4_scan_int(char* str, int* o){
    scanf(str, o);
}

void *c4_realloc(void* p, int sz) {
    return realloc(p, sz);
}

void c4_free(void *p) {
    free(p);
}

void c4_print_str(char* str) {
    printf("%s", str);
}

void c4_printf_int(char* str, int i) {
    printf(str, i);
}

void c4_printf_int_int(char* str, int i, int i2) {
    printf(str, i, i2);
}

void c4_sprintf(char* buf, char* str, int x) {
    sprintf(buf, str, x);
}

void c4_print_int(int x) {
    printf("%d", x);
}

void c4_error_str(char* str) {
    fprintf(stderr, "%s", str);
}

void c4_error_int(int x) {
    fprintf(stderr, "%d", x);
}

int c4_strncmp(char *a, char *b, int n) {
    return strncmp(a, b, n);
}

int c4_strcmp(char *a, char *b) {
    return strcmp(a, b);
}

char* c4_strcpy(char *a, char *b) {
    return strcpy(a, b);
}

void* c4_memcpy(void *a, void *b, int size) {
    return memcpy(a, b, size);
}

void c4_qsort(void *base, int nmemb, int size, int (*compar)(void *, void *)) {
    qsort(base, nmemb, size, (int (*)(const void*, const void*)) compar);
}

int c4_strlen(char *a) {
    return strlen(a);
}

static FILE* current_file = NULL;

int c4_file_open(char *fname, char *mode) {
    assert(current_file == NULL);
    current_file = fopen(fname, mode);
    return current_file != NULL;
}

void c4_file_close(void) {
    assert(current_file != NULL);
    fclose(current_file);
    current_file = NULL;
}

int c4_file_getc(void) {
    assert(current_file != NULL);
    return getc(current_file);
}

int c4_abs(int x) {
    return abs(x);
}

int c4_atoi(char* str) {
    return atoi(str);
}
