#include "markdown-parser.hpp"

#include <iostream>

int main(int argc, const char* argv[])
{
	Markdown::Parser parser;

	parser.configure("code-style", "solarized-dark");
	
	parser.configure("enable-code", true);
	
	parser.configure("include-mode", "local");
	
	parser.stylesheet("test/additional.css");
	
	parser.render_file("../../test/test.md",
					   "../../output.html");
}
