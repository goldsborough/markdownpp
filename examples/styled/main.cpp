#include "../../include/markdown-parser.hpp"

int main(int argc, const char* argv[])
{
	// Set the root path where to look for style at ../../
	Markdown::Parser parser("../../");

	// Add additional stylesheet (for large changes)
	parser.stylesheet("additional.css");

	// Make small change without having to specify additional stylesheet
	parser.add_custom_css("body { color: red; }");

	// Have render_file convert test.md into output.html
	parser.render_file("test.md", "output.html");
}
