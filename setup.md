# Some general notes on working with GPU nodes

Login to the interactive GPU nodes using 
```bash
sxm2sh
```
This should put you in the same directory as before, but on a GPU node with access to at least one GPU.

Usually you can get access to more by exporting the environment variable `CUDA_VISIBLE_DEVICES=(1,2,3,4)` which in this case would select gpu 1, 2, 3 and 4. Can also select a subset.

To view the available nodes and to see the current usage you can use
```bash
nvidia-smi
```
If you notice exceptionally bad performance on a GPU, it might be because some else is using it. This you can see using the above command. From here you can change to a different GPU by exporting `CUDA_VISIBLE_DEVICES`

## Loading NVHPC
This should be enough to load the Nvidia compiler, both for CUDA development and OMP using either nvcc or nvc++
```bash
module load cuda/11.7
module load nvhpc/22.7-nompi
```

## Compiling OMP code
I'm assuming that you already have a bit of knowledge on how to compile with makefiles. But we can go over that next time, if you have questions. I can also recommend using cmake to generate the makefiles.

If you already have something that can compile CPU code with OpenMP, then is should be fairly straight forward to compile for GPUs using nvc++. The only difference I have is the following flags:
```bash
OMPFLAGS = -fopenmp -mp=gpu -mp=noautopar -gpu=fastmath

# will give more info on how stuff is mapped parallel regions
OMPFLAGS += -Minfo=mp 
```