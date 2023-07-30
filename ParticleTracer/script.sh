#!/bin/sh 
### General options 
### -- specify queue -- 
#BSUB -q hpc
### -- set the job Name -- 
#BSUB -J My_Application
### -- ask for number of cores (default: 1) -- 
#BSUB -n 16 
### -- specify that the cores must be on the same host -- 
#BSUB -R "span[hosts=1]"
### -- specify that we need 4GB of memory per core/slot -- 
#BSUB -R "rusage[mem=12GB]"

#BSUB -R "select[model==XeonGold6226R]"
### -- set walltime limit: hh:mm -- 
#BSUB -W 3:00 
### -- set the email address -- 
# please uncomment the following line and put in your e-mail address,
# if you want to receive e-mail notifications on a non-default address
### -- Specify the output and error file. %J is the job-id -- 
### -- -o and -e mean append, -oo and -eo mean overwrite -- 
#BSUB -o Output/Output_%J.out 
#BSUB -e Output/Output_%J.err 

# here follow the commands you want to execute with input.in as the input file


CALL="./main 10000"

PERF_ARGS="stat -x, -e L1-dcache-loads,L1-dcache-load-misses,LLC-load-loads,LLC-load-misses"
export OMP_NUM_THREADS=1
perf $PERF_ARGS $CALL
export OMP_NUM_THREADS=2
perf $PERF_ARGS $CALL
export OMP_NUM_THREADS=4
perf $PERF_ARGS $CALL
export OMP_NUM_THREADS=8
perf $PERF_ARGS $CALL
export OMP_NUM_THREADS=16
perf $PERF_ARGS $CALL
export OMP_NUM_THREADS=32
perf $PERF_ARGS $CALL


