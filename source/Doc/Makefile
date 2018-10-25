CC = g++
CFLAGS = -I. -O3 -std=c++11
TARGET = main
LIBS = -L/opt/software/epos/lib/ -lEposCmd

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) -o $(TARGET) $(TARGET).cpp $(CFLAGS) $(LIBS)

clean:
	$(RM) $(TARGET)
