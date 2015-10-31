#include "../../include/markdown-parser.hpp"

#include <iostream>

int main(int argc, const char* argv[])
{
	// Set the root path where to look for style at ../../
	Markdown::Parser parser("../../");

	// Using a parser object for many calls
	std::cout << parser.snippet("*hello*, `world`__!!!__");

	// Or just using the namespace-visible
	// snippet function for one-shot rendering
	std::cout << Markdown::snippet("~~strike~~ http://github.com");
}
