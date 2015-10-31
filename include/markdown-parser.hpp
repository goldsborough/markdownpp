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
		
		Parser(const Configurable::settings_t& settings = default_settings,
			   const std::string& stylesheet_path = std::string());
		
		Parser(std::unique_ptr<AbstractMarkdown> markdown_engine,
			   std::unique_ptr<AbstractMath> math_engine,
			   const Configurable::settings_t& settings = default_settings,
			   const std::string& stylesheet_path = std::string());
		
		Parser(const Parser& other) = delete;
		
		Parser(Parser&& other) noexcept;
		
		Parser& operator=(Parser other) = delete;
		
		virtual void swap(Parser& other) noexcept;
		
		friend void swap(Parser& first, Parser& second);
		
		virtual ~Parser();
		
		
		virtual std::string render(std::string markdown);
		
		virtual std::string render_file(const std::string& path);
		
		virtual void render_file(const std::string& path,
								 const std::string& destination);
		
		virtual std::string snippet(std::string markdown) const;
		
		
		virtual void stylesheet(const std::string& path);
		
		virtual const std::string& stylesheet() const;
		
		virtual void remove_stylesheet();
		
		
		virtual void add_css(const std::string& css);
		
		virtual const std::string& custom_css() const;
		
		virtual void remove_custom_css();
		
		
		virtual void markdown(std::unique_ptr<AbstractMarkdown> markdown_engine);
		
		virtual void math(std::unique_ptr<AbstractMath> math_engine);
		
		virtual const AbstractMarkdown& markdown() const;
		
		virtual const AbstractMath& math() const;
		
		virtual AbstractMarkdown& markdown();
		
		virtual AbstractMath& math();
		
	protected:
		
		using equations_t = std::vector<std::string>;
		
		using extraction_t = std::pair<equations_t, equations_t>;
		
		using tag_t = std::pair<std::string, std::string>;
		
		
		static const tag_t _link;
		
		static const tag_t _script;
		
		static const tag_t _style;
		
		
		std::string _get_stylesheet(const std::string& path) const;
		
		std::string _get_script(const std::string& path) const;
		
		extraction_t _extract_math(std::string& markdown) const;
		
		equations_t _extract(std::string& markdown,
							 const std::string& pattern) const;
		
		void _convert_math(extraction_t& equations) const;
		
		void _insert_math(std::string& html,
						  extraction_t& equations) const;
		
		void _render_equation(std::string& equation) const;
		
		std::string _read_file(const std::string& path) const;
		
		std::string _enable_highlighting() const;
		
		std::string _add_custom_css();
		
		
		std::unique_ptr<AbstractMarkdown> _markdown;
		
		std::unique_ptr<AbstractMath> _math;
		
		std::string _stylesheet_path;
		
		std::string _stylesheet;
		
		std::string _custom_css;
	};
}

#endif /* MARKDOWNPP_PARSER_HPP */
