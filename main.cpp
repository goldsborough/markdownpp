#include "markdown-parser.hpp"

#include <boost/program_options.hpp>
#include <iostream>

int main(int argc, const char* argv[])
{
	namespace po = boost::program_options;

	po::options_description description("Usage");

	std::string markdown_style;
	std::string code_style;
	std::string include_mode;
	std::string input;
	std::string output;

	description.add_options()
		("help", "show help")
		(
			"markdown,m",
			po::value<std::string>(&markdown_style)
				->default_value("solarized-dark")
				->value_name("STYLE"),
			"set the markdown-style"
		)
		(
			"code,c",
			po::value<std::string>(&code_style)
				->default_value("xcode")
				->value_name("STYLE"),
			"set the code-style"
		)
		(
			"include-mode,l",
			po::value<std::string>(&include_mode)
				->default_value("network")
				->value_name("MODE"),
			"set the include-mode"
		)
		(
			"input,i",
			po::value<std::string>(&input)
				->required(),
			"the input markdown file"
		)
		(
			"output,o",
			po::value<std::string>(&output)
				->default_value("output.html"),
			"the output html file"
		);
	
	
	po::positional_options_description positional;
	
	positional.add("input", 1);
	positional.add("output", 1);
	
	po::variables_map variables;

	try 
	{
		auto arguments = po::command_line_parser(argc, argv)
							 .options(description)
							 .positional(positional)
							 .run();
			
		po::store(arguments, variables);

		if (variables.count("help"))
		{ 
			std::cout << description;
			
			return EXIT_SUCCESS;
		} 

		po::notify(variables);
		
		Markdown::Parser parser(".", "adjust.css");
	
		parser.configure("include-mode", include_mode);
	
		parser.configure("markdown-style", markdown_style);
		
		parser.configure("code-style", code_style);
		
		parser.render_file(input, output);
		
		std::cout << "Success \033[91m<3\033[0m\n";
	}
	
	catch(po::error& error)
	{ 
		std::cerr << "\033[91mError\033[0m: " << error.what() << "\n";
		std::cerr << description;
		
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
