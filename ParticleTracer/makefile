IDIR = include
CC=nvc++
# -03 to -msse4 is optimization
# March native is to compile to the native architecture of the pc
# -fopenmp = openMP     -mp=gpu NVIDIA           -gpu=fastmath aproxoperations       -mavx&mavx2&msse4 vectorisation 
CFLAGS= -I $(IDIR) -Wall -std=c++17 -Msafeptr -march=native -fopenmp -mp=gpu -mp=noautopar -gpu=fastmath -O3 -mavx -mavx2 -msse4 -gpu=cc70 #-01 -02 -03 -gpu=cc80
OUT=Output


ODIR=obj
LIBS= -lm

_DEPS = particleTypes.hpp particles.hpp bruteforce.hpp util.hpp io.hpp util.hpp poissonSolver.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o bruteforce.o particles.o io.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -o  $@ -c $< $(CFLAGS)



# $@ is the left side of the " : " 
# $^ is the right side of the " : "

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)



.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -f $(OUT)/*.pmm $(OUT)/*.csv
	rm -f $(OUT)/*.pmm $(OUT)/*.ppm

