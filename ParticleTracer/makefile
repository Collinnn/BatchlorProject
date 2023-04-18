IDIR = include
CC=g++
CFLAGS=-I$(IDIR) -Wall  #-01 -02 -03 -

ODIR=obj
LDIR =lib

LIBS=-L/usr/lib 

_DEPS = particle.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


main: $(OBJ)
	$(CC) -o  main $^ $(CFLAGS) $(LIBS)
	

$(OBJ): $(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 