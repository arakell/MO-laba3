#include <vector>
#include "libxl.h"
#include <iostream>
using namespace std;
using namespace libxl;
typedef double (*Function)(vector<double>);
typedef void (*Gradient)(vector<double>, vector<double>&);

// H матрица Гессе
// Вектора сопряжены по H если S^T H S = 0 вне диагонали и >0 по диагонали 

class ConjugateGradMethod {
public:

	Book* book;
	Sheet* sheet;
	Format* format;
	vector<double> X;
	vector<double> gradX = { 0, 0 };
	Function f;
	Gradient grad;
	vector<double> S = {0, 0};
	double eps = 0.01;
	int row = 4;

	ConjugateGradMethod(Book* book, Sheet* sheet, Format* format, vector<double> X, Function f, Gradient grad){
		
		this->book = book;
		this->sheet = sheet;
		this->format = format;
		this->X = X;
		this->f = f;
		this->grad = grad;
		sheet->writeStr(row, 3, L"x1");
		sheet->writeStr(row, 4, L"x2");
		sheet->writeStr(row, 5, L"f");
		sheet->writeStr(row, 6, L"S1");
		sheet->writeStr(row, 7, L"S2");
		sheet->writeStr(row, 8, L"lambda");
		sheet->writeStr(row, 9, L"omega");
		sheet->writeStr(row, 10, L"S1_new");
		sheet->writeStr(row, 11, L"S2_new");
		sheet->writeStr(row, 12, L"x1_new");
		sheet->writeStr(row, 13, L"x2_new");
		sheet->writeStr(row, 14, L"f_new");
		row++;

	}

	void method() {
		grad(X, gradX);
		S[0] = -gradX[0];
		S[1] = -gradX[1];
		while (sqrt(pow(S[0],2) + pow(S[1], 2)) > eps) {
			sheet->writeNum(row, 3, X[0], format);
			sheet->writeNum(row, 4, X[1], format);
			sheet->writeNum(row, 5, f(X), format);
			sheet->writeNum(row, 6, S[0], format);
			sheet->writeNum(row, 7, S[1], format);
			double lambda = searchLambda();
			sheet->writeNum(row, 8, lambda, format);
			X[0] = X[0] + lambda * S[0];
			X[1] = X[1] + lambda * S[1];
			vector<double >gradX_new = {0, 0};
			grad(X, gradX_new);
			double omega = pow(sqrt(pow(gradX_new[0], 2) + pow(gradX_new[1], 2)), 2) / pow(pow(gradX[0], 2) + pow(gradX[1], 2), 2);
			sheet->writeNum(row, 9, omega, format);
			S[0] = -gradX_new[0] + S[0] * omega;
			S[1] = -gradX_new[1] + S[1] * omega;
			sheet->writeNum(row, 10, S[0], format);
			sheet->writeNum(row, 11, S[1], format);
			sheet->writeNum(row, 12, X[0], format);
			sheet->writeNum(row, 13, X[1], format);
			sheet->writeNum(row, 14, f(X), format);
			cout << X[0] << " " << X[1] << endl;
			row++;
		}

	
	}

	double searchLambda() {
		double check = f(X);
		double lambda = 0;
		double delta = 0.0001;
		vector<double> temp = { X[0] + S[0] * (lambda + delta), X[1] + S[1] * (lambda + delta) };
		while (delta > 0.0000000001) {

			if (f(vector<double>({ X[0] + S[0] * (lambda + delta), X[1] + S[1] * (lambda + delta) })) < check) {
				lambda += delta;
				check = f(vector<double>({ X[0] + S[0] * lambda, X[1] + S[1] * lambda }));
			}
			else if (f(vector<double>({ X[0] + S[0] * (lambda - delta), X[1] + S[1] * (lambda - delta) })) < check) {
				lambda -= delta;
				check = f(vector<double>({ X[0] + S[0] * lambda, X[1] + S[1] * lambda }));
			}
			else delta /= 10;
		}

		return lambda;
	}


};