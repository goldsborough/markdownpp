#include "markdown-abstract-markdown.hpp"
#include "markdown-abstract-math.hpp"
#include "markdown-markdown.hpp"
#include "markdown-math.hpp"
#include "markdown-parser.hpp"

#include <fstream>
#include <regex>

namespace Markdown
{
	std::string render(const std::string& markdown)
	{
		Parser parser;
		
		return parser.render(markdown);
	}
	
	std::string snippet(const std::string& markdown)
	{
		Parser parser;
		
		return parser.snippet(markdown);
	}
	
	const Configurable::settings_t Parser::default_settings = {
		{"enable-math", "1"},
		{"enable-highlighting", "1"},
		{"markdown-style", "github"},
		{"code-style", "solarized-dark"},
		{"include-mode", "network"}
	};
	
	const Parser::tag_t Parser::_link = {
		"<link type='text/css' rel='stylesheet' href='",
		"'>\n"
	};
	
	const Parser::tag_t Parser::_script = {
		"<script type='text/javascript' src='",
		"'></script>\n"
	};
	
	Parser::Parser(const Configurable::settings_t& settings,
				   const std::string& stylesheet_path)
	: Configurable(settings)
	, _markdown(std::make_unique<Markdown>())
	, _math(std::make_unique<Math>())
	, _stylesheet_path(stylesheet_path)
	{ }
	
	Parser::Parser(std::unique_ptr<AbstractMarkdown> markdown_engine,
				   std::unique_ptr<AbstractMath> math_engine,
				   const Configurable::settings_t& settings,
				   const std::string& stylesheet_path)
	: Configurable(settings)
	, _markdown(std::move(markdown_engine))
	, _math(std::move(math_engine))
	, _stylesheet_path(stylesheet_path)
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
	
	
	std::string Parser::render(std::string markdown)
	{
		std::string html = "<!DOCTYPE html>\n<html>\n<head>\n"
						   "<meta charset='utf-8'/>\n";
		
		html += _get_stylesheet("../../katex");
		
		auto markdown_style = Configurable::get("markdown-style");
		
		if (markdown_style != "none")
		{
			html += _get_stylesheet("../../style/themes/" + markdown_style);
		}
		
		if (Configurable::get<bool>("enable-highlighting"))
		{
			html += _enable_highlighting();
		}
		
		if (! _stylesheet_path.empty() || ! _custom_css.empty())
		{
			html += _add_custom_css();
		}
		
		html += "</head>\n<body>\n";
		html += snippet(markdown);
		html += "</body>\n</html>";
		
		return html;
	}
	
	std::string Parser::render_file(const std::string &path)
	{
		auto markdown = _read_file(path);
		
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
	
	void Parser::stylesheet(const std::string& path)
	{
		if (Configurable::get("include-mode") == "embed")
		{
			_stylesheet = _read_file(path);
		}
		
		_stylesheet_path = path;
	}
	
	const std::string& Parser::stylesheet() const
	{
		return _stylesheet_path;
	}
	
	void Parser::remove_stylesheet()
	{
		_stylesheet.clear();
		
		_stylesheet_path.clear();
	}
	
	
	void Parser::add_css(const std::string& css)
	{
		_custom_css += css;
	}
	
	const std::string& Parser::custom_css() const
	{
		return _custom_css;
	}
	
	void Parser::remove_custom_css()
	{
		_custom_css.clear();
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
	
	std::string Parser::_get_stylesheet(const std::string &path) const
	{
		auto include_mode = Configurable::get("include-mode");
		
		if (include_mode == "embed")
		{
			auto css = _read_file(path + "/style.css");
			
			return "<style>\n" + css + "</style>\n";
		}
		
		else if(include_mode == "local")
		{
			return _link.first + path + "/style.css" + _link.second;
		}
		
		else if (include_mode == "network")
		{
			auto url = _read_file(path + "/network.url");
			
			return _link.first + url + _link.second;
		}
		
		else throw ConfigurationValueException("include-mode", include_mode);
	}
	
	std::string Parser::_get_script(const std::string &path) const
	{
		auto include_mode = Configurable::get("include-mode");
		
		if (include_mode == "embed")
		{
			auto css = _read_file(path + "/script.js");
			
			return "<script>\n" + css + _script.second;
		}
		
		else if(include_mode == "local")
		{
			return _script.first + path + "/script.js" + _script.second;
		}
		
		else if (include_mode == "network")
		{
			auto url = _read_file(path + "/network.url");
			
			return _script.first + url + _script.second;
		}
		
		else throw ConfigurationValueException("include-mode", include_mode);
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
	
	std::string Parser::_read_file(const std::string &path) const
	{
		std::ifstream file(path);
		
		if (! file)
		{
			throw FileException("Could not open file '" + path + "'!");
		}
		
		std::string contents;
		
		std::copy(std::istreambuf_iterator<char>{file},
				  std::istreambuf_iterator<char>{},
				  std::back_inserter(contents));
		
		// Strip trailing whitespace
		auto end = std::find_if_not(contents.rbegin(),
									contents.rend(),
									::isspace);
		
		contents.erase(end.base(), contents.end());
		
		return contents;
	}
	
	std::string Parser::_enable_highlighting() const
	{
		auto code_style = Configurable::get("code-style");
		
		if (code_style == "none") return "";
		
		// highlight.js uses underscores, we use hyphens
		std::replace(code_style.begin(), code_style.end(), '-', '_');
		
		auto html = _get_stylesheet("../../style/code/themes/" + code_style);
			
		html += _get_script("../../style/code/highlight");
			
		html += "<script>hljs.initHighlightingOnLoad();</script>\n";
		
		return html;
	}
	
	std::string Parser::_add_custom_css()
	{
		std::string html;
		
		if (! _stylesheet_path.empty())
		{
			if (Configurable::get("include-mode") == "embed")
			{
				if (_stylesheet.empty())
				{
					_stylesheet = _read_file(_stylesheet_path);
				}
				
				html += "<style>\n" + _stylesheet + "</style>\n";
			}
			
			else html += _link.first + _stylesheet_path + _link.second;
		}
		
		if (! _custom_css.empty())
		{
			html += "<style>\n" + _custom_css + "</style>\n";
		}
		
		return html;
	}
}