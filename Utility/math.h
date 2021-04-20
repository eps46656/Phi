#ifndef PHI__define_guard__Utility__math_h
#define PHI__define_guard__Utility__math_h

#include "../define.h"

namespace phi {
namespace math {

double sqrt(double x) {
	if (x <= 0) { return 0; }

	double lower;
	double upper;

	if (x < 1) {
		lower = 0;
		upper = 1;
	} else if (1 < x) {
		lower = 1;
		upper = x;
	} else {
		return 1;
	}

	double lower_sq(lower * lower);
	double upper_sq(upper * upper);

	while (PHI__eps < upper - lower) {
		double mid((lower + upper) / 2);
		double mid_sq(mid * mid);

		if (mid_sq < x) {
			lower = mid;
			lower_sq = mid_sq;
		} else {
			upper = mid;
			upper_sq = mid_sq;
		}
	}

	return (lower + upper) / 2;
}

double exp(double x) {
	double r(1 + x);
	double a(x);

	for (double i(2); PHI__eps < a; ++i) { r += a = a * x / i; }

	return r;
}

double ln(double x) {
	if (x <= 0) { return 0; }

	double r(0);
	double a;

	if (1 < x) {
		x = 1 / x;
		a = 1;
	} else {
		a = -1;
	}

	x = 1 - x;

	for (double i(1); PHI__eps < a; ++i) { r += (a *= x) / i; }

	return r;
}

double atan(double x) {
	bool s(x < 0);
	if (s) { x = -x; }

	bool c(1 < x);
	if (c) { x = 1 / x; }

	double r(x);
	double d(x);
	double dd(-x * x);

	for (double i(3); PHI__eps < d; i += 2) { r += (d *= dd) / i; }

	if (c) { r = PHI__pi / 2 - r; }
	return s ? -r : r;
}

double atan(double x, double y) {
	if (x == 0) { return y < 0 ? PHI__pi * 3 / 2 : PHI__pi / 2; }

	double phase(atan(y / x));

	if (x < 0) { return PHI__pi + phase; }
	if (phase < 0) { return PHI__pi * 2 + phase; }
	return phase;
}

void atan(double& phase, double& r, double x, double y) {
	r = sqrt(x * x + y * y);
	x /= r;
	y /= r;
}

double power(double base, double exponent) { return exp(exponent * ln(base)); }

}
}

#endif