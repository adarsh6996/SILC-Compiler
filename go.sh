yacc -d ast.y
lex ast.l
yacc ast.y
gcc lex.yy.c y.tab.c
./a.out testfile
lex ltrans.l
gcc lex.yy.c
./a.out
cp -i opfile1.xsm ./xsm_expl/
cd xsm_expl
./xsm -l library.lib -e opfile1.xsm
