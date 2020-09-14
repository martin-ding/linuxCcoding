src = $(wildcard *.c)
targets = $(patsubst %.c, %, $(src))


CFLAG = -Wall -g -pthread

all: serverM client


serverM:serverM.o wrap.o
	gcc serverM.o wrap.o $(CFLAG) -o serverM  

client:client.o wrap.o
	gcc client.o wrap.o $(CFLAG) -o client

%.o:%.c
	gcc -c $< $(CFLAG)

.PHONY: all
clean:
	-rm -rf $(targets) test 
