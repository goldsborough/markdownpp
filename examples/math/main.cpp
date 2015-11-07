#include "../../include/markdown-abstract-math.hpp"
#include "../../include/markdown-parser.hpp"

int main(int argc, const char* argv[])
{
	// Set the root path where to look for style at ../../
	Markdown::Parser parser("../../");

	// Don't throw exceptions, just mark
	// faulty equations in a special color
	parser.math().configure("throw-on-error", false);

	// Set the "faulty-equation" color to blue
	parser.math().configure("error-color", "#0000FF");

	// Have render_file convert test.md into output.html
	parser.render_file("test.md", "output.html");
}
