#pragma once

#include "Arm.h"

class Arm
{
public:
	float phase;
	float freq;
	float ampl;

	Arm() {}

	Arm(float phase, float frequency, float amplitude) {
		this->phase = phase;
		this->freq  = frequency;
		this->ampl  = amplitude;
	}

	void set(float phase, float frequency, float amplitude) {
		this->phase = phase;
		this->freq  = frequency;
		this->ampl  = amplitude;
	}

	void setPhase    (float phase) { this->phase = phase; }
	void setFrequency(float freq)  { this->freq  = freq;  }
	void setAmplitude(float ampl)  { this->ampl  = ampl;  }
};


