#include "markdown-parser.hpp"

#include <iostream>

int main(int argc, const char* argv[])
{
	Markdown::Parser parser("../../", "adjust.css");
	
	parser.configure("markdown-style", "solarized-dark");
	
	parser.configure("code-style", "xcode");
	
	parser.render_file("README.md", "docs/index.html");
}
