.PHONY:clean
CC=gcc
CFLAGES= -g
LDFLAGES=-L$(PRE_PATH)/lib
BIN=post1
SNAP=$(BIN_DIR)/snap
OBJS=base64.o post1.o cJSON.o
LIBS= -lm

$(BIN):$(OBJS)
	$(CC) $(CFLAGES) $(LDFLAGES) $^ -o $@ $(LIBS)
	gcc snap.c -o ./bin/snap
%.o:%.c
	$(CC) $(CFLAGES) -c $< -o $@
clean:
	rm -f *.o $(BIN)
