flags=-02 -Wall --std=c2x
ldflags=-lbu

.PHONY: all clean

all: clean antiviRus

antiviRus: antiviRus.o
	cc $(flags) $^ -o $@ $(ldflags)

antiviRus.o: antiviRus.c antiviRus.h
	cc $(flags) -c $<

clean: rm -f *.o antiviRus