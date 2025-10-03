CC = clang
override CFLAGS += -g -Wno-everything -pthread -lm -lpanel -lmenu -lncurses
OBJFILES = main.o Connect4.o
TARGET = Connect4Game

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) 

clean:
	rm -f $(OBJFILES) $(TARGET) *~