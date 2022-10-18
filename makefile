FLAGS = -g -Wall

all: clean lab2 run

lab2: lab2.c
	gcc $(FLAGS) -o lab2 lab2.c

run:
	./lab2

clean:
	rm -f padre hijo *.o