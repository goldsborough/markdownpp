#ifndef MARKDOWN_ABSTRACT_MATH_HPP
#define MARKDOWN_ABSTRACT_MATH_HPP

#include "markdown-configurable.hpp"

namespace Markdown
{
	class AbstractMath : public Configurable
	{
	public:
		
		AbstractMath(const Configurable::settings_t& settings);
		
		virtual ~AbstractMath() = default;
		
		virtual std::string render(const std::string&) = 0;
	};
}

#endif /* MARKDOWN_ABSTRACT_MATH_HPP */