
GCC         = gcc
EXE	    = pthread_multiple_arguments
OBJ	    = $(EXE).o
SOURCE      = $(EXE).c

default: $(EXE)

$(OBJ): $(SOURCE)
	$(GCC) -c -o $@ $(SOURCE) -std=gnu99 -lrt -pthread

$(EXE): $(OBJ)
	$(GCC) $(OBJ) -o $(EXE) -std=gnu99 -lrt -pthread

clean:
	rm -rf *.o $(EXE)
