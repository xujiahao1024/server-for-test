SRC += ./data_struct/data_struct.c 
SRC += ./server/server.c 
SRC += ./server/deal.c 
SRC += ./server/register.c
SRC += ./server/send.c
SRC += ./server/heart.c
SRC += ./server/cmd.c
SRC += ./server/update.c
SRC += main.c 
OBJ = $(SRC:%.c=%.o)
CC = gcc
EXEC_NAME = demo
FLAG = -l pthread
demo:$(OBJ)
	$(CC) $^ -o $(EXEC_NAME) $(FLAG)
clean:
	rm $(EXEC_NAME) *.o ./server/*.o ./data_struct/*.o -rf
