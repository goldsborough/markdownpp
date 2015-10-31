/***************************************************************************//*!
*
*	@file markdown-math.hpp
*
*	@author Peter Goldsborough.
*
*******************************************************************************/

#ifndef MARKDOWN_MATH_HPP
#define MARKDOWN_MATH_HPP

#include "markdown-abstract-math.hpp"

#include <stdexcept>
#include <string>
#include <v8.h>

namespace Markdown
{
	/***********************************************************************//*!
	*
	*	@brief Renders Math to HTML.
	*
	*	@details Uses <a href="khan.github.io/KaTeX">KaTeX</a>.
	*			 Configuration (key : values [default]):
	*			 + all-display-math	: (true | false) [false]
	*
	***************************************************************************/

	class Math : public AbstractMath
	{
	public:
		
		/*! Thrown when the LaTeX expression could not be rendered by KaTeX. */
		struct ParseException : public std::runtime_error
		{
			ParseException(const std::string& what)
			: std::runtime_error(what)
			{ }
		};
		
		/*! The default settings for this math engine. */
		static const Configurable::settings_t default_settings;
		
		/*******************************************************************//*!
		*
 		*	@brief Constructs a new Math engine.
		*
		*	@param settings The settings for the Math engine.
		*
		***********************************************************************/
		
		Math(const Configurable::settings_t& settings = default_settings);
		
		/*******************************************************************//*!
		*
 		*	@brief Copy-constructs a Math engine.
		*
		*	@param other The other Math object.
		*
		***********************************************************************/
		
		Math(const Math& other);
		
		/*******************************************************************//*!
		*
 		*	@brief Move-constructs a Math engine.
		*
		*	@param other The other Math object.
		*
		***********************************************************************/
		
		Math(Math&& other) noexcept;
		
		/*******************************************************************//*!
		*
 		*	@brief Assigns the Math engine.
		*
		*	@param other The other Math object.
		*
		***********************************************************************/
		
		Math& operator=(Math other);
		
		/*******************************************************************//*!
		*
 		*	@brief Swaps members with another Math engine.
		*
		*	@param other The other Math object.
		*
		***********************************************************************/
		
		void swap(Math& other) noexcept;
		
		/*******************************************************************//*!
		*
 		*	@brief Swaps members of two Math engines.
		*
		*	@param first The first Math object.
		*
		*	@param second The second Math object.
		*
		***********************************************************************/
		
		friend void swap(Math& first, Math& second) noexcept;
		
		/*******************************************************************//*!
		*
 		*	@brief Destructs the Math engine.
		*
		***********************************************************************/
		
		~Math();
		
		/*******************************************************************//*!
		*
 		*	@brief Renders a LaTeX expression to HTML.
		*
		*	@param expression The LaTeX expression to render to HTML.
		*
		*	@param display_math Whether to use a displaymath environment.
		*
		*	@return A HTML snippet without any enclosing <html> or <body> tags.
		*
		***********************************************************************/
		
		virtual std::string render(const std::string& expression,
								   bool display_math = false) override;
		
		
	private:
		
		struct Allocator : public v8::ArrayBuffer::Allocator
		{
			virtual void* Allocate(size_t length) override;
			
			virtual void* AllocateUninitialized(size_t length) override;
			
			virtual void Free(void* data, size_t) override;
		};
		
		static struct V8
		{
			V8();
			
			~V8();
			
			std::unique_ptr<v8::Platform> platform;
			
		} _v8;

		/*******************************************************************//*!
		*
		*	@brief Creates and initializes a new v8::Isolate.
		*
		*	@details Deals with setting the Allocator instance in the
		*			 isolate's parameters.
		*
		*	@return A __naked pointer__ to a v8::Isolate.
		*
		***********************************************************************/

		v8::Isolate* _new_isolate() const;
		
		/*******************************************************************//*!
		*
		*	@brief Compiles and executes JavaScript code via the V8 engine.
		*
		*	@param source The JavaScript source-code to execute.
		*
		*	@param context The context in which to compile and execute the code.
		*
		*	@return Any return value of the execution.
		*
		*	@throws ParseException If there was an exception in the JS
		*						   environment, which can only stem
		*						   from a parse-exception.
		*
		***********************************************************************/
		
		v8::Local<v8::Value> _run(const std::string& source,
								  const v8::Local<v8::Context>& context) const;

		/*******************************************************************//*!
		*
		*	@brief Retrieves the JavaScript for calling KaTeX.
		*
		*	@details Handles determining whether to use display-math depending
		*			 one the display-mode flag in the configuration and the
		*			 display_math parameter passed to the render() method.
		*
		*	@param	expression A LaTeX expression.
		*
		*	@param	display_math The display_math parameter for this expression.
		*
		***********************************************************************/

		std::string _get_javascript(const std::string& expression,
									bool display_math) const;
		
		/*******************************************************************//*!
		*
		*	@brief Escapes backslashes and whitespace in the LaTeX source.
		*
		*	@details The problem is that even though you may escape backslashes
		*			 in the string you pass to the Latex instance, those
		*			 backlashes then have to again be escaped for the JavaScript
		*			 source (as they are simply backslashes for the JS
		*			 environment otherwise, which initiate escape sequences).
		*
		*	@return The escaped LaTeX string.
		*
		***********************************************************************/
		
		std::string _escape(std::string source) const;
		
		/*******************************************************************//*!
		*
		*	@brief Loads the KaTeX JavaScript library.
		*
		*	@details The KaTeX library is loaded and executed in the current
		*			 V8 context, such that subsequent code executed in that
		*			 environment can access the KaTeX library.
		*
		*	@param	context A V8 context object.
		*
		***********************************************************************/
		
		void _load_katex(const v8::Local<v8::Context>& context) const;
		
		/*! A instance of the Allocator struct for the V8 engine. */
		mutable Allocator _allocator;
		
		/*! The virtual environment in which the V8 runs. */
		v8::Isolate* _isolate;
		
	
		/*! A persistent (i.e. non-expiring/global) handle
		    to the context in which the instance interacts
		    with the V8 engine. */
		v8::UniquePersistent<v8::Context> _persistent_context;
		
	};
	
}

#endif /* MARKDOWN_MATH_HPP */