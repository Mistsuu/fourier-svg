#pragma once

#define FOURIER
#include "Include.h"
#include "SVG.h"
#include "Arm.h"
#include "Complex.h"

using namespace std;
using namespace sf;

void dft(Polygon fig, map<int, Complex>& coefficient)
{
	printf("[] Calculating coefficient for the Fourier Transform algorithm... Doing coefficent ");
	for (int co = MIN_FREQ; co <= MAX_FREQ; co++) {
		printf("%4d/%4d", co, MAX_FREQ);
		coefficient[co] = Complex(0, 0);
		int i = 0;
		for (double _t = 0, _dt = 1 / (fig.size() * 1.0f); i < fig.size(); _t += _dt, i++) {
			coefficient[co] += 
				VertexToComplex(fig[i]) * 
				exp(Complex(0, -2 * PI * _t * co)) * 
				_dt;
		}
		printf("\b\b\b\b\b\b\b\b\b");
	}
	printf("\b... Finished.\n");
}