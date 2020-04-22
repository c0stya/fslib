# create a symbol table
echo "\
e   0
a   1
b   2
c   3" > sym.st

# create the first transducer
echo  "\
0 1 a a
1 2 b b
2 3 c c
3" | bin/fscompile --isym=sym.st --osym=sym.st > a.t

# create the second transducer
echo  "\
0 1 c c
1 2 b b
2 3 a a
3" | bin/fscompile --isym=sym.st --osym=sym.st > b.t

# create the edit transducer 
echo "\
0 0 a a 0
0 0 a b 1
0 0 a c 1
0 0 a e 1
0 0 b a 1
0 0 b b 0
0 0 b c 1
0 0 b e 1
0 0 c a 1
0 0 c b 1
0 0 c c 0
0 0 c e 1
0 0 e a 1
0 0 e b 1
0 0 e c 1
0" | bin/fscompile --isym=sym.st --osym=sym.st > ed.t

bin/fscompose a.t ed.t | bin/fscompose - b.t | bin/fsdraw --isym=sym.st --osym=sym.st | dot -Tpng > lev.png

rm a.t
rm b.t
rm ed.t
rm sym.st
