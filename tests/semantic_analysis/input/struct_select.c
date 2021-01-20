struct t1 { int m; };
struct t2 { int m; };


int f(struct t1 *p1, struct t2 *p2) {
    if (p1->m < 0)
        p2->m = -p2->m;
    return p1->m;
}

int g(void) {
    struct {
        struct t1 s1;
        struct t2 s2;
    }u;

    return f(&u.s1, &u.s2);
}
