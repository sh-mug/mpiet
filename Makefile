CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -Ofast -lpng -lgflags
TARGET = mpiet
SOURCE = main.cpp

.PHONY: clean sample

$(TARGET): $(SOURCE)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(RM) $(TARGET)

sample: $(TARGET)
	./$(TARGET) -f png/ack.png -c 10
