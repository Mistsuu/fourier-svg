#define SOURCE
#include "Include.h"
#include "Complex.h"
#include "Fourier.h"
#include "SVG.h"
#include "Arm.h"
#include "Calc.h"
#include "Draw.h"

using namespace std;

void displayHelp() {
    cout << "[Usage:] fourier-svg <input.svg> [ -z <zoom> ] [ -x <off-x> ] [ -y <off-y> ] [ -h ] [ -d <draw-mode> ]\n\n";
    cout << "   <input.svg>       Input file.\n";
    cout << "\n";
    cout << "   -h                Display this help.\n";
    cout << "   -z <zoom>         (float,  Default 1.0)        Set the zoom level for the image.\n";
    cout << "   -x <off-x>        (int,    Default 0)          Set the offset in the X-direction.\n";
    cout << "   -y <off-y>        (int,    Default 0)          Set the offset in the Y-direction.\n";
    cout << "   -d <draw-mode>    (string, Default DEFAULT)    Set draw mode. It could be one of the following values:\n\n";
    cout << "                     DEFAULT:     Showing drawing arm and the figure being drawn.\n";
    cout << "                     CIRCLE:      Showing drawing arm and the figure being drawn with the circle around the drawing arm.\n";
    cout << "                     FIGURE-ONLY: Showing figure being drawn ONLY.\n";
}

int main(int argc, char** argv)
{
    // Specify for display help...
    if (argc == 1) {
        displayHelp();
        return 0;
    }
    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "-h") {
            displayHelp();
            return 0;
        }
    }

    // Get filename
    string filename = string(argv[1]);

    // Get other args
    float  zoom=1.0;
    int    offX=0;
    int    offY=0;
    string drawMode = "DEFAULT";
    for (int i = 2; i < argc-1; ++i) {
        if (string(argv[i]) == "-z")
            zoom     = stof(string(argv[i+1]));
        if (string(argv[i]) == "-x")
            offX     = stoi(string(argv[i+1]));
        if (string(argv[i]) == "-y")
            offY     = stoi(string(argv[i+1]));
        if (string(argv[i]) == "-d")
            drawMode = string(argv[i+1]);
    }

    draw(filename, zoom, offX, offY, drawMode);
}

