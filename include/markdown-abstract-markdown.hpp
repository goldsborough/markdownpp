#ifndef MARKDOWN_ABSTRACT_MARKDOWN_HPP
#define MARKDOWN_ABSTRACT_MARKDOWN_HPP

#include "markdown-configurable.hpp"

namespace Markdown
{
	class AbstractMarkdown : public Configurable
	{
	public:
		
		using flags_t = unsigned short;
		
		AbstractMarkdown(const Configurable::settings_t& settings);
		
		virtual ~AbstractMarkdown() = default;
		
		virtual std::string render(const std::string&) = 0;
		
		virtual void settings(flags_t) = 0;
	};
}

#endif /* MARKDOWN_ABSTRACT_MARKDOWN_HPP */