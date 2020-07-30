/*
Part of coursework for Soft Computing under the guidance of Dr. Shampa Chakraverty 

Run on a ubuntu system with min gcc version of 4.2.0
To check if OPENMP is installed, run
echo | cpp -fopenmp -dM | grep -i open

The command should display something like
#define _OPENMP 201511
201511 means : year 2015, month November(11) 

To specify the number of parallel threads for open mp
export OMP_NUM_THREADS=<omp_num_threads> 

Compile the code
g++ -std=c++11 ga.cpp -fopenmp -o ga

Run the Code
./ga <population_size> 


A program designed to find optimal solution for playlidst scheduling using genetic algorithm
The following are given about the system:
a) The duration of songs
b) Suitability[i][j] representing the suitability of activity j given song i is being played
given learner has completed course i
The following objectives needs to be fulfilled :
a) The total value needs to be maximized.
b) The number of switchovers between activities must be minimized.
c) The number of distinct activities must be maximized

The following is/are the constraints :
a) Switchover Time is 10 minutes
*/
#include <omp.h>
#include <bits/stdc++.h>
#include<random>
#define INF 1000000000

using namespace std;

struct gene
{
	int song_num;
	int activity_num;
};

struct trapezoid
{
	float break_in, rest_point;
	float val;

	trapezoid(float _break_in, float _rest_point, float _val)
	{
		break_in = _break_in;
		rest_point = _rest_point;
		val = _val; 
	}
};


class GeneticAlgorithm{

public:
	GeneticAlgorithm(int _population_size){ population_size = _population_size;}
	void run();

private:
	vector< vector<gene> > population;
	vector<int> duration;
	vector< vector<float> > suitability;
	vector<trapezoid> functions;
	 
	float switchover_time = 10;
	int num_songs, num_activities, population_size, num_gen;
	
		
	float w_val = 0.4, w_switch_over = 0.3, w_num_activities = 0.3;
	float mu, delta;
	
	void input_parameters();/*To take input from the user */
	void output_parameters();/*To display generated values to the user */
	void generate_initial_population();
	void crossover();
	void mutation();
	void selection();
	void print_population();

	float value_activity(int activity_num, float duration_activity);
	float calculate_fitness(vector<gene> &chromosome);
	bool compare_fitness(vector<gene> &a, vector<gene> &b);	
};

//Driver Code
int main(int argc, char* argv[])
{
	if(argc!=2)
	{
		cout << "Program requires population_size as a cmd line argument\n";
	}
	else
	{
		int population_size=stoi(argv[1]);
		GeneticAlgorithm ga = GeneticAlgorithm(population_size);
		ga.run();
	}
return 0;
}

void GeneticAlgorithm :: input_parameters()
{

	//Input number of generations
	cout << "Enter the number of generations : ";
	cin >> num_gen;
	//Input number of song in playlist
	cout << "Enter the number of songs in graph : ";
	cin >> num_songs;
	//Generate duration of each song
	duration = vector<int>(num_songs);
	for(int i=0; i<num_songs; i++)
		duration[i] = rand()%51 ;
	cout << "Duration of each song(in min) generated...\n ";
	//Input number of song in playlist
	cout << "Enter the number of activities : ";
	cin >> num_activities;
	//Generate Suitability Matrix
	suitability = vector< vector<float> >(num_songs, vector<float>(num_activities));
	for(int i=0; i<num_songs; i++)
		for(int j=0; j<num_activities; j++)
			suitability[i][j] = rand()/double(RAND_MAX);
	cout << "Suitability matrix of songs is generated...\n ";
	//Generate Function Parameters
	for(int i=0; i<num_activities; i++)
	{
		float break_in, rest_point, val;
		break_in = (rand() % 101) + (rand()/(double) RAND_MAX) ;
		rest_point = (rand() % 201) + (rand()/(double) RAND_MAX) ;
		val = break_in = (rand() % 10) + (rand()/(double) RAND_MAX) ;
		functions.push_back({break_in, rest_point, val});
	}
	cout << "Function parameters of each activity i.e. (break_in, rest_point, val) are generated...\n ";

	// Set size of population matrix
	population = vector< vector<gene> >(population_size, vector<gene>(num_songs));
}

void GeneticAlgorithm :: output_parameters()
{
	cout << "Displaying Generated values : \n" ;
	//Output generated duration of each song
	cout << "\n" << "Duration of each song(in min) : \n ";
	for(int i=0; i<num_songs; i++)
		cout << duration[i] << " ";
	
	//Output generated Suitability Matrix
	cout << "\n" << "Suitability matrix of songs : \n ";
	for(int i=0; i<num_songs; i++)
	{
		for(int j=0; j<num_activities; j++)
			cout << suitability[i][j] << " ";
		cout << "\n";
	}
	
	//Output Generated Function Parameters
	cout << "\n"<<  "Function parameters of each activity i.e. (break_in, rest_point, val) : \n ";
	for(int i=0; i<num_activities; i++)
	{
		float break_in = functions[i].break_in;
		float rest_point = functions[i].rest_point;
		float val = functions[i].val;
		cout << "{ " << break_in << "," << rest_point << "," << val << "}\n";
	}
}

void GeneticAlgorithm :: generate_initial_population()
{
	#pragma omp parallel for
	for(int i=0; i<population_size; i++)
	{
		for(int j=0; j<num_activities; j++)
		{
			/*rand() generates a pseudo-random number between 0 and RAND_MAX*/
			population[i][j].song_num = rand()%num_songs ;
			population[i][j].activity_num = rand()%num_activities;
		}
	}
}

float GeneticAlgorithm :: value_activity(int activity_num, float duration_activity)
{	
	if( duration_activity <0)
		return 0;
	else
	{	
		if(duration_activity <= functions[activity_num].break_in)
			return duration_activity * functions[activity_num].val ;
		else
		{
			int time_left = functions[activity_num].rest_point-duration_activity;
			int falling_time = functions[activity_num].rest_point - functions[activity_num].break_in; 
			/*Falling time is the time required by a membership function to reach zero from the constant value*/

			if(time_left<0)
				return 0;

			float val_end = (time_left/falling_time)*val_end;
			return (functions[activity_num].val * functions[activity_num].break_in +
			 0.5 * (functions[activity_num].val*falling_time - val_end*time_left) );
		}
	}	

}

float GeneticAlgorithm :: calculate_fitness(vector<gene> &chromosome)
{
	float value_gained = 0 ;
	int num_switchover = -1 ; //Because last activity will register a switchover
	set<int> distinct_activities;
	float duration_activity;
	
	int i=0;
	while(i < chromosome.size())
	{
		distinct_activities.insert(chromosome[i].activity_num);
		duration_activity += (duration[chromosome[i].song_num] - switchover_time);//Not complete logic
		while( i < chromosome.size()-1 && chromosome[i].activity_num == chromosome[i+1].activity_num)
		{
			duration_activity += duration[chromosome[i++].song_num] ;
		}
		num_switchover++ ;
		value_gained += suitability[chromosome[i].song_num][chromosome[i].activity_num] * value_activity(chromosome[i++].activity_num, duration_activity) ;
		
		if(duration_activity>0)
		{
			duration_activity = 0 ;
		}
	}

	int num_distinct_activities = distinct_activities.size();

	float fitness_score = w_val*value_gained + w_num_activities*num_distinct_activities - w_switch_over*num_switchover;
	return fitness_score;
}

bool GeneticAlgorithm :: compare_fitness(vector<gene> &a, vector<gene> &b)
{
	return calculate_fitness(a) > calculate_fitness(b);
}

void GeneticAlgorithm :: selection()
{
	using namespace std::placeholders;
  	//std::sort(arr, arr+someSize, std::bind(&MyClass::doCompare, this, _1, _2)
	sort(population.begin(),population.end(),bind(&GeneticAlgorithm::compare_fitness, this, _1, _2));
}

void GeneticAlgorithm :: crossover()
{
	int num_crossover_pt = delta*num_songs;
	#pragma omp parallel for
	for(int i=0; i<population_size/2; i++)
	{
		set<int> crossover_points = set<int>();
		while ( crossover_points.size() < num_crossover_pt )
			crossover_points.insert( rand()%num_songs );

		for(int j=0; j<crossover_points.size(); j++)
			swap(population[i][j],population[population_size-i][j]);
	}
}

void GeneticAlgorithm :: mutation()/*single point mutation*/
{
	int num_mutation = mu*population_size;
	set<int> mutation_points = set<int>();
	while ( mutation_points.size() < num_mutation )
		mutation_points.insert( rand()%population_size );

	/*#pragma omp parallel for*/
	for(auto it=mutation_points.begin(); it!=mutation_points.end(); it++)
	{
		int i = *it;
		int j =  rand()%num_songs ;

		population[i][j].song_num = rand()%num_songs;
		population[i][j].activity_num = rand()%num_activities;
	}
}

void GeneticAlgorithm :: print_population()
{
	cout<< "\nPopulation matrix :\n";
	float value_gained = 0 ;
	int num_switchover = -1 ; //Because last activity will register a switchover
	set<int> distinct_activities;
	float duration_activity;
	
	for (int i = 0; i < population.size(); i++)
	{
		cout << "{ " ;
		int j=0;
		while(j < population_size)
		{
			distinct_activities.insert(population[i][j].activity_num);
			duration_activity += (duration[population[i][j].song_num] - switchover_time);//Not complete logic
			while( j < population[i].size()-1 && population[i][j].activity_num == population[i][j+1].activity_num)
			{
				cout << "(" << population[i][j].song_num << " , " ;
				cout << population[i][j].activity_num << " ) , " ;
				duration_activity += duration[population[i][j++].song_num] ;
			}
			cout << "(" << population[i][j].song_num << " , " ;
			cout << population[i][j].activity_num << " ) , " ;

			num_switchover++ ;
			value_gained += suitability[population[i][j].song_num][population[i][j].activity_num] * 
				value_activity(population[i][j++].activity_num, duration_activity) ;
			
			if(duration_activity>0)
			{
				duration_activity = 0 ;
			}
		}
		int num_distinct_activities = distinct_activities.size();
		cout <<"}, {value_gained : " << value_gained << "}, ";
		cout << "{ num_switchover : " << num_switchover << "}, ";
		cout << "{ num_distinct_activities : " << num_distinct_activities << "} \n";
	}
}

void GeneticAlgorithm :: run()
{
	input_parameters();
	output_parameters();
	generate_initial_population();
	cout << "ini pop gen...\n";
	for(int gen=0; gen<num_gen; gen++)
	{
		selection();
		crossover();
		mutation();
		print_population();
		cout << "Gen : " << gen+1 << "completed\n";
	}
}

