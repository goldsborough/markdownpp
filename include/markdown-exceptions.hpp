#ifndef MARKDOWN_EXCEPTIONS_HPP
#define MARKDOWN_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace Markdown
{
	/*! An exception thrown when a requested key is not available. */
	struct ConfigurationKeyException : public std::invalid_argument
	{
		ConfigurationKeyException(const std::string& key)
		: std::invalid_argument("No such key '" + key + "'!")
		{ }
	};
	
	/*! An exception thrown when an invalid value is supplied for a key. */
	struct ConfigurationValueException : public std::invalid_argument
	{
		ConfigurationValueException(const std::string& key,
									const std::string& value)
		: std::invalid_argument("Invalid value '" + value +
								"' for key '" + key + "'!")
		{ }
	};
	
	/*! Thrown when the LaTeX expression could not be rendered by KaTeX. */
	struct ParseException : public std::runtime_error
	{
		ParseException(const std::string& what)
		: std::runtime_error(what)
		{ }
	};
	
	/*! Thrown when a file could not be opened. */
	struct FileException : public std::runtime_error
	{
		FileException(const std::string& what)
		: std::runtime_error(what)
		{ }
	};
}

#endif /* MARKDOWN_EXCEPTIONS_HPP */
