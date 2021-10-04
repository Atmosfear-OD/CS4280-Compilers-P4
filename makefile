CC	= g++
CFLAGS	= -g
TARGET	= compfs
OBJS	= main.o scanner.o parser.o staticSem.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

scanner.o: scanner.cpp
	$(CC) $(CFLAGS) -c scanner.cpp

parser.o: parser.cpp
	$(CC) $(CFLAGS) -c parser.cpp

staticSem.o: staticSem.cpp
	$(CC) $(CFLAGS) -c staticSem.cpp

clean:
	rm -rf *.o compfs
