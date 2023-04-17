IDIR = include
CC=gcc
CFLAGS=-I$(IDIR) -Wall -fopenmp #-01 -02 -03 -

ODIR=obj
LDIR =lib

LIBS=-L/usr/lib #-pthread

#_DEPS =
#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = test.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


test: $(OBJ)
	$(CC) -o  test $^ $(CFLAGS) $(LIBS)
	

$(OBJ): $(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 