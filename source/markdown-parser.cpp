#include "markdown-abstract-markdown.hpp"
#include "markdown-abstract-math.hpp"
#include "markdown-markdown.hpp"
#include "markdown-math.hpp"
#include "markdown-parser.hpp"

#include <fstream>

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
	
	Parser::Parser(std::unique_ptr<AbstractMarkdown> markdown_engine,
				   std::unique_ptr<AbstractMath> math_engine,
				   const Configurable::settings_t& settings)
	: Configurable(settings)
	, _markdown(std::move(markdown_engine))
	, _math(std::move(math_engine))
	{ }
	
	Parser::Parser(Parser&& other) noexcept
	: Parser()
	{
		swap(other);
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
	
	std::string Parser::render_file(const std::string &path)
	{
		std::ifstream file(path);
		
		if (! file)
		{
			throw FileException("Could not open file '" + path + "'!");
		}
		
		std::string markdown;
		
		std::copy(std::istreambuf_iterator<char>{file},
				  std::istreambuf_iterator<char>{},
				  std::back_inserter(markdown));
		
		return render(markdown);
	}
	
	void Parser::render_file(const std::string &path,
							   const std::string &destination)
	{
		std::ofstream file(destination);
		
		if (! file)
		{
			throw FileException("Could not open file '" + path + "'");
		}
		
		auto html = render_file(path);
		
		file << html;
	}
	
	std::string Parser::snippet(const std::string &markdown) const
	{
		// extract math, leave comment-markers
		
		return _markdown->render(markdown);
	}
	
	const AbstractMarkdown& Parser::markdown() const
	{
		return *_markdown;
	}
	
	
	const AbstractMath& Parser::math() const
	{
		return *_math;
	}
	
	AbstractMarkdown& Parser::markdown()
	{
		return *_markdown;
	}
	
	
	AbstractMath& Parser::math()
	{
		return *_math;
	}
}