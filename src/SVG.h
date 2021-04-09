#pragma once

#include "Include.h"
#include "Calc.h"

using namespace std;
using namespace sf;

typedef long long int lli;

class SVGData
{
public:
	char             cmd;
	vector<Vector2f> points;

	SVGData() {}
	SVGData(char  cmd) { this->cmd = cmd; }
};

typedef vector<SVGData> SVGPath;
typedef vector<Vertex>  Polygon;
//////////////////////////////////////////////////////////////////////////////////////////////
// Opens an SVG file in <i_filename>
// ------------------------------------------------------------------------------
// Extract the d parameter in the <path> tag and the output is packed in a string
//////////////////////////////////////////////////////////////////////////////////////////////
string get_d_param_from_path_tag_of_a_svg_file(string svg_filename)
{
	cout << "[] Doing some d parameter extraction from file..." << endl;
	////////////////////////////////////////////////////////
	// Finding d parameter of the path tag in the SVG file
	////////////////////////////////////////////////////////
	fstream svgfile;
	svgfile.open(svg_filename, fstream::in);

	enum TASK { FIND_PATH_TAG, FIND_D_PARAM, FIND_ENDING_QUOTATION, JOINING_LINES };
	TASK CURRENT_TASK = FIND_PATH_TAG;

	vector<string> lines;
	int            d_pos, a_pos;
	string         line;

	while (getline(svgfile, line)) {
		switch (CURRENT_TASK) {
		case FIND_PATH_TAG:
			if (line.find("<path") == -1) break;
			CURRENT_TASK = FIND_D_PARAM;

		case FIND_D_PARAM:
			d_pos = line.find("d=\"");
			if (d_pos == -1)                          break; 
			if (d_pos != 0 && line[d_pos - 1] == 'i') break; // Really don't want to be confused with the id="" parameter

			line = line.substr(d_pos + 3);
			CURRENT_TASK = FIND_ENDING_QUOTATION;

		case FIND_ENDING_QUOTATION:
			a_pos = line.find("\"");
			if (a_pos == -1) {
				lines.push_back(line);
				break;
			}
			lines.push_back(line.substr(0, a_pos));
			CURRENT_TASK = JOINING_LINES;

		case JOINING_LINES:
			line = "";
			for (int i = 0; i < lines.size(); ++i)
				line += lines[i];
			goto FINISH_PROCESS;
		}
	}

	FINISH_PROCESS:
	svgfile.close();
#ifdef DEBUG
	cout << "[] D Param is: \"" << line << "\"" << endl;
#endif
	cout << "[] Finishing extracting d parameter from the <path> tag of the file! ..." << endl;

	return line;
}

///////////////////////////////////////////////////////////////////////////////////////
// Converts string of d parameter into SVGPath variables, with absolute coordinates
// < Note > : I have not done anything for 'a' and 'A' commands.
///////////////////////////////////////////////////////////////////////////////////////
SVGPath from_string_to_SVGPath(string input, float magnificent_coefficent)
{
	cout << "[] Converting the d_param into SVGPath..." << endl;
	SVGPath path;
	string  token = "";
	char    cmd;
	int     paramCount = 0;
	float   xPrev = 0, yPrev = 0;
	float   x = 0, y = 0;

	for (int i = 0; i < input.length(); ++i) {
		if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z')) {
			cmd = input[i];
			paramCount = 0;
			xPrev = x; yPrev = y;

			path.push_back(SVGData(cmd));
		}
		else if (input[i] == '-' || (input[i] >= '0' && input[i] <= '9')) {
			///////////////////////////////
			// Get the parameter number
			///////////////////////////////
			do {
				token += input[i++];
				if (input[i] == 'e') {       // For numbers like -10e-3 or 69e6, etc.
					token += input[i++];
					if (input[i] == '-')
						token += input[i++];
				}						           // Yes, i know such input as 12.3.3 could cause a big mess, but I believe you, my friend. :) 
			} while (i < input.length() && ((input[i] >= '0' && input[i] <= '9') || (input[i] == '.'))); // <-----------------------------┘

			i--;

			/////////////////////////////////
			// h/H commands
			/////////////////////////////////
			if (cmd == 'h' || cmd == 'H') {
				x = stof(token);
				if (cmd == 'h')  x += xPrev;    // Cause those commands with lowercase are for relative position
				                 xPrev = x;     // For sets of H commands
				path[path.size() - 1].points.push_back(Vector2f(x * magnificent_coefficent, y * magnificent_coefficent));   // Push back new data point
				token = "";
				continue;
			}

			/////////////////////////////////
			// v/V commands
			/////////////////////////////////
			if (cmd == 'v' || cmd == 'V') {
				y = stof(token);
				if (cmd == 'v')  y += yPrev;    // Cause those commands with lowercase are for relative position
								 yPrev = y;     // For sets of H commands
				path[path.size() - 1].points.push_back(Vector2f(x * magnificent_coefficent, y * magnificent_coefficent));   // Push back new data point
				token = "";
				continue;
			}

			///////////////////////////////////////
			// Other commands, including C/c & Q/q
			///////////////////////////////////////
			if (((cmd == 'c' || cmd == 'C') && paramCount % 6 == 0 && paramCount > 0) ||
				((cmd == 'q' || cmd == 'Q') && paramCount % 4 == 0 && paramCount > 0))
			{
				xPrev = x; yPrev = y;
				path.push_back(SVGData(cmd));
			}

			if (paramCount % 2 == 0) {
				x = stof(token);
			}
			else {
				y = stof(token); 
				if (cmd >= 'a' && cmd <= 'z')  x += xPrev, y += yPrev;    // Cause those commands with lowercase are for relative position
				if (cmd == 'l')                xPrev = x, yPrev = y;      // For sets of l commands
				path[path.size() - 1].points.push_back(Vector2f(x * magnificent_coefficent, y * magnificent_coefficent));   // Push back new data point
			}
		
			paramCount++;
			token = "";
		}
	}

	#ifdef DEBUG
		for (int i = 0; i < path.size(); ++i) {
			cout << "[ + ] Cmd: " << path[i].cmd << endl;
			for (int j = 0; j < path[i].points.size(); ++j)
				cout << "  [ - ] Coordinate " << j << ": " << path[i].points[j].x << " " << path[i].points[j].y << endl;
		}

		for (int i = 0; i < path.size(); ++i) {
			cout << (char)toupper(path[i].cmd) << " ";
			for (int j = 0; j < path[i].points.size(); ++j)
				cout << path[i].points[j].x << " " << path[i].points[j].y << " ";
			cout << endl;
		}
	#endif
	cout << "[] I've done it now, bitch! ..." << endl;
	return path;
}

//////////////////////////////////////////////////////////////////////////////
// Resolve path into a list of coordinates, packed in a vector of Vertexes
//////////////////////////////////////////////////////////////////////////////
Polygon convert_SVGPath_to_list_of_coordinates(SVGPath path)
{
	cout << "[] Converting SVGPath variable into a list of coordinates where our program is going to pickup and analyze..." << endl;
	if (path.empty()) return Polygon();
	Polygon  image;
	Vector2f currPoint;
	Vector2f endPoint;
	Vector2f bezierPoint1;
	Vector2f bezierPoint2;

	const float _dt = 0.01;
	int i, j;
	float t;

	for (i = 0; i < path.size(); ++i) {
		switch (path[i].cmd) {
		case 'M':
		case 'm':
		case 'L':
		case 'l':
		case 'H':
		case 'h':
		case 'V':
		case 'v':
		{
			j = 0;
			if (i == 0) {
				currPoint = path[i].points[0];
				image.push_back(Vertex(currPoint));
				j++;
			}

			for (; j < path[i].points.size(); ++j) {
				endPoint = path[i].points[j];

				for (t = 0; t < 1; t += dt) 
					image.push_back(Vertex(Vector2f((1 - t) * currPoint + t * endPoint)));

				currPoint = endPoint;
			}
			break;
		}
		case 'Q':
		case 'q':
		{
			bezierPoint1 = path[i].points[0];
			endPoint     = path[i].points[1];

			for (t = 0; t < 1; t += dt) 
				image.push_back(Vertex(Vector2f(sqr(1-t)* currPoint + 2*(1-t)*t* bezierPoint1 + sqr(t)* endPoint)));

			currPoint = endPoint;
			break;
		}
		case 'C':
		case 'c':
		{
			bezierPoint1 = path[i].points[0];
			bezierPoint2 = path[i].points[1];
			endPoint     = path[i].points[2];

			for (t = 0; t < 1; t += dt)
				image.push_back(Vertex(Vector2f(cube(1-t)* currPoint + 3*sqr(1-t)*t* bezierPoint1 + 3*(1-t)*sqr(t)* bezierPoint2 + cube(t)* endPoint)));

			currPoint = endPoint;
			break;
		}

		case 'Z':
		case 'z':
		{
			endPoint = path[0].points[0];
			for (t = 0; t < 1; t += dt)
				image.push_back(Vertex(Vector2f((1 - t) * currPoint + t * endPoint)));

			break;
		}

		default:
			cout << "[] Oops... This command was not handled yet! Command: " << path[i].cmd << endl;
			assert(false);
			break;
		}
	}

	fstream fs("output.txt", fstream::out);
	for (i = 0; i < image.size(); ++i) {
		fs << image[i].position.x << " " << image[i].position.y << endl;
	}
	fs.close();

	cout << "[] Done it too!" << endl;

	return image;
}