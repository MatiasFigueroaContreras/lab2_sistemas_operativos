FLAGS = -g -Wall

all: clean lab2 broker worker run

lab2: lab2.c
	gcc $(FLAGS) -o lab2 lab2.c

broker: broker.c fbroker.o yearData.o
	gcc $(FLAGS) -o broker fbroker.o yearData.o broker.c

worker: worker.c fworker.o yearData.o
	gcc $(FLAGS) -o worker worker.c fworker.o yearData.o

fbroker.o: fbroker.c fbroker.h
	gcc $(FLAGS) -c fbroker.c

fworker.o: fworker.c fworker.h
	gcc $(FLAGS) -c fworker.c
	
yearData.o: yearData.c yearData.h
	gcc $(FLAGS) -c yearData.c

run:
	./lab2

clean:
	rm -f lab2 broker worker *.o