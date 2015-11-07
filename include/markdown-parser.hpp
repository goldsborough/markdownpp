/***************************************************************************//*!
*
*	@file markdown-parser.hpp
*
*	@author Peter Goldsborough.
*
*******************************************************************************/

#ifndef MARKDOWNPP_PARSER_HPP
#define MARKDOWNPP_PARSER_HPP

#include "markdown-configurable.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace Markdown
{
	class AbstractMarkdown;
	class AbstractMath;
	class Code;
	
	/***********************************************************************//*!
	*
	*	@brief Renders a markdown snippet.
	*
	*	@param markdown The markdown to render.
	*
	*	@return A HTML snippet without any enclosing <html> or <body> tags.
	*
	***************************************************************************/

	std::string snippet(const std::string& markdown);

	/***********************************************************************//*!
	*
	*	@brief Handles snippet and file parsing.
	*
	*	@details Configuration (key : values [default]):
	*			 + enable-math 		: (true | false) [true]
	*			 + enable-code 		: (true | false) [true]
	*			 + markdown-style 	: (see themes/markdown) [github]
	*			 + code-style		: (see themes/code/style) [solarized-dark]
	*			 + include-mode		: (embed|local|network) [network]
	*			 + file-protocol	: (true | false) [false]
	*
	***************************************************************************/
	
	class Parser : public Configurable
	{
	public:
		
		/*! The default settings for a Parser. */
		static const Configurable::settings_t default_settings;
		
		/*******************************************************************//*!
		*
 		*	@brief Constructs a new Parser instance.
		*
		*	@param root The root path for the themes/ and katex/ folders.
		*
		*	@param stylesheet_path The path to a custom stylesheet.
		*
		*	@param settings The settings for the parser.
		*
		***********************************************************************/
		
		Parser(const std::string& root = ".",
			   const std::string& stylesheet_path = std::string(),
			   const Configurable::settings_t& settings = default_settings);

		/*******************************************************************//*!
		*
 		*	@brief Constructs a new Parser instance.
		*
		*	@details The `std::unique_ptr`s passed are invalidated
		*			 (their contents are moved).
		*
		*	@param markdown_engine A pointer to the markdown engine to use.
		*
		*	@param math_engine A pointer to the math engine to use.
		*
		*	@param root The root path for the themes/ and katex/ folders.
		*
		*	@param stylesheet_path The path to a custom stylesheet.
		*
		*	@param settings The settings for the parser.
		*
		***********************************************************************/
		
		Parser(std::unique_ptr<AbstractMarkdown> markdown_engine,
			   std::unique_ptr<AbstractMath> math_engine,
			   const std::string& root = ".",
			   const std::string& stylesheet_path = std::string(),
			   const Configurable::settings_t& settings = default_settings);
		
		/*******************************************************************//*!
		*
		*	@brief Deleted copy-constructor.
		*
		*	@details Copy-constructing is not possible because math and
		*			 markdown engines cannot be shared between two parsers
		*			 and there is no way to determine the concrete class
		*			 of the object to which the pointers point.
		*
		***********************************************************************/
		
		Parser(const Parser& other) = delete;
		
		/*******************************************************************//*!
		*
		*	@brief Move-constructs the parser.
		*
		*	@param other The other Parser object.
		*
		***********************************************************************/
		
		Parser(Parser&& other) noexcept;
		
		/*******************************************************************//*!
		*
		*	@brief Deleted copy-assignment operator.
		*
		*	@details (Copy-)Assignment is not possible because math and
		*			 markdown engines cannot be shared between two parsers
		*			 and there is no way to determine the concrete class
		*			 of the object to which the pointers point.
		*
		***********************************************************************/
		
		Parser& operator=(Parser other) = delete;
		
		/*******************************************************************//*!
		*
		*	@brief Swaps members with another Parser.
		*
		*	@param other The other Parser object.
		*
		***********************************************************************/
		
		virtual void swap(Parser& other) noexcept;
		
		/*******************************************************************//*!
		*
		*	@brief Swaps members of two Parsers.
		*
		*	@param first The first Parser object.
		*
		*	@param second The second Parser object.
		*
		***********************************************************************/
		
		friend void swap(Parser& first, Parser& second);
		
		/*******************************************************************//*!
		*
		*	@brief Destructs a Parser object.
		*
		***********************************************************************/
		
		virtual ~Parser();
		
		/*******************************************************************//*!
		*
		*	@brief Renders markdown and returns a __full__ HTML document.
		*
		*	@details The rendered HTML is a complete web-page with included
		*			 or embedded CSS and JavaScript files.
		*
		*	@return A full HTML document with the rendered markdown.
		*
		***********************************************************************/
		
		virtual std::string render(std::string markdown);
		
		/*******************************************************************//*!
		*
		*	@brief Renders markdown contained in a file.
		*
		*	@details Calls render() with the contents of the file.
		*
		*	@param path The path of the file containing the markdown.
		*
		*	@return A full HTML document with the rendered markdown.
		*
		*	@see render(std::string)
		*
		***********************************************************************/
		
		virtual std::string render_file(const std::string& path);

		/*******************************************************************//*!
		*
		*	@brief Renders a markdown file to an HTML file.
		*
		*	@details Writes the output of render_file() to the destination.
		*			 Erases any previous contents of the destination file.
		*
		*	@param path The path of the file containing the markdown.
		*
		*	@param destination The path where the output should be written to.
		*
		*	@see render_file(std::string)
		*
		***********************************************************************/
		
		virtual void render_file(const std::string& path,
								 const std::string& destination);
		
		/*******************************************************************//*!
		*
		*	@brief Renders a markdown snippet.
		*
		*	@param markdown The markdown to render.
		*
		*	@return A HTML snippet without any enclosing <html> or <body> tags.
		*
		***********************************************************************/
		
		virtual std::string snippet(std::string markdown) const;

		/*******************************************************************//*!
		*
		*	@brief Sets the path for the custom stylesheet.
		*
		*	@param path The path to the stylesheet.
		*
		***********************************************************************/
		
		virtual void stylesheet(const std::string& path);

		/*******************************************************************//*!
		*
		*	@brief Returns the path for the custom stylesheet.
		*
		***********************************************************************/
		
		virtual const std::string& stylesheet() const;
		
		/*******************************************************************//*!
		*
		*	@brief Removes the link to the custom sytlesheet.
		*
		***********************************************************************/
		
		virtual void remove_stylesheet();
		
		/*******************************************************************//*!
		*
		*	@brief Adds a CSS snippet to the custom-CSS buffer.
		*
		*	@details The CSS should be formatted so that it can be included
		*			 in a <style> tag, e.g. 'body { color: red; }'.
		*
		*	@param css The additional CSS.
		*
		***********************************************************************/
		
		virtual void add_custom_css(const std::string& css);
		
		/*******************************************************************//*!
		*
		*	@brief Returns the current accumulation of custom-CSS.
		*
		***********************************************************************/
		
		virtual const std::string& custom_css() const;
		
		/*******************************************************************//*!
		*
		*	@brief Clears any custom-CSS accumulated so far.
		*
		***********************************************************************/
		
		virtual void clear_custom_css();
		
		/*******************************************************************//*!
		*
		*	@brief Sets the root directory path.
		*
		*	@details root/katex/ and root/themes should be valid paths
		*			 (i.e. those folders should be accessible from the root).
		*
		*	@param root The new root directory path.
		*
		***********************************************************************/
		
		virtual void root(const std::string& root);
		
		/*******************************************************************//*!
		*
		*	@brief Returns the current root path.
		*
		***********************************************************************/
		
		virtual const std::string& root() const;
		
		
		/*******************************************************************//*!
		*
		*	@brief Sets the markdown-engine.
		*
		*	@details The `std::unique_ptr` passed is invalidated
		*			 (its contents are moved).
		*
		*	@param markdown_engine A pointer to an markdown engine.
		*
		***********************************************************************/
		
		virtual void markdown(std::unique_ptr<AbstractMarkdown> markdown_engine);
		
		/*******************************************************************//*!
		*
		*	@brief Sets the math-engine.
		*
		*	@details The `std::unique_ptr` passed is invalidated
		*			 (its contents are moved).
		*
		*	@param math_engine A pointer to an math engine.
		*
		***********************************************************************/
		
		virtual void math(std::unique_ptr<AbstractMath> math_engine);
		
		/*******************************************************************//*!
		*
		*	@brief Returns the markdown-engine.
		*
		*	@details `const`-version.
		*
		***********************************************************************/
		
		virtual const AbstractMarkdown& markdown() const;

		/*******************************************************************//*!
		*
		*	@brief Returns the math-engine.
		*
		*	@details `const`-version.
		*
		***********************************************************************/
		
		virtual const AbstractMath& math() const;

		/*******************************************************************//*!
		*
		*	@brief Returns the markdown-engine.
		*
		***********************************************************************/
		
		virtual AbstractMarkdown& markdown();
		
		/*******************************************************************//*!
		*
		*	@brief Returns the math-engine.
		*
		***********************************************************************/
		
		virtual AbstractMath& math();
		
	protected:
		
		/*! A container for equations. */
		using equations_t = std::vector<std::string>;
		
		/*! A pair of inline-math and display-math equations. */
		using extraction_t = std::pair<equations_t, equations_t>;
		
		/*! An HTML tag (opening and closing). */
		using tag_t = std::pair<std::string, std::string>;
		
		
		/*! A <link> HTML tag. */
		static const tag_t _link;

		/*! A <script> HTML tag with external source. */
		static const tag_t _external_script;
		
		/*! A <script> HTML tag with embedded source code. */
		static const tag_t _embedded_script;
		
		/*! A <style> HTML tag. */
		static const tag_t _style;
		
		/*******************************************************************//*!
		*
		*	@brief Handles retrieval of a CSS stylesheet.
		*
		*	@details Returns:
		*			 * A <link> tag with local source in *local* include-mode.
		*			 * A <link> tag with online source in *network* include-mode.
		*			 * A <style> tag with embedded code in *embed* include-mode.
		*
		*	@param path The path to the stylesheet folder.
		*
		***********************************************************************/
		
		virtual std::string _get_stylesheet(const std::string& path) const;

		/*******************************************************************//*!
		*
		*	@brief Handles retrieval of a JavaScript script.
		*
		*	@details Returns:
		*			 * A <script> tag with local source in *local* include-mode.
		*			 * A <script> tag with online source in *network* include-mode.
		*			 * A <script> tag with embedded code in *embed* include-mode.
		*
		*	@param path The path to the JavaScript script folder.
		*
		***********************************************************************/
		
		virtual std::string _get_script(const std::string& path) const;
		
		/*******************************************************************//*!
		*
		*	@brief Extracts LaTeX equations from Markdown source.
		*
		*	@details LaTeX equations are extracted one-by-one and replaced
		*			 with numeric markers. After the resulting Markdown is
		*			 rendered by the Markdown-engine and the LaTeX equations
		*			 have been rendered by the Math-engine, the numeric markers
		*			 are replaced with their respective rendered LaTeX (HTML).
		*
		*	@param markdown The markdown from which to extract.
		*
		*	@return An extraction_t with first being the inline-math and
		*			second being the display-math.
		*
		***********************************************************************/
		
		virtual extraction_t _extract_math(std::string& markdown) const;
		
		/*******************************************************************//*!
		*
		*	@brief Extracts a pattern-matching string from a markdown source.
		*
		*	@param markdown The markdown source from which to extract.
		*
		*	@param pattern The regex pattern extracted strings should match.
		*
		***********************************************************************/
		
		virtual equations_t _extract(std::string& markdown,
							 const std::string& pattern) const;
		
		/*******************************************************************//*!
		*
		*	@brief Converts math equations one-by-one.
		*
		*	@details Operates in-place and replaces LaTeX equations with the
		*			 rendered HTML (for each index in each set of equations).
		*
		*	@param equations The equations to render with the math-engine.
		*
		***********************************************************************/
		
		virtual void _convert_math(extraction_t& equations) const;
		
		/*******************************************************************//*!
		*
		*	@brief Re-inserts the rendered math into the destination HTML.
		*
		*	@param html The rendered markdown.
		*
		*	@param equations The equations to insert.
		*
		***********************************************************************/
		
		virtual void _insert_math(std::string& html,
								  extraction_t& equations) const;
		
		/*******************************************************************//*!
		*
		*	@brief Reads a file and returns its contents.
		*
		*	@param path The path to the file to read.
		*
		***********************************************************************/
		
		virtual std::string _read_file(const std::string& path) const;
		
		/*******************************************************************//*!
		*
		*	@brief Enables code-highlighting.
		*
		*	@return The necessary CSS and JavaScript links and
		*		 	<script> tags for the final HTML document.
		*
		***********************************************************************/
		
		virtual std::string _enable_code() const;
		
		/*******************************************************************//*!
		*
		*	@brief Handles the custom CSS (stylesheet and snippets).
		*
		*	@return The necessary CSS links and <style> tags.
		*
		***********************************************************************/
		
		virtual std::string _add_custom_css();
		
		/*******************************************************************//*!
		*
		*	@brief Makes an HTML tag with contents.
		*
		*	@param tag The relevant tag_t instance.
		*
		*	@param contents What to put between tag.first and tag.second;
		*
		*	@return tag.first + contents + tag.second.
		*
		***********************************************************************/
		
		virtual inline std::string
		_make_tag(const tag_t& tag, const std::string& contents) const;
		
		/*******************************************************************//*!
		*
		*	@brief Joins paths with the _root directory.
		*
		*	@param paths The paths to join with the _root directory.
		*
		*	@return _root/x/y/z/...
		*
		***********************************************************************/
		
		virtual std::string
		_join_paths(const std::vector<std::string>& paths) const;
		
		/*******************************************************************//*!
		*
		*	@brief Escapes </script> tags in the contents of a <script> tag.
		*
		*	@details Necessary because highlight.js' JavaScript source
		*			 contains the string '</script>' which closes the
		*			 <script> tag in which highlight.js' code is included.
		*
		*	@param raw_script The raw JavaScript source.
		*
		*	@return The same string, but with </script> -> <\/script>.
		*
		***********************************************************************/
		
		virtual inline std::string
		_escape_script(const std::string& raw_script) const;
		
		
		/*! The root directory path. */
		std::string _root;
		
		/*! The markdown-engine in use. */
		std::unique_ptr<AbstractMarkdown> _markdown;

		/*! The math-engine in use. */
		std::unique_ptr<AbstractMath> _math;
		
		/*! The stylesheet path. */
		std::string _stylesheet;
		
		/*! The accumulated custom CSS. */
		std::string _custom_css;
	};
}

#endif /* MARKDOWNPP_PARSER_HPP */
