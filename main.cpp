#include "markdown-parser.hpp"

#include <iostream>

int main(int argc, const char* argv[])
{
	Markdown::Parser parser("/Users/petergoldsborough/Documents/Projects/markdownpp");

	parser.configure("code-style", "railscasts");
	
	parser.configure("enable-code", true);
	
	parser.configure("include-mode", "network");
	
	parser.stylesheet("test/additional.css");
	
	parser.render_file("../../test/test.md",
					   "../../output.html");
}
