gcc -E -C -P -o ./out/out.c ./test/test.c
(cd out; ../build/debug/c4 --compile ./out.c )
clang -Wno-everything -o ./out/out ./lib/c4lib/c4lib.c ./out/out.ll
./out/out