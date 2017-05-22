del /Q *.exe
del /Q *.o
del /Q *.scanner.*
del /Q *.parser.*

bison --warnings=all -d -o minicoin.parser.c minicoin.y
flex -ominicoin.scanner.c minicoin.l

@pause

gcc -c minicoin.scanner.c -o minicoin.lex.o
gcc -c minicoin.parser.c -o minicoin.y.o
gcc -Wextra -c minicoin_tree.c -o minicoin_tree.o
gcc -Wextra -c minicoin_eval.c -o minicoin_eval.o
g++ -o minicoin *.o

@pause

minicoin -f in.txt
