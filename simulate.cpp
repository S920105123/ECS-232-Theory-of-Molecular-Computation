#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <cassert>
using namespace std;

// random number generator
mt19937 rng(time(NULL));

enum {A, B, U};

int random_molecule(int na, int nb, int nu) {
	int n = na + nb + nu, r = rng() % n + 1;
	if (r <= na) {
		return A;
	}
	if (r <= na + nb) {
		return B;
	}
	return U;
}

// n is the total number of molecules in the initial condition
// (a, b, u) is the proportion of A, B, U in the initial condition, where 10 = 8 - a - b
// For example, (a, b, u) = (3, 5, 2)  means a : b : u = 3 : 5 : 2.
// n should be a multiple of 10.
// Return the time spent for simulation (in millisecond)
double simulate(int n, int a, int b) {
	int na = n / 10 * a, nb = n / 10 * b, nu = n / 10 * (10 - a - b), interaction = 0;
	while (na != n && nb != n) {
		interaction++;
		int x = random_molecule(na, nb, nu), y = random_molecule(na, nb, nu);
		if (x == A && y == B) {
			nb--;
			nu++;
		}
		else if (x == B && y == A) {
			na--;
			nu++;
		}
		else if ((x == U && y == A) || (x == A && y == U)) {
			na++;
			nu--;
		}
		else if ((x == U && y == B) || (x == B && y == U)) {
			nb++;
			nu--;
		}
	}
	return (double)interaction / n;
}

int main() {
	int a[4] = {1, 5, 6, 9};
	int b[4] = {0, 5, 4, 1};
	vector<int> population;
	
	int INIT = 100, END = 10000;
	for (int i = INIT; i <= END; i = i * 10) {
		for (int j = 1; j < 10; j++) {
			population.push_back(i * j);
		}
	}
	population.push_back(END * 10);
	
	
	for (int type = 0; type < 4; type++) {
		string name = string("result_") + to_string(a[type]) + string("_vs_") + to_string(b[type]) + string(".txt");
		ofstream fout(name);
		int round = 0, ITERATION = 50;
		double factor = 0.0;
		for (int n : population) {
			cout << "Doing type " << type << " size " << n << endl;
			double total_time = 0;
			for (int i = 0; i < ITERATION; i++) {
				total_time += simulate(n, a[type], b[type]);
			}
			double average = (double)total_time / ITERATION;
			factor += average / (log(n));
			round++;
			fout << n << ", " << fixed << setprecision(3) << average  << endl;
		}
		factor /= round;
		fout.close();
		
		name = string("approximate_") + to_string(a[type]) + string("_vs_") + to_string(b[type]) + string(".txt");
		fout.open(name);
		for (int n : population) {
			fout << n << ", " << factor * log(n)  << endl;
		}
		fout.close();
	}
	
	return 0;
}
