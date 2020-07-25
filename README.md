# Optimal-Playlist-Design
A program designed to find optimal solution for playlist scheduling using genetic algorithm. <br>
Part of coursework for Soft Computing under the guidance of Dr. Shampa Chakraverty. <br>
The following are given about the system : <br>
a) The duration of songs <br>
b) Suitability[i][j] representing the suitability of activity j given song i is being played <br>

## Objectives
The following objectives needs to be fulfilled :
a) The total value needs to be maximized.
b) The number of switchovers between activities must be minimized.
c) The number of distinct activities must be maximized

## Constraints
The following is/are the constraints :
a) Switchover Time is 10 minutes

# Run on a linux system with min gcc version of 4.2.0
## To check if OPENMP is installed, run
echo | cpp -fopenmp -dM | grep -i open

The command should display something like : <br>
#define _OPENMP 201511 <br>
201511 means : year 2015, month November(11) 

# Running Instructions
<b> To specify the number of parallel threads for open mp </b> <br>
export OMP_NUM_THREADS=<omp_num_threads> <br>
<b> Compile the code </b> <br>
g++ -std=c++11 ga.cpp -fopenmp -o ga <br>
<b> Run the code </b> <br>
./ga <population_size> <br>
