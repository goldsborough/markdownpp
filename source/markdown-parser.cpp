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
	
	std::string snippet(const std::string& markdown)
	{
		const Parser parser;
		
		return parser.snippet(markdown);
	}
	
	const Configurable::settings_t Parser::default_settings = {
		{"enable-math", "true"},
		{"style", "none"},
		{"code-style", "none"}
	};
	
	Parser::Parser(const Configurable::settings_t& settings)
	: Configurable(settings)
	, _markdown(std::make_unique<Markdown>())
	, _math(std::make_unique<Math>())
	{ }
	
	Parser::Parser(const Parser& other)
	: Configurable(other._settings)
	, _markdown(std::make_unique<Markdown>(*other._markdown))
	, _math(std::make_unique<Math>(*other._math))
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
	}
	
	void swap(Parser& first, Parser& second)
	{
		first.swap(second);
	}
	
	Parser::~Parser() = default;
	
	
	std::string Parser::render(const std::string &markdown) const
	{
		// extract math, leave comment-markers
		
		auto html = snippet(markdown);
		
		return html;
	}
	
	std::string Parser::snippet(const std::string &markdown) const
	{
		// extract math, leave comment-markers
		
		return _markdown->render(markdown);
	}
	
	const Markdown& Parser::markdown() const
	{
		return *_markdown;
	}
	
	
	const Math& Parser::math() const
	{
		return *_math;
	}
	
	Markdown& Parser::markdown()
	{
		return *_markdown;
	}
	
	
	Math& Parser::math()
	{
		return *_math;
	}
}