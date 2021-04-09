#pragma once

#define DRAW
#include "Include.h"
#include "SVG.h"
#include "Arm.h"
#include "Complex.h"
#include "Fourier.h"

using namespace sf;

double t = 0;

//////////////////////////////////////////////////////////////
// Create arms which draw our image from data gathered
//////////////////////////////////////////////////////////////
void make_arms_from_fourier_coefficents(map<int, Complex> FourierCoefficent, vector<Arm>& arms, vector<Vertex>& vertices, vector<CircleShape>& circles)
{
	float phase;
	float ampl;
	float freq;

	vertices.push_back(Vertex(Vector2f(FourierCoefficent[0].real, FourierCoefficent[0].imag)));
	for (int i = MIN_FREQ; i <= MAX_FREQ; ++i) {
		if (i == 0) continue;

		freq  = i;
		ampl  = sqrt(FourierCoefficent[i].real * FourierCoefficent[i].real + FourierCoefficent[i].imag * FourierCoefficent[i].imag);
		phase = atan(abs(FourierCoefficent[i].imag) / abs(FourierCoefficent[i].real));

		if      (FourierCoefficent[i].real > 0 && FourierCoefficent[i].imag < 0) phase *= -1.0f;
		else if (FourierCoefficent[i].real < 0 && FourierCoefficent[i].imag > 0) phase = PI - phase;
		else if (FourierCoefficent[i].real < 0 && FourierCoefficent[i].imag < 0) phase = PI + phase;

		arms.push_back(Arm(phase, freq, ampl));

		///////////////////////////////
		// Vertices connect arm-points
		///////////////////////////////
		vertices.push_back(Vertex(Vector2f(), Color::Red));

		///////////////////////////////
		// Setup circle color, outline
		///////////////////////////////
		CircleShape newCircle;
		newCircle.setFillColor       (Color::Transparent);
		newCircle.setOutlineThickness(1);
		newCircle.setOutlineColor    (Color(255, 255, 255, 50));

		circles.push_back(newCircle);
	}

	std::sort(arms.begin(), arms.end(), [](Arm const& a, Arm const& b) {return a.ampl > b.ampl; });
}

void UpdatingVertices(vector<Vertex>& curves, vector<Vertex>& vertices, vector<CircleShape>& circles, vector<Arm>& arms)
{
	int no_arms = arms.size();

	for (int i = 1; i <= no_arms; i++) {
		vertices[i].position.x = vertices[i - 1].position.x + arms[i - 1].ampl * cos(t * 2 * PI * arms[i - 1].freq + arms[i - 1].phase);
		vertices[i].position.y = vertices[i - 1].position.y + arms[i - 1].ampl * sin(t * 2 * PI * arms[i - 1].freq + arms[i - 1].phase);

		if (i > 1) {
			circles[i - 2].setRadius(dist(vertices[i].position, vertices[i - 1].position));
			circles[i - 2].setPosition(vertices[i - 1].position);
			circles[i - 2].setOrigin(circles[i - 2].getRadius(), circles[i - 2].getRadius());
		}
	}
	curves.push_back(Vertex(Vector2f(vertices[no_arms].position.x, vertices[no_arms].position.y), Color::Cyan));

	if (curves.size() > STORAGE_THRESHOLD)
		curves.erase(curves.begin());

    t += dt;
}

void initDrawing(string filename, Polygon& coors, map<int, Complex>& fourier_coefficents, vector<Arm>& arms, vector<Vertex>& vertices, vector<CircleShape>& circles, float zoom, int offX, int offY)
{
	string  d_param   = get_d_param_from_path_tag_of_a_svg_file(filename);
	SVGPath svg_path  = from_string_to_SVGPath(d_param, zoom, offX, offY);
	coors             = convert_SVGPath_to_list_of_coordinates(svg_path);

	dft(coors, fourier_coefficents);
	make_arms_from_fourier_coefficents(fourier_coefficents, arms, vertices, circles);
}

void draw(string filename, float zoom, int offX, int offY, string drawMode) {
    vector<Vertex>      curves;                curves.clear();
	vector<Vertex>      vertices;			   vertices.clear();
	vector<Arm>         arms;				   arms.clear();
	map<int, Complex>   fourier_coefficents;   fourier_coefficents.clear();
	vector<CircleShape> circles;			   circles.clear();
	map <int, Text>     coor_tags, freq_tags;  coor_tags.clear(); freq_tags.clear();
	Polygon				coors;				   coors.clear();

	///////////////////////////////
	// Handle Data from SVG File
	///////////////////////////////
	// string filename = parseArgs(argc, argv);
	initDrawing(filename, coors, fourier_coefficents, arms, vertices, circles, zoom, offX, offY);
	UpdatingVertices(curves, vertices, circles, arms);

	//////////////////////////////
	// Setting up SFML Window
	//////////////////////////////
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(WIDTH, HEIGHT), filename, Style::Default, settings);
	window.setFramerateLimit(FRAME_RATE);

	Event event;
	
	while (window.isOpen())
	{
		while (window.pollEvent(event))
			if (event.type == Event::Closed)
				window.close();

		window.clear(Color::Black);

		/////////////////////////////////
		// Drawing stuffs here!!
		/////////////////////////////////
		UpdatingVertices(curves, vertices, circles, arms);

        // Draw figure
		if (!curves.empty()) 
            window.draw(&curves[0], curves.size(), LineStrip);

        // Draw arms
        if (drawMode != "FIGURE-ONLY")
		    window.draw(&vertices[0], vertices.size(), LinesStrip);

        // Draw actual figure
		// window.draw(&coors[0], coors.size(), LinesStrip);

        // Draw circles
        if (drawMode == "CIRCLE")
            for (int i = 0; i < circles.size(); ++i) {
                window.draw(circles[i]);
            }
		window.display();
	}
}