trabalhoFinal.out: main.o estruturaDeDados.o
	gcc -Wall -Wextra main.o estruturaDeDados.o -o trabalhoFinal.out

main.o : main.c trabalho.h
	gcc -Wall -Wextra main.c -c -o main.o

estruturaDeDados.o : estruturaDeDados.c trabalho.h
	gcc -Wall -Wextra estruturaDeDados.c -c -o estruturaDeDados.o
