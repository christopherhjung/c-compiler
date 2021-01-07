int p;
int x;
int y;

void lol(void);

struct somename;
struct somename value;
struct { int noname; };
struct { int noname; } value2;
struct { int somevalue; char nextdecl; } value21;
struct { int somevalue; struct { int inner; }; } nested;
struct { int somevalue; struct { int inner; } id; } nested2;
struct name { int foo; };
struct name2 { int foo; } value3;
void nm(int (void));
void nm(int (x)(void));

void lol1(int);
void lol2(int *);
void lol3(int (*));
void lol1n(int x);
void lol2n(int *x);
void lol3n(int (*x));
void lol4(int (()(void)));
void lol5(int (()(void)));
void lol61(int x);
void **(((***funptr)(int (((x))))));

void def1(int x, int y, struct withname z) {}
void def2(int x, int y, struct { int x; } z) {}
void def3(int x, int y, struct { int (*x)(void); } z) {}

int fun(int)(int)(int);

int statements(int x, int y) {
  goto foo;
  foo:
  x;
  x;;;
  {{ ; }}
  int z;
  if (x) if (y) return x; else return x;
  while (x) continue;
  while (x) {
    int z;
    continue;
    break;
  }
}
void expressions(int z, int y) {
  ;
  1;
  x;
  'c';
  "str";
  0;
  int foo;
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
  1?2?3:4:5;
  1?2:3?4:5;
  (1?2:3)?4:5;
  int *x;
  x=0;
  sizeof!-*&x[1];
  1*2+3-4<5==6!=7&&8||9?10:11;
  1?2:3||4&&5!=6==7<8-9+10*11;
  1=2?3=4:5;
}
