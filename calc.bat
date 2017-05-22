del minipseudtree.o
del minipseudeval.o
del minipseud.lex.c
del minipseud.exe
del minipseud.lex.o
del minipseud.h
del minipseud.y.c
del minipseud.y.o

pause
bison -d minipseud.y
rename minipseud.tab.h minipseud.h
rename  minipseud.tab.c minipseud.y.c
flex minipseud.l
rename  lex.yy.c minipseud.lex.c
gcc -c minipseudtree.c -o minipseudtree.o
gcc -c minipseudeval.c -o minipseudeval.o
gcc -c minipseud.lex.c -o minipseud.lex.o
gcc -c minipseud.y.c -o minipseud.y.o
gcc -o minipseud minipseud.lex.o minipseud.y.o minipseudtree.o minipseudeval.o 
minipseud -f in.txt
pause
