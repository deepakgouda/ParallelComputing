/**
 * g++ -fopenmp fsm.cpp && ./a.out <|w|>
 */

#include <iostream>
#include <vector>
#include <unordered_set>
#include <omp.h>

#define debug(x) cerr<<#x<<" = "<<x<<endl;
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

// Generate string of pattern (0011)* of length n
string getPatternString(long n)
{
	string s = "";
	if(n%4)
		return s;
	for (long i = 0; i < n / 4; i++)
	{
		s+="0011";
	}
	return s;
}

int main(int argc, char *argv[])
{
	// Accept length of string from system arguments
	long n = (long)atoi(argv[1]);

	// Generate string of required pattern
	string s = getPatternString(n);
	double start, stop;

	// Mark q4 as accept state
	acceptStates.insert(4);

	start = omp_get_wtime();

	if (run(s))
		std::cout<<"Acccepted"<<endl;
	else
		std::cout<<"Rejected"<<endl;
	stop = omp_get_wtime();
	std::cout<<stop - start<<" seconds"<<endl;
	return 0;
}