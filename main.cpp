#include "markdown-parser.hpp"

#include <iostream>

int main(int argc, const char* argv[])
{
	Markdown::Parser parser;

	parser.stylesheet("test/additional.css");
	
	parser.render_file("../../test/test.md",
					   "../../output.html");
}
