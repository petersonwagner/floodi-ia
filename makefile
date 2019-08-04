CFLAGS = -O0 -g -Wall -Wno-unused-result

all: flood

flood: flood.c utils.c utils.h
	gcc $(CFLAGS) flood.c utils.c utils.h -o flood

clean:
	@echo "Limpando sujeira ..."
	@rm -f flood *.o

distclean: clean
	@echo "Limpando tudo ..."
	@rm -f *.o core
