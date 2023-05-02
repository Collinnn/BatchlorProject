IDIR = include
CC=g++
CFLAGS=-I$(IDIR) -Wall #-std=c++11 #-01 -02 -03 -

ODIR=obj
LIBS= -pthread

_DEPS = particle.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))


_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))



main: $(OBJ)
	$(CC) -o main $@ $^ $(CFLAGS) $(LIBS)

$(OBJ): $(ODIR)/%.o: %.c $(DEPS)
	$(CC) -cpp -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

