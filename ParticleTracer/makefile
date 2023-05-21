IDIR = include
CC=g++
CFLAGS=-I$(IDIR) -Wall #-std=c++11 #-01 -02 -03 -

ODIR=obj
LIBS= 

_DEPS = particle.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))


_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -o $@ -c $< 

main:  obj/main.o
#$(info $(CC) -o main obj/main.o)
	$(CC) -o main obj/main.o 
#$(CFLAGS) $(LIBS)


#$(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

