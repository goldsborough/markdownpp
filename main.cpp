#include "markdown-parser.hpp"

#include <iostream>

int main(int argc, const char* argv[])
{

	Markdown::Parser parser;

	parser.render_file("../../ignore/test3.md",
					   "../../ignore/output.html");
}
