
CFLAGS = -Wall

all:
	$(CC) $(CFLAGS) -std=c99 -o main main.c -lm -lpthread
	$(CC) $(CFLAGS) -o matrixgen matrixgen.c
	$(CC) $(CFLAGS) -o serialtester serialtester.c

test: all
	./matrixgen 1024
	./main 1
	./main 4
	./serialtester

clean:
	rm -rf main
	rm -rf matrixgen
	rm -rf serialtest