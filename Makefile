CFLAGS = -Wall -Wextra -ggdb3 --std=c99
LIB =
INC =
main: main.c
	gcc $(CFLAGS) $(LIB) $(INC) -o $@.o $^ -lslurm
clean:
	rm -f *.o *.out
