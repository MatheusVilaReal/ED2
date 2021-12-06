#	Projeto
PROJ = t2

#	.c
C_SOURCE = $(wildcard *.c)

#	.h
H_SOURCE = $(wildcard *.h)

#	.o
OBJ = $(C_SOURCE:.c = .o)

#	Compilador
CC = gcc

#	Flags
CC_FLAGS = -c                     	\
           -std=c99               	\
           -fstack-protector-all	\
		   -lm						\
		   -fstack-check

#	Compilação e linkagem
$(PROJ): $(OBJ)
	$(CC) -o $@ $^ -lm
	
%.o: %.c %.h
	$(CC) -o $@ $< $(CC_FLAGS) -lm
	
main.o: main.c $(H_SOURCE)
	$(CC) -o $@ $< $(CC_FLAGS) -lm
