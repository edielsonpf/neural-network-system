CC=gcc 
CFLAGS=-pg -c -Wall
LDFLAGS= -pg -lm -lfann

SOURCES= main.c FileConfigCtrl.c NeuralNetworkTrain.c

OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE=../Debug/neural_train

all: $(SOURCES) $(EXECUTABLE)
	rm -rf *.o
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE)
	

