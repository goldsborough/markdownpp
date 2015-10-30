#include "markdown-parser.hpp"
#include "markdown-markdown.hpp"
#include "markdown-configurable.hpp"

#include "hoedown/html.h"
#include "hoedown/document.h"

#include <iostream>

int main(int argc, const char* argv[])
{

	Markdown::Parser parser;

	parser.render_file("../../ignore/test3.md",
					   "../../ignore/output.html");
	/*
	auto html = parser.render_file("../../test/test3.md");
	
	std::cout << html << std::endl;
	*/
}
