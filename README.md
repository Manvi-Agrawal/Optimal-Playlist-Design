# Optimal-Playlist-Design
 A program designed to find optimal solution for playlist scheduling using genetic algorithm

# Run on a linux system with min gcc version of 4.2.0
## To check if OPENMP is installed, run
echo | cpp -fopenmp -dM | grep -i open

The command should display something like
#define _OPENMP 201511
201511 means : year 2015, month November(11) 

# Running Instructions
export OMP_NUM_THREADS=<omp_num_threads> //To specify the number of parallel threads for open mp
g++ -std=c++11 ga.cpp -fopenmp -o ga
./ga <population_size> 
