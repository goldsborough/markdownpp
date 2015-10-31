#include "markdown-parser.hpp"

#include <iostream>

int main(int argc, const char* argv[])
{
	Markdown::Parser parser;

	parser.stylesheet("additional.css");
	
	parser.render_file("../../ignore/test.md",
					   "../../output.html");
}
