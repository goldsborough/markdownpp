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
	
	class Parser : public Configurable<1>
	{
	public:
		
		Parser(const Configurable::flags_t& flags = Configurable::flags_t());
		
		Parser(const Parser& other);
		
		Parser(Parser&& other) noexcept;
		
		Parser& operator=(Parser other);
		
		virtual void swap(Parser& other) noexcept;
		
		friend void swap(Parser& first, Parser& second);
		
		virtual ~Parser();
		
		
		virtual std::string render(const std::string& markdown) const;
		
		
		virtual const Markdown& markdown() const;
		
		virtual const Math& math() const;
		
		virtual const Code& code() const;
		
	protected:
		
		std::unique_ptr<Markdown> _markdown;
		
		std::unique_ptr<Math> _math;
		
		std::unique_ptr<Code> _code;
	};
}

#endif /* MARKDOWNPP_PARSER_HPP */
