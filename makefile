CC = g++
CFLAGS = -Wall -I "./includes" -Wno-write-strings -Wno-stringop-truncation -Wno-reorder
LDFLAGS =
OBJFILES = Program
TARGET = falsy

all: $(TARGET) run

$(OBJFILES):
	$(CC) $(CFLAGS) -c ./src/$@.cpp -o ./lib/$@.o

$(TARGET): $(OBJFILES)
	$(CC) -o ./bin/$(TARGET) ./lib/*.o $(LDFLAGS)

run:
	./bin/${TARGET}

clean:
	rm -f $(OBJFILES) $(TARGET) *~