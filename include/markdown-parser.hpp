#ifndef MARKDOWNPP_PARSER_HPP
#define MARKDOWNPP_PARSER_HPP

#include "markdown-configurable.hpp"

#include <string>
#include <memory>

namespace Markdown
{
	class Markdown;
	class Math;
	class Code;
	
	std::string render(const std::string& markdown);
	
	std::string snippet(const std::string& markdown);
	
	class Parser : public Configurable
	{
	public:
		
		static const Configurable::settings_t default_settings;
		
		Parser(const Configurable::settings_t& settings = default_settings);
		
		Parser(const Parser& other);
		
		Parser(Parser&& other) noexcept;
		
		Parser& operator=(Parser other);
		
		virtual void swap(Parser& other) noexcept;
		
		friend void swap(Parser& first, Parser& second);
		
		virtual ~Parser();
		
		
		virtual std::string render(const std::string& markdown) const;
		
		virtual std::string snippet(const std::string& markdown) const;
		
		
		virtual const Markdown& markdown() const;
		
		virtual const Math& math() const;
		
	protected:
		
		std::unique_ptr<Markdown> _markdown;
		
		std::unique_ptr<Math> _math;
	};
}

#endif /* MARKDOWNPP_PARSER_HPP */
