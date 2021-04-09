#pragma once
#include "Include.h"

using namespace sf;
using namespace std;

class Complex
{
public:
	double real;
	double imag;

	Complex(Vector2f point) {
		real = point.x;
		imag = point.y;
	}

	Complex(Vertex point) {
		real = point.position.x;
		imag = point.position.y;
	}

	Complex(double x, double y) {
		this->real = x;
		this->imag = y;
	}

	Complex() {}

	Vector2f vector() {
		return Vector2f(this->real, this->imag);
	}
};

Complex operator*(Complex& a, Complex& b) {
	Complex c;
	c.imag = b.real * a.imag + b.imag * a.real;
	c.real = a.real * b.real - a.imag * b.imag;
	return c;
}

Complex operator*(Complex const a, Complex const b) {
	Complex c;
	c.imag = b.real * a.imag + b.imag * a.real;
	c.real = a.real * b.real - a.imag * b.imag;
	return c;
}

Complex operator+(Complex& a, Complex& b) {
	return Complex(a.real + b.real, a.imag + b.imag);
}

Complex operator+(Complex const a, Complex const b) {
	return Complex(a.real + b.real, a.imag + b.imag);
}

void operator+=(Complex& a, Complex& b) {
	a.real += b.real;
	a.imag += b.imag;
}
void operator+=(Complex& a, Complex const b) {
	a.real += b.real;
	a.imag += b.imag;
}

void operator*=(Complex& a, Complex& b) {
	Complex c;
	c.imag = b.real * a.imag + b.imag * a.real;
	c.real = a.real * b.real - a.imag * b.imag;
	a = c;
}

Complex operator*(Complex& a, double b) {
	return Complex(a.real * b, a.imag * b);
}

Complex operator*(Complex const a, double b) {
	return Complex(a.real * b, a.imag * b);
}

Complex operator+(Complex& a, double b) {
	return Complex(a.real + b, a.imag);
}

Complex operator+(Complex const a, double b) {
	return Complex(a.real + b, a.imag);
}

void operator*=(Complex& a, double b) {
	a.real *= b;
	a.imag *= b;
}

void operator+=(Complex& a, double b) {
	a.real += b;
}

void print(Complex c)
{
	cout << c.real << "+" << c.imag << "i";
}

Complex VectorToComplex(Vector2f point) {
	return Complex(point);
}

Complex VertexToComplex(Vertex point) {
	return Complex(point);
}

Vector2f ComplexToVector(Complex number) {
	return number.vector();
}

Complex exp(Complex c)
{
	Complex d;
	d.real = cos(c.imag);
	d.imag = sin(c.imag);
	d *= exp(c.real);
	return d;
}