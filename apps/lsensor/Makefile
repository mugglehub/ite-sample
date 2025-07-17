CC=gcc
ITE=../../ite

CFLAGS=-I. -Wall

CFLAGS+=-I$(ITE)

LDFLAGS=-lncurses

SRCS=$(ITE)/ite.c
SRCS+=$(ITE)/acpi.c
SRCS+=$(ITE)/ite-mbx.c
SRCS+=$(ITE)/ite-dt.c
SRCS+=$(ITE)/ite-ram.c
SRCS+=lsensor.c

TARGET=lsensor

.PHONY: all
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGET)
