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
#define FRAME_RATE 120
#define MIN_FREQ -50
#define MAX_FREQ 50
#define WIDTH  1200
#define HEIGHT 900
#define STORAGE_THRESHOLD 1000
//#define SVG_SIGNATURE "[] Verified by Mistsu"
//#define DEBUG

#define dt  0.002 * FRAME_RATE / 120 * (MAX_FREQ - MIN_FREQ) / 300