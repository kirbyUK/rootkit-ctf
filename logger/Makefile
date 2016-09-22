SRC=	logger.c
BIN=	logger
CC=	clang
FLAGS=	-std=c99 -Wall -Werror
RM=	/bin/rm

$(BIN): $(SRC)
	$(CC) $(SRC) $(FLAGS) -o $(BIN)

clean:
	$(RM) -f $(BIN)
