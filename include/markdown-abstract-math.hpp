/***************************************************************************//*!
*
*	@file markdown-abstract-math.hpp
*
*	@author Peter Goldsborough.
*
*******************************************************************************/

#ifndef MARKDOWN_ABSTRACT_MATH_HPP
#define MARKDOWN_ABSTRACT_MATH_HPP

#include "markdown-configurable.hpp"

namespace Markdown
{
	/***********************************************************************//*!
	*
	*	@brief An abstract class that handles math rendering.
	*
	*	@details It is expected that a math engine
	*			 convert LaTeX expressions to html.
	*
	***************************************************************************/

	class AbstractMath : public Configurable
	{
	public:
		
		/*******************************************************************//*!
		*
		*	@brief Initializes members of an abstract math engine.
		*
		*	@param settings The configuration-settings for this engine.
		*
		***********************************************************************/
		
		AbstractMath(const Configurable::settings_t& settings);
		
		/*******************************************************************//*!
		*
		*	@brief Destructs members of an abstract math engine.
		*
		***********************************************************************/
		
		virtual ~AbstractMath() = default;
		
		/*******************************************************************//*!
		*
		*	@brief Renders math to HTML.
		*
		*	@param expression A string containing a LaTeX expression.
		*
		*	@param display_math Whether to use display-math for the expression.
		*
		*	@return A <span> or <div> without any enclosing
		*			<html> or <body> tags, such that the result
		*			can be embedded in an HTML document.
		*
		***********************************************************************/
		
		virtual std::string render(const std::string& expression,
								   bool display_math) = 0;
	};
}

#endif /* MARKDOWN_ABSTRACT_MATH_HPP */