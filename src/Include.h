#pragma once
////////////////////////
// For every file
////////////////////////
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdio.h>
#include <random>
#include <string>
#include <string.h>
#include <ctype.h>
#include <map>
#include <math.h>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cassert>

////////////////////////
// Definition
////////////////////////
#define PI 3.14159265359
#define FRAME_RATE 144
#define MIN_FREQ -200
#define MAX_FREQ 200
#define WIDTH  1600
#define HEIGHT 800
#define STORAGE_THRESHOLD 20 * FRAME_RATE
//#define SVG_SIGNATURE "[] Verified by Mistsu"
// #define DEBUG

#define dt  0.02 * (MAX_FREQ - MIN_FREQ) / 300 / FRAME_RATE 