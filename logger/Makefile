SRC=	logger.c
BIN=	logger
CC=	clang
FLAGS=	-std=c99 -Wall -Werror
LOG=	/var/log/keys
RM=	/bin/rm

$(BIN): $(SRC)
	$(CC) $(SRC) $(FLAGS) -DOUTPUT_FILE='"$(LOG)"' -o $(BIN)

clean:
	$(RM) -f $(BIN) $(LOG)
