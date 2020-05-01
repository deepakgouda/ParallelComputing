#include <iostream>
#include <vector>
#include <unordered_set>
#include <omp.h>

#define debug(x) cerr << #x << " = " << x << endl;
#define row 6
#define col 2

using namespace std;

int delta[row][col] =
	{{1, 5},
	 {2, 5},
	 {5, 3},
	 {5, 4},
	 {1, 5},
	 {5, 5}};

unordered_set<int> acceptStates;

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

vector<vector<int>> finalStates;

void *run(void *b)
{
	Block *block = (Block *)b;
	vector<int> f(row, 0);
	int indx, q, q0;
	for (int q0 = 0; q0 < row; q0++)
	{
		indx = 0, q = q0;
		while (indx < block->s.length())
		{
			q = delta[q][block->s[indx] - 48];
			indx++;
		}
		finalStates[block->pid].push_back(q);
	}
}

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
	long n = (long)atoi(argv[1]);
	int p = atoi(argv[2]);
	string s = getPatternString(n);
	double start, stop;

	finalStates.resize(p);
	acceptStates.insert(4);

	start = omp_get_wtime();
	Block **blockArr = new Block *[p];
	pthread_t threads[p];

	for (int i = 0; i < p; i++)
	{
		blockArr[i] = new Block(i, s.substr(i * n / p, n / p));
		pthread_create(&threads[i], NULL, run, (void *)blockArr[i]);
	}

	for (int i = 0; i < p; i++)
	{
		pthread_join(threads[i], NULL);
	}

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