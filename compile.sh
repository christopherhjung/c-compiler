BASEDIR=$(dirname "$0")

gcc -E -C -P -o $BASEDIR/out/out.c $1 &&\
(cd $BASEDIR/out; ../build/debug/c4 --compile ./out.c ) &&\
clang -Wno-everything -o $BASEDIR/out/out $BASEDIR/lib/c4lib/c4lib.c $BASEDIR/out/out.ll &&\
$BASEDIR/out/out