#	Projeto
PROJ = t1

#	.c
C_SOURCE = $(wildcard *.c)

#	.h
H_SOURCE = $(wildcard *.h)

#	.o
OBJ = $(C_SOURCE:.c = .o)

#	Compilador
CC = gcc

#	Flags
CC_FLAGS = -c                     \
           -std=c99               \
           -fstack-protector-all

#	Compilação e linkagem
t1: $(OBJ)
	$(CC) -o $@ $^
	
%.o: %.c %.h
	$(CC) -o $@ $< $(CC_FLAGS)
	
main.o: main.c $(H_SOURCE)
	$(CC) -o $@ $< $(CC_FLAGS)
