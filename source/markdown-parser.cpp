#include "markdown-code.hpp"
#include "markdown-markdown.hpp"
#include "markdown-math.hpp"
#include "markdown-parser.hpp"

namespace Markdown
{
	std::string render(const std::string& markdown)
	{
		const Parser parser;
		
		return parser.render(markdown);
	}
	
	Parser::Parser(const Configurable::flags_t& flags)
	: Configurable(flags)
	{ }
	
	Parser::Parser(const Parser& other)
	: _markdown(other._markdown.get())
	, _math(other._math.get())
	, _code(other._code.get())
	{ }
	
	Parser::Parser(Parser&& other) noexcept
	: Parser()
	{
		swap(other);
	}
	
	Parser& Parser::operator=(Parser other)
	{
		swap(other);
		
		return *this;
	}
	
	void Parser::swap(Parser &other) noexcept
	{
		// Enable Argument-Dependent-Lookup (ADL)
		using std::swap;
		
		swap(_markdown, other._markdown);
		
		swap(_math, other._math);
		
		swap(_code, other._code);
	}
	
	void swap(Parser& first, Parser& second)
	{
		first.swap(second);
	}
	
	Parser::~Parser() = default;
	
	
	std::string Parser::render(const std::string &markdown) const
	{
		// extract math, leave comment-markers
		
		
	}
	
	const Markdown& Parser::markdown() const
	{
		return *_markdown;
	}
	
	
	const Math& Parser::math() const
	{
		return *_math;
	}
	
	
	const Code& Parser::code() const
	{
		return *_code;
	}
}