CC=g++
LIBS= -lglut -lGL -lGLU
INCLUDES=  
CCFLAGS= -O0 -g -gstabs -I. -DLINUX
SOURCES=
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=Sudoku

# --- targets
all:  $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): Sudoku.cpp $(OBJECTS)
	$(CC) Sudoku.cpp $(OBJECTS) -o $(EXECUTABLE) $(CCFLAGS) $(INCLUDES)  $(LIBS) 

.cc.o:
	$(CC) $(CCFLAGS) $(INCLUDES) $< -c

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
