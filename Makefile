CC      := gcc
CFLAGS  := -I header -Wall -Wextra -O2
SRC     := $(wildcard src/*.c)
TARGET  := neogit

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC) $(wildcard header/*.h)
	$(CC) $(CFLAGS) $(SRC) -o $@

clean:
	rm -f $(TARGET)
