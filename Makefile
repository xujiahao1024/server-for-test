SRC += server.c main.c queue.c deal.c
OBJ = $(SRC:%.c=%.o)
CC = gcc
EXEC_NAME = server
FLAG = -l pthread
demo:$(OBJ)
	$(CC) $^ -o $(EXEC_NAME) $(FLAG)
clean:
	rm $(EXEC_NAME) *.o -rf
