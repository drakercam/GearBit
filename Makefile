CC = g++
CFLAGS = -Wall -Iinclude
LIBS = -lglfw -ldl -lGL -lm 
SRC = src/main.cpp src/glad.c
OUT = Engine

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

run:
	./$(OUT)

clean:
	rm -f $(OUT)
