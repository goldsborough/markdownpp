#include "markdown-parser.hpp"

#include <iostream>

int main(int argc, const char* argv[])
{
	Markdown::Parser parser;

	parser.configure("code-style", "railscasts");
	
	parser.configure("enable-code", false);
	
	parser.configure("include-mode", "local");
	
	parser.stylesheet("../../test/additional.css");
	
	parser.render_file("../../test/test.md",
					   "../../output.html");
}
