/**
 * g++ -pthread -fopenmp fsmParallel.cpp && ./a.out <|w|> <numProcesses>
 */

#include <iostream>
#include <vector>
#include <unordered_set>
#include <omp.h>

#define debug(x) cerr << #x << " = " << x << endl;
#define row 6
#define col 2

using namespace std;

// Transition table of the DFA
int delta[row][col] =
	{{1, 5},
	 {2, 5},
	 {5, 3},
	 {5, 4},
	 {1, 5},
	 {5, 5}};

// Set of accept states
unordered_set<int> acceptStates;

// finalStates[i][j] stores the final state achieved 
// by process i after starting from state j
vector<vector<int>> finalStates;

// Block stores the process id and substring to be 
// processed by the process
typedef struct Block
{
	int pid;
	string s;
	Block(int pid, string s)
	{
		this->pid = pid;
		this->s = s;
	}
} Block;

// Processes the substring by taking initial state as q0, q1, ... qk-1
// and stores results in finalStates vector
void *run(void *b)
{
	Block *block = (Block *)b;
	vector<int> f(row, 0);
	int indx, q, q0;

	// Take initial state as q0, q1, ... qk-1
	for (int q0 = 0; q0 < row; q0++)
	{
		indx = 0, q = q0;
		while (indx < block->s.length())
		{
			// Perform the transition of state
			q = delta[q][block->s[indx] - 48];
			indx++;
		}
		finalStates[block->pid].push_back(q);
	}
	return nullptr;
}

// Generate string of pattern (0011)* of length n
string getPatternString(long n)
{
	string s = "";
	if (n % 4)
		return s;
	for (long i = 0; i < n / 4; i++)
	{
		s += "0011";
	}
	return s;
}

int main(int argc, char *argv[])
{
	// Accept length of string and number of processes from system arguments
	long n = (long)atoi(argv[1]);
	int p = atoi(argv[2]);

	// Generate string of required pattern
	string s = getPatternString(n);

	double start, stop;
	finalStates.resize(p);

	// Mark q4 as accept state
	acceptStates.insert(4);

	start = omp_get_wtime();
	Block **blockArr = new Block *[p];
	pthread_t threads[p];

	for (int i = 0; i < p; i++)
	{
		// Create block with process id and substring
		blockArr[i] = new Block(i, s.substr(i * (n / p), n / p));
		
		// Create thread and call function run with the block as parameter
		pthread_create(&threads[i], NULL, run, (void *)blockArr[i]);
	}

	for (int i = 0; i < p; i++)
	{
		pthread_join(threads[i], NULL);
	}

	// Iterate over final states of each segment to get final state, starting from q0
	int q = finalStates[0][0];
	for (int i = 1; i < p; i++)
	{
		q = finalStates[i][q];
	}

	if (acceptStates.find(q) != acceptStates.end())
		cout << "Acccepted" << endl;
	else
		cout << "Rejected" << endl;
	stop = omp_get_wtime();
	cout << stop - start << " seconds" << endl;
	return 0;
}