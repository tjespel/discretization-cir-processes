#include "plots.hpp"

//<<<<<<< HEAD:src/Plots.h
//void PlotPathsCIR2(int n, int nn, double x, double T, double k, double a, double sigma) { //This produces n paths from the CIR2 with nn nodes and returns a csv file that contains the paths (each column is a path)
//	vector <vector<double>> M;
//	for (int j = 0; j < n; j++) { M.push_back(CIR2(nn, x, T, k, a, sigma)); }
//=======
#include "plots.hpp"
#include "exact_values.hpp"

void plot_mean_var_cir2(int nn, double x, double T, double k, double a, double sigma, string c) { //nn are the number of nodes

	ofstream fout(c);
	if (!fout.is_open()) {
		cout << "Error opening the file output.txt for writing." << endl;
		//return -1;
	}
	for (int j = 0; j <= nn; j++) {
		double mean = cir_mean(x, (T*j)/nn, k, a, sigma);
		double std = sqrt(cir_variance(x, (T*j)/nn, k, a, sigma));
		double u = mean + std;
		double l = mean - std;
		fout << setprecision(5) << l << "," << mean << "," << u << endl;
	}
	fout.close();
}

void plot_paths_cir2(int n, int nn, double x, double T, double k, double a, double sigma, bool normal, string c) {
	vector <vector<double> > M;
	for (int j = 0; j < n; j++) { M.push_back(cir2(nn, x, T, k, a, sigma, normal)); }

	ofstream fout(c);
	if (!fout.is_open()) {
		cout << "Error opening the file output.txt for writing." << endl;
		//return -1;
	}
	for (int j = 0; j <= nn; j++) {
		for (int i = 0; i < n - 1;i++) {
			fout << setprecision(5) << M[i][j] << ",";
		}
		fout << setprecision(5) << M[n - 1][j] << endl;
	}
	fout.close();
}




int plot_paths_cir2_heston(int n, int nn, double x, double T, double k, double a, string c, string d) {

	vector <vector<double> > result_vector;
	vector <vector<double> > volatility_vector;


	// Add the random generation parameters

	std::default_random_engine generator;
	std::normal_distribution<double> normal(0.0,1.0);

	vector <vector<double> > Z1;
	vector <vector<double> > Z2;


	for (int j=0; j<n; j++){
		vector<double> Z1temp;
		vector<double> Z2temp;
		for (int i=0; i<nn; i++){
			Z1temp.push_back(normal(generator));
			Z2temp.push_back(normal(generator));
		}
		Z1.push_back(Z1temp);
		Z2.push_back(Z2temp);
	}

	double rau = .7683;
	for (int j=0; j<n; j++){
		for (int i=0; i<Z2.size(); i++){
			Z2[j][i] = rau * rau * Z2[j][i] + std::sqrt(1 - rau * rau ) * Z1[j][i];
		}
	}

	double sigma = 0.3484;

	std::clock_t start = std::clock();
	for (int i = 0; i < n; i++) {
		volatility_vector.push_back(cir2_heston(nn, 0.07728, T, k, .092, sigma, Z1[i]));
	}

	for (int i = 0; i < n; i++) {
		result_vector.push_back(heston(a, volatility_vector[i], x, n, Z2[i]));
	}
	std::clock_t end = std::clock();
	cout << "Paper version: " << (end-start) / (double) CLOCKS_PER_SEC << endl;

	ofstream fout(c);
	if (!fout.is_open()) {
		cout << "Error opening the file output.txt for writing." << endl;
		//return -1;
	}
	for (int j = 0; j <= nn; j++) {
		for (int i = 0; i < n - 1;i++) {
			fout << setprecision(5) << result_vector[i][j] << ",";
		}
		fout << setprecision(5) << result_vector[n - 1][j] << endl;
	}
	fout.close();

	volatility_vector.clear();
	result_vector.clear();

	start = std::clock();
	for (int i = 0; i < n; i++) {
		volatility_vector.push_back(exact_heston(nn, 0.07728, T, k, .092, sigma, Z1[i]));
	}

	for (int i = 0; i < n; i++) {
		result_vector.push_back(heston(a, volatility_vector[i], x, n, Z2[i]));
	}
	end = std::clock();
	cout << "Exact version: " << (end-start) / (double) CLOCKS_PER_SEC << endl;

	ofstream fout2(d);
	if (!fout2.is_open()) {
		cout << "Error opening the file output.txt for writing." << endl;
		//return -1;
	}
	for (int j = 0; j <= nn; j++) {
		for (int i = 0; i < n - 1;i++) {
			fout2 << setprecision(5) << result_vector[i][j] << ",";
		}
		fout2 << setprecision(5) << result_vector[n - 1][j] << endl;
	}
	fout2.close();

}



void plot_paths_exact(int n, int nn, double x, double T, double k, double a, double sigma, bool normal, string c) {
	vector <vector<double> > M;
	for (int j = 0; j < n; j++) { M.push_back(exact(nn, x, T, k, a, sigma)); }

	ofstream fout(c);
	if (!fout.is_open()) {
		cout << "Error opening the file output.txt for writing." << endl;
		//return -1;
	}
	for (int j = 0; j <= nn; j++) {
		for (int i = 0; i < n - 1;i++) {
			fout << setprecision(5) << M[i][j] << ",";
		}
		fout << setprecision(5) << M[n - 1][j] << endl;
	}
	fout.close();
}


void plot_average(int m, int N, double x, double T, double k, double a, double sigma, bool normal, string c) { //For fixed m computes E and write them in a csv file running n from 1 to N
	vector<double> values_cir2;
	vector<double> values_exact;
	vector<double> values_b1;
	vector<double> values_b3;
	for (int j = 1; j <= N; j++) {
		values_cir2.push_back(Ecir2(m, j, x, T, k, a, sigma, normal));
		values_exact.push_back(Eexact(m, j, x, T, k, a, sigma));
		values_b1.push_back(Eb1(m, j, x, T, k, a, sigma));
		values_b3.push_back(Eb3(m, j, x, T, k, a, sigma));
	}

	reverse(values_cir2.begin(), values_cir2.end());   //First the value with N nodes
	reverse(values_exact.begin(), values_exact.end());
	reverse(values_b1.begin(), values_b1.end());
	reverse(values_b3.begin(), values_b3.end());

	ofstream fout(c);
	if (!fout.is_open()) {
		cout << "Error opening the file output.txt for writing." << endl;
		//return -1;
	}
	for (int j = 0; j < values_cir2.size(); j++) {
		fout << setprecision(5) << values_cir2[j] << "," << values_exact[j] << "," << values_b1[j] << "," << values_b3[j] << endl;
	}
	fout.close();
}

void plot_order(int m, int N, double x, double T, double k, double a, double sigma, bool normal, string c) { //For fixed m computes -log(error) and write them in a csv file running n from 1 to N
	vector<double> values;
	for (int j = 100; j <= N; j+=100) {
		values.push_back(-log(error(m, j, x, T, k, a, sigma, normal)));
	}

	//reverse(values.begin(), values.end());

	ofstream fout(c);
	if (!fout.is_open()) {
		cout << "Error opening the file output.txt for writing." << endl;
		//return -1;
	}
	for (int j = 0; j < values.size(); j++) {
		fout << setprecision(5) << values[j] << endl;
	}
	fout.close();
}

void plot_paper(int m, int N, double x, double T, double k, double a, double sigma, bool normal, string c) { //For fixed m computes E and write them in a csv file running n from 1 to N
	vector<double> values_cir2;
	vector<double> values_exact;
	vector<double> values_K3;
	vector<double> values_K4;
	vector<double> values_b1;
	vector<double> values_b3;
	if (normal) {
		for (int j = 1; j <= N; j++) {
			cout << j << endl;
			vector<double> E = E_exp_paper(m, j, x, T, k, a, sigma, normal);
			values_cir2.push_back(E[0]);
			values_exact.push_back(E[1]);
			values_K3.push_back(E[0]);
			values_K4.push_back(E[0]);
			values_b1.push_back(E[2]);
			values_b3.push_back(E[3]);
		}
	}

	else {
		for (int j = 1; j <= N; j++) {
			cout << j << endl;
			vector<double> E = E_exp_paper(m, j, x, T, k, a, sigma, normal);
			values_cir2.push_back(E[0]);
			values_exact.push_back(E[1]);
			values_K3.push_back(E[2]);
			values_K4.push_back(E[3]);
			values_b1.push_back(E[4]);
			values_b3.push_back(E[5]);
		}
	}

	ofstream fout(c);
	if (!fout.is_open()) {
		cout << "Error opening the file output.txt for writing." << endl;
		//return -1;
	}
	for (int j = 0; j < values_cir2.size(); j++) {
		fout << setprecision(5) << values_cir2[j] << "," << values_exact[j] << "," << values_K3[j] << ","<< values_K4[j] << "," << values_b1[j] << "," << values_b3[j] << endl;
	}
	fout.close();
}
