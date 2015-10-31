/***************************************************************************//*!
*
*	@file markdown-abstract-markdown.hpp
*
*	@author Peter Goldsborough.
*
*******************************************************************************/

#ifndef MARKDOWN_ABSTRACT_MARKDOWN_HPP
#define MARKDOWN_ABSTRACT_MARKDOWN_HPP

#include "markdown-configurable.hpp"

namespace Markdown
{
	/***********************************************************************//*!
	*
	*	@brief An abstract class that handles markdown rendering.
	*
	*	@details It is expected that a markdown engine convert markdown to
	* 			 valid html and have a mechanism for enabling/disabling flags.
	*
	***************************************************************************/

	class AbstractMarkdown : public Configurable
	{
	public:
		
		/*! For flags such as 'ENABLE_TABLES'. */
		using flags_t = unsigned short;
		
		/*******************************************************************//*!
		*
		*	@brief Initializes members of an abstract markdown engine.
		*
		*	@param settings The configuration-settings for this engine.
		*
		***********************************************************************/
		
		AbstractMarkdown(const Configurable::settings_t& settings);
		
		/*******************************************************************//*!
		*
		*	@brief Destructs members of an abstract markdown engine.
		*
		***********************************************************************/
		
		virtual ~AbstractMarkdown() = default;
		
		/*******************************************************************//*!
		*
		*	@brief Renders markdown to HTML.
		*
		*	@param markdown A string containing markdown.
		*
		*	@return A one-to-one translation of the markdown to HTML, i.e.
		*			without the enclosing <html> or <body> tags. E.g. *hello*
		*			as input should yield "<p><em>hello</em></p>" as output.
		*
		***********************************************************************/
		
		virtual std::string render(const std::string& markdown) = 0;

		/*******************************************************************//*!
		*
		*	@brief Sets the confiuguration-settings via flags.
		*
		*	@param flags An OR-ed combination of configuration-flags.
		*
		***********************************************************************/
		
		virtual void settings(flags_t flags) = 0;
	};
}

#endif /* MARKDOWN_ABSTRACT_MARKDOWN_HPP */