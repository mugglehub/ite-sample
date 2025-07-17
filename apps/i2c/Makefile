CC=gcc
ITE=../../ite

CFLAGS=-I. -Wall
CFLAGS+=-I$(ITE)

SRCS=$(ITE)/ite.c
SRCS+=$(ITE)/acpi.c
SRCS+=$(ITE)/ite-mbx.c
SRCS+=$(ITE)/ite-dt.c
SRCS+=$(ITE)/ite-i2c.c
SRCS+=i2c.c

TARGET=i2c

.PHONY: all
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

.PHONY: clean
clean:
	rm -f $(TARGET)
