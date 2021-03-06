#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>

#include "cir2.hpp"
#include "heston.hpp"
#include "monte_carlo.hpp"

void plot_mean_var_cir2(int nn, double x, double T, double k, double a, double sigma, string c);




using namespace std;

void plot_paths_cir2(int n, int nn, double x, double T, double k, double a, double sigma, bool normal, string c);

int plot_paths_cir2_heston(int n, int nn, double x, double T, double k, double a, string c, string d);

void plot_average(int m, int N, double x, double T, double k, double a, double sigma, bool normal, string c);

void plot_paths_exact(int n, int nn, double x, double T, double k, double a, double sigma, bool normal, string c);

void plot_order(int m, int N, double x, double T, double k, double a, double sigma, bool normal, string c);

void plot_paper(int m, int N, double x, double T, double k, double a, double sigma, bool normal, string c);
