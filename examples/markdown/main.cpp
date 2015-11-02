#include "../../include/markdown-parser.hpp"

int main(int argc, const char* argv[])
{
	// Set the root path where to look for style at ../../
	Markdown::Parser parser("../../");

	parser.configure("markdown-style", "solarized-dark");

	parser.configure("code-style", "xcode");

	parser.configure("include-mode", "embed");

	// Have render_file convert test.md into output.html
	parser.render_file("test.md", "output.html");
}
