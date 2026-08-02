CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -lm

SRC = src/utils.c src/auth.c src/account.c src/transaction.c \
      src/loan.c src/interest.c src/file_io.c src/display.c \
      src/admin.c src/main.c

OBJ = $(SRC:.c=.o)
TARGET = bank

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
