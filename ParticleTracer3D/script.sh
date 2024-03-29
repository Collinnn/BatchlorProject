#!/bin/sh 
### General options 
### -- specify queue -- 
#BSUB -q hpc
### -- set the job Name -- 
#BSUB -J My_Application
### -- ask for number of cores (default: 1) -- 
#BSUB -n 4 
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
./main 100
./main 1000
./main 5000
./main 10000
./main 20000
./main 30000
./main 40000
./main 50000

