#ifndef MARKDOWNPP_PARSER_HPP
#define MARKDOWNPP_PARSER_HPP

#include "markdown-configurable.hpp"

#include <stdexcept>
#include <string>
#include <memory>

namespace Markdown
{
	class AbstractMarkdown;
	class AbstractMath;
	class Code;
	
	std::string render(const std::string& markdown);
	
	std::string snippet(const std::string& markdown);
	
	class Parser : public Configurable
	{
	public:
		
		struct FileException : public std::runtime_error
		{
			FileException(const std::string& what)
			: std::runtime_error(what)
			{ }
		};
		
		static const Configurable::settings_t default_settings;
		
		Parser(const Configurable::settings_t& settings = default_settings);
		
		Parser(std::unique_ptr<AbstractMarkdown> markdown_engine,
			   std::unique_ptr<AbstractMath> math_engine,
			   const Configurable::settings_t& settings = default_settings);
		
		Parser(const Parser& other) = delete;
		
		Parser(Parser&& other) noexcept;
		
		Parser& operator=(Parser other) = delete;
		
		virtual void swap(Parser& other) noexcept;
		
		friend void swap(Parser& first, Parser& second);
		
		virtual ~Parser();
		
		
		virtual std::string render(const std::string& markdown) const;
		
		virtual std::string render_file(const std::string& path);
		
		virtual void render_file(const std::string& path,
								 const std::string& destination);
		
		virtual std::string snippet(const std::string& markdown) const;
		
		
		virtual const AbstractMarkdown& markdown() const;
		
		virtual const AbstractMath& math() const;
		
		virtual AbstractMarkdown& markdown();
		
		virtual AbstractMath& math();
		
	protected:
		
		std::unique_ptr<AbstractMarkdown> _markdown;
		
		std::unique_ptr<AbstractMath> _math;
	};
}

#endif /* MARKDOWNPP_PARSER_HPP */
