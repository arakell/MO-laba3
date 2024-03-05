#include "Functions.h"

double f1(vector<double> x) { 
    return 10 * (x[0] + x[1] - 10) * (x[0] + x[1] - 10) + (x[0] - x[1] + 4) * (x[0] - x[1] + 4);
}

void gradf1(vector<double> x, vector<double>& grad) {

    grad[0] = 22 * x[0] + 18 * x[1] - 192;
    grad[1] = 18 * x[0] + 22 * x[1] - 208;

}

double f2(vector<double> x) {
    return 100 * (x[1] - x[0] * x[0]) * (x[1] - x[0] * x[0]) + (1 - x[0]) * (1 - x[0]);
}

void gradf2(vector<double> x, vector<double>& grad) {

    grad[0] = -400 * x[0] * (-x[0] * x[0] + x[1]) + 2 * x[0] - 2;
    grad[1] = -200 * x[0] * x[0] + 200 * x[1];

}