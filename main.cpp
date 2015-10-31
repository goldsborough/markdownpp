#include "markdown-parser.hpp"

#include <iostream>

int main(int argc, const char* argv[])
{
	Markdown::Parser parser("../../", "adjust.css");
	
	parser.render_file("README.md", "docs/README.html");
}
