CC=gcc
ITE=../../ite

CFLAGS=-I. -Wall
CFLAGS+=-I$(ITE)

SRCS=$(ITE)/ite.c
SRCS+=$(ITE)/acpi.c
SRCS+=$(ITE)/ite-mbx.c
SRCS+=$(ITE)/ite-dt.c
SRCS+=info.c

TARGET=info

.PHONY: all
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

.PHONY: clean
clean:
	rm -f $(TARGET)
