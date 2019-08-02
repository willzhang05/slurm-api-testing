CFLAGS = -Wall -Wextra -ggdb3 --std=c99
LIB =
INC =
slurmutil: main.c
	gcc $(CFLAGS) $(LIB) $(INC) -o $@ $^ -lslurm
clean:
	rm -f slurmutil *.o *.out
