
#include "vector.h"

//Some vector helper functions

float dot_product(float *a, float *b, int size) {
	float dp = 0.0f;
	int i = 0;
	for (i = 0; i < size; i++)
		dp += a[i] * b[i];
	return dp;
}

float magnitude(float *a, int size) {
	int index = 0;
	float total = 0;
	for (index = 0; index < size; index++) {
		total = total + a[index] * a[index];
	}

	return sqrtf(total);
}
float angle_between(float *a, float *b, int size) {
	float dp = dot_product(a, b, size);
	float divisor = magnitude(a, size) * magnitude(b, size);
	float cos_inverse = dp / divisor;
	return acosf(cos_inverse);
}
float absolute_value(float value) {
	if (value < 0) {
		value = value * -1;
	}
	return value;
}
