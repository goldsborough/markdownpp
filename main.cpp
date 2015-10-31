#include "markdown-parser.hpp"

#include <iostream>

int main(int argc, const char* argv[])
{
	Markdown::Parser parser("../../", "README.css");
	
	parser.render_file("README.md", "docs/README.html");
}
