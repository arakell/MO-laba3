#include <vector>
#include "libxl.h"
#include <iostream>
using namespace std;
using namespace libxl;
typedef double (*Function)(vector<double>);
typedef void (*Gradient)(vector<double>, vector<double>&);


class GradientMethod {
public:

	Book* book;
	Sheet* sheet;
	Format* format;
	vector<double> X;
	vector<double> gradX = {0, 0};
	Function f;
	Gradient grad;
	double eps = 0.01;
	int row = 4;


	GradientMethod(Book* book, Sheet* sheet,  Format* format, vector<double> X, Function f, Gradient grad) {
		this->book = book;
		this->sheet = sheet;
		this->format = format;
		this->X = X;
		this->f = f;
		this->grad = grad;
		sheet->writeStr(row, 3, L"x1");
		sheet->writeStr(row, 4, L"x2");
		sheet->writeStr(row, 5, L"f");
		sheet->writeStr(row, 6, L"lambda");
		sheet->writeStr(row, 7, L"grad1");
		sheet->writeStr(row, 8, L"grad2");
		sheet->writeStr(row, 9, L"x1_new");
		sheet->writeStr(row, 10, L"x2_new");
		sheet->writeStr(row, 11, L"f_new");
		row++;
	}

	void method() {
		grad(X, gradX);
		while (sqrt(pow(gradX[0], 2) + pow(gradX[1], 2)) > eps) {
			sheet->writeNum(row, 3, X[0], format);
			sheet->writeNum(row, 4, X[1], format);
			sheet->writeNum(row, 5, f(X), format);
			double lambda = searchLambda();
			sheet->writeNum(row, 6, lambda, format);
			sheet->writeNum(row, 7, gradX[0], format);
			sheet->writeNum(row, 8, gradX[1], format);
			X = { X[0] + gradX[0] * lambda, X[1] + gradX[1] * lambda};
			sheet->writeNum(row, 9, X[0], format);
			sheet->writeNum(row, 10, X[1], format);
			sheet->writeNum(row, 11, f(X), format);
			grad(X, gradX);
			cout << X[0] << " " << X[1] << endl;
			row++;
		
		}
	
	}

	double searchLambda() {
		double check = f(X);
		double lambda = 0;
		double delta = 0.0001;
		vector<double> temp = {X[0]+gradX[0]*(lambda+delta), X[1] + gradX[1] * (lambda + delta) };
		while (delta > 0.0000000001) {
		
			if (f(vector<double>({ X[0] + gradX[0] * (lambda + delta), X[1] + gradX[1] * (lambda + delta) })) < check) {
				lambda += delta;
				check = f(vector<double>({ X[0] + gradX[0] * lambda, X[1] + gradX[1] * lambda }));
			}
			else if (f(vector<double>({ X[0] + gradX[0] * (lambda - delta), X[1] + gradX[1] * (lambda - delta) })) < check) {
				lambda -= delta;
				check = f(vector<double>({ X[0] + gradX[0] * lambda, X[1] + gradX[1] * lambda }));
			}
			else delta /= 10;
		}

		return lambda;
	}


};