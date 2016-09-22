SRC=	keys.c
BIN=	keys
CC=	clang
FLAGS=	-std=c99 -Wall -Werror
LOG=	/var/log/keys
RC=	./$(BIN)-rc
RM=	/bin/rm

$(BIN): $(SRC)
	$(CC) $(SRC) $(FLAGS) -DOUTPUT_FILE='"$(LOG)"' -o $(BIN)

clean:
	$(RM) -f $(BIN) $(LOG)

install:
	cp $(BIN) /usr/sbin/
	cp $(RC) /usr/local/etc/rc.d/$(BIN)
	echo '$(BIN)_enable="YES"' >> /etc/rc.conf
