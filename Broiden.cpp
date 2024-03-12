#include <vector>
#include "libxl.h"
#include <iostream>
using namespace std;
using namespace libxl;
typedef double (*Function)(vector<double>);
typedef void (*Gradient)(vector<double>, vector<double>&);


class Broiden {
public:

	Book* book;
	Sheet* sheet;
	Format* format;
	int row = 4;

	vector<double> X = {0, 0};
	vector<double> X_new = { 0, 0 };
	vector<double> deltaX = { 0, 0 };

	Gradient grad;
	vector<double> gradX = { 0, 0 };
	vector<double> gradX_new = { 0, 0 };
	vector<double> deltaG = { 0, 0 };

	Function f;
	vector<vector <double>> N = { {1, 0}, {0, 1} };

	vector<double> temp = { 0, 0 };

	vector<vector <double>> divisor = { {1, 0}, {0, 1} };


	double eps = 0.01;

	Broiden(Book* book, Sheet* sheet, Format* format, vector<double> X, Function f, Gradient grad) {

		this->book = book;
		this->sheet = sheet;
		this->format = format;
		this->X = X;
		this->f = f;
		this->grad = grad;
		sheet->writeStr(row, 2, L"¹");
		sheet->writeStr(row, 3, L"x");
		sheet->writeStr(row, 4, L"f");
		sheet->writeStr(row, 5, L"grad");
		sheet->writeStr(row, 6, L"N");
		sheet->writeStr(row, 8, L"lambda");
		sheet->writeStr(row, 9, L"x_new");
		sheet->writeStr(row, 10, L"f_new");
		sheet->writeStr(row, 11, L"N_new");
		row++;
	}

	void method() {
		
		cout << X[0] << " " << X[1] << endl;
		int count = 0;
		int num = 1;
		do {
			sheet->writeNum(row, 2, num, format);
			num++;
			if (count > 2) {

				N = { {1, 0}, {0, 1} };
				count = 0;

			}

			sheet->writeNum(row, 3, X[0], format);
			sheet->writeNum(row + 1, 3, X[1], format);
			sheet->writeNum(row, 4, f(X), format);

			double lambda = searchLambda();
			grad(X, gradX);
			X_new[0] = X[0] - lambda * (N[0][0] * gradX[0] + N[0][1] * gradX[1]);
			X_new[1] = X[1] - lambda * (N[1][0] * gradX[0] + N[1][1] * gradX[1]);
			grad(X_new, gradX_new);

			sheet->writeNum(row, 5, gradX[0], format);
			sheet->writeNum(row + 1, 5, gradX[1], format);

			sheet->writeNum(row, 6, N[0][0], format);
			sheet->writeNum(row, 7, N[0][1], format);
			sheet->writeNum(row + 1, 6, N[1][0], format);
			sheet->writeNum(row + 1, 7, N[1][1], format);

			sheet->writeNum(row, 8, lambda, format);

			sheet->writeNum(row, 9, X_new[0], format);
			sheet->writeNum(row + 1, 9, X_new[1], format);

			sheet->writeNum(row, 10, f(X_new), format);

			deltaX[0] = X_new[0] - X[0];
			deltaX[1] = X_new[1] - X[1];
			deltaG[0] = gradX_new[0] - gradX[0];
			deltaG[1] = gradX_new[1] - gradX[1];

			temp[0] = deltaX[0] - (N[0][0] * deltaG[0] + N[0][1] * deltaG[1]);
			temp[1] = deltaX[1] - (N[1][0] * deltaG[0] + N[1][1] * deltaG[1]);

			double m = temp[0] * deltaG[0] + temp[1] * deltaG[1];
			
			N[0][0] += temp[0] * temp[0] / m;
			N[0][1] += temp[0] * temp[1] / m;
			N[1][0] += temp[1] * temp[0] / m;
			N[1][1] += temp[1] * temp[1] / m;

			sheet->writeNum(row, 11, N[0][0], format);
			sheet->writeNum(row, 12, N[0][1], format);
			sheet->writeNum(row + 1, 11, N[1][0], format);
			sheet->writeNum(row + 1, 12, N[1][1], format);

			X[0] = X_new[0];
			X[1] = X_new[1];
			cout << X[0] << " " << X[1] << endl;
			row += 3;
			count++;

		} while (sqrt(gradX_new[0] * gradX_new[0] + gradX_new[1] * gradX_new[1]) >
			eps);
	}

	double searchLambda() {
		double check = f(X);
		double lambda = 0;
		double delta = 0.0001;
		grad(X, gradX);
		while (delta > 0.0000000001) {
			if (f(vector<double>({ X[0] - (lambda + delta) * (N[0][0] * gradX[0] + N[0][1] * gradX[1]), 
				X[1] - (lambda + delta) * (N[1][0] * gradX[0] + N[1][1] * gradX[1])})) < check) {
				lambda += delta;
				check = f(vector<double>({ X[0] - lambda * (N[0][0] * gradX[0] + N[0][1] * gradX[1]),
										   X[1] - lambda * (N[1][0] * gradX[0] + N[1][1] * gradX[1]) }));
				grad(vector<double>({ X[0] - lambda * (N[0][0] * gradX[0] + N[0][1] * gradX[1]),
									  X[1] - lambda * (N[1][0] * gradX[0] + N[1][1] * gradX[1]) }), gradX);
			}
			else if (f(vector<double>({ X[0] - (lambda - delta) * (N[0][0] * gradX[0] + N[0][1] * gradX[1]),
				X[1] - (lambda - delta) * (N[1][0] * gradX[0] + N[1][1] * gradX[1]) })) < check) {
				lambda -= delta;
				check = f(vector<double>({ X[0] - lambda * (N[0][0] * gradX[0] + N[0][1] * gradX[1]),
										   X[1] - lambda * (N[1][0] * gradX[0] + N[1][1] * gradX[1]) }));
				grad(vector<double>({ X[0] - lambda * (N[0][0] * gradX[0] + N[0][1] * gradX[1]),
									  X[1] - lambda * (N[1][0] * gradX[0] + N[1][1] * gradX[1]) }), gradX);
			}
			else delta /= 10;
		}

		return lambda;
	}


};