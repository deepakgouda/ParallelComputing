/**
 * g++ -fopenmp fsm.cpp && ./a.out <|w|> <k> <|S|>
 * where, w = input word
 * 		  k = number of states
 * 		  S = set of alphabets
 */

#include <iostream>
#include <vector>
#include <unordered_set>
#include <omp.h>

#define debug(x) cerr<<#x<<" = "<<x<<endl;

using namespace std;

// Transition table of the DFA
/** Use this transition state table to recognise (0011)*
 * int delta[row][col] =
	{{1, 5},
	 {2, 5},
	 {5, 3},
	 {5, 4},
	 {1, 5},
	 {5, 5}};
*/

vector <vector <int> > delta;
int row, col;

// Set of accept states
unordered_set <int> acceptStates;

// Processes the string by taking initial state as q0
// and returns true if final state is in accpet states set
bool run(string s)
{
	int indx = 0, q0 = 0, q;
	q = q0;
	while(indx < s.length())
	{
		// Perform the transition of state
		q = delta[q][s[indx]-48];
		indx++;
	}
	if(acceptStates.find(q) != acceptStates.end())
		return true;
	else
		return false;
}

// Generate a random string of length n with alphabets
// taken from a set of size 'col'
string getPatternString(long n)
{
	string s = "";
	for (long i = 0; i < n; i++)
	{
		s+=to_string(rand()%col);
	}
	return s;
}

int main(int argc, char *argv[])
{
	// Accept length of string from system arguments
	long n = (long)atoi(argv[1]);
	row = atoi(argv[2]), col = atoi(argv[3]);

	srand(7);
	delta.resize(row);
	for (int i = 0; i < row; i++)
	{
		delta[i].resize(col);
		for (int j = 0; j < col; j++)
		{
			delta[i][j] = rand()%row;
		}
	}

	// Generate string of required pattern
	string s = getPatternString(n);

	double start, stop;

	// Mark q4 as accept state
	acceptStates.insert(4);

	start = omp_get_wtime();
	run(s);
	// if (run(s))
	// 	std::cout<<"Acccepted"<<endl;
	// else
	// 	std::cout<<"Rejected"<<endl;
	stop = omp_get_wtime();

	std::cout<<stop - start<<endl;
	return 0;
}