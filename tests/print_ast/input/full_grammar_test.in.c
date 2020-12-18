int;
void;
int x;
int y;

void k;

void lol(void);

struct somename;
struct somename value;
struct { int noname; };
struct { int noname; } value2;
struct { int somevalue; char nextdecl; } value21;
struct { int somevalue; struct { int inner; }; } nested;
struct { int somevalue; struct { int inner; } id; } nested2;
struct name { int foo; };
struct name { int foo; } value3;
void nm(int (((())())));
void nm(int (((())(void))));
void nm(int ((((x))(void))));

void lol1(int);
void lol2(int *);
void lol3(int (*));
void lol1n(int x);
void lol2n(int *x);
void lol3n(int (*x));
void lol4(int (()(void)));
void lol5(int (()(void)));
void lol61(int x);
void **(((***funptr)(int (((x))), void)));

void def1(int x, int y, struct withname z) {}
void def2(int x, int y, struct { int x; } z) {}
void def3(int x, int y, struct withname { int (*x)(void); } z) {}

int fun(int)(int)(int);

void statements(int x, int y) {
  goto foo;
  foo:
  x;
  x;;;
  {{ ; }}
  int z;
  if (x) if (y) return; else return x;
  while (x) continue;
  while (x) {
    int z;
    continue;
    break;
  }
}
void expressions(int x, int y) {
  ;
  1;
  x;
  'c';
  "str";
  0;
  1[1];
  1();
  1(1);
  1(1, 1);
  1.x;
  1->x;
  &1;
  *1;
  -1;
  !1;
  sizeof 1;
  sizeof(1);
  sizeof(foo);
  sizeof(struct foosizeof);
  sizeof(int ()(int x,int y));
  sizeof(int (*)(int,int));
  1 + 1;
  1 * 1;
  1 - 1;
  1 < 1;
  1 == 1;
  1 != 1;
  1 && 1;
  1 || 1;
  1 ? 1 : 1;
  1 = 1;
  ****((((1))))[1][1][1][1];
  1?2?3:4:5;
  1?2:3?4:5;
  (1?2:3)?4:5;
  x=y=3;
  sizeof!-*&x[1]().x->y;
  1*2+3-4<5==6!=7&&8||9?10:11;
  1?2:3||4&&5!=6==7<8-9+10*11;
  1=2?3=4:5;
}
