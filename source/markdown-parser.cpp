#include "markdown-abstract-markdown.hpp"
#include "markdown-abstract-math.hpp"
#include "markdown-markdown.hpp"
#include "markdown-math.hpp"
#include "markdown-parser.hpp"

#include <fstream>
#include <regex>
#include <thread>

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
		{"enable-math", "1"},
		{"markdown-style", "witex"},
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
	
	
	std::string Parser::render(std::string markdown) const
	{
		std::string html = "<!DOCTYPE html>\n<html>\n";
		
		html += "<head>\n<meta charset='utf-8'/>\n";
		
		html += _get_stylesheet_link("katex/katex.min.css");
		
		auto markdown_style = Configurable::get("markdown-style");
		
		if (markdown_style != "none")
		{
			html += _get_stylesheet_link("themes/markdown/"
										 + markdown_style
										 + "/style.css");
		}
		
		auto code_style = Configurable::get("code-style");

		if (code_style != "none")
		{
			html += _get_stylesheet_link("themes/code/"
										 + code_style
										 + "/style.css");
		}
		
		html += "</head>\n<body>\n";
		html += snippet(markdown);
		html += "</body>\n</html>";
		
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
		std::ofstream file(destination, std::ios::trunc);
		
		if (! file)
		{
			throw FileException("Could not open file '" + path + "'!");
		}
		
		auto html = render_file(path);
		
		file << html;
	}
	
	std::string Parser::snippet(std::string markdown) const
	{
		if (Configurable::get<bool>("enable-math"))
		{
			auto equations = _extract_math(markdown);
			
			auto html = _markdown->render(markdown);
			
			_convert_math(equations);
			
			_insert_math(html, equations);
			
			return html;
		}
		
		else return _markdown->render(markdown);
	}
	
	void Parser::markdown(std::unique_ptr<AbstractMarkdown> markdown_engine)
	{
		_markdown = std::move(markdown_engine);
	}
	
	
	void Parser::math(std::unique_ptr<AbstractMath> math_engine)
	{
		_math = std::move(math_engine);
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
	
	inline std::string Parser::_get_stylesheet_link(const std::string &path) const
	{
		static const std::string link = "<link rel='stylesheet' "
									    "type='text/css' href='";
		
		return link + path + "'>\n";
	}

	Parser::extraction_t Parser::_extract_math(std::string& markdown) const
	{
		static const std::string inline_math("(?:^|[^$])\\$([^$]+?)\\$(?!\\$)");
		
		static const std::string display_math("\\${2}([^$]+?)\\${2}");
		
		auto inline_matches = _extract(markdown, inline_math);
		
		auto display_matches = _extract(markdown, display_math);
		
		return {inline_matches, display_matches};
	}
	
	Parser::equations_t
	Parser::_extract(std::string &markdown,
					 const std::string& pattern) const
	{
		equations_t equations;
		
		std::regex regex_pattern(pattern,
								 std::regex_constants::ECMAScript |
								 std::regex_constants::optimize);
		
		std::smatch match;
		
		auto begin = markdown.cbegin();
		
		for (std::size_t count = 0;
			 std::regex_search(begin, markdown.cend(), match, regex_pattern);
			 ++count)
		{
			// Get the actual LaTeX equation
			equations.push_back(match.str(1));
			
			// Offset because match.position is relative to begin
			auto offset = std::distance(markdown.cbegin(), begin);
			
			auto marker = std::to_string(count);
			
			markdown.replace(offset + match.position(1),
							 match.length(1),
							 marker);
			
			// Skip the marker and the trailing $
			std::advance(begin, match.position(1) + marker.size() + 2);
		}
		
		return equations;
	}
	
	void Parser::_convert_math(extraction_t &equations) const
	{
		for (auto& equation : equations.first)
		{
			equation = _math->render(equation, false);
		}
		
		for (auto& equation : equations.second)
		{
			equation = _math->render(equation, true);
		}
	}
	
	void Parser::_insert_math(std::string &html, extraction_t &equations) const
	{
		// Display-math first (have to rely on first getting rid of
		// the double $, because if you use a lookbehind to see if
		// there is another $ for the inline statements, that would
		// also eat up the character matching (?:^|[^$]), which we
		// don't want)
		for (std::size_t i = 0; i < equations.second.size(); ++i)
		{
			std::regex pattern("\\${2}" + std::to_string(i) + "\\${2}");
			
			html = std::regex_replace(html, pattern, equations.second[i]);
		}
		
		// Inline-math
		for (std::size_t i = 0; i < equations.first.size(); ++i)
		{
			std::regex pattern("\\$" + std::to_string(i) + "\\$");
			
			html = std::regex_replace(html, pattern, equations.first[i]);
		}
	}
}