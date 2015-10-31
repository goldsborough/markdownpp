#include "../../include/markdown-parser.hpp"

int main(int argc, const char* argv[])
{
	// Set the root path where to look for style at ../../
	Markdown::Parser parser("../../");

	// True by default but just for demonstration (set to
	// false to disable code highlighting altogether)
	parser.configure("enable-code", true);

	// See all styles here: https://highlightjs.org/static/demo/

	parser.configure("include-mode", "network");

	parser.configure("code-style", "solarized-dark");
	parser.render_file("solarized-dark.md", "solarized-dark.html");

	parser.configure("code-style", "solarized-light");
	parser.render_file("solarized-light.md", "solarized-light.html");

	parser.configure("include-mode", "local");

	parser.configure("code-style", "xcode");
	parser.render_file("xcode.md", "xcode.html");

	parser.configure("code-style", "railscasts");
	parser.render_file("railscasts.md", "railscasts.html");

	parser.configure("include-mode", "embed");

	parser.configure("code-style", "far");
	parser.render_file("far.md", "far.html");
}
