/***************************************************************************//*!
*
*	@file markdown-markdown.hpp
*
*	@author Peter Goldsborough.
*
*******************************************************************************/

#ifndef MARKDOWNPP_MARKDOWN_HPP
#define MARKDOWNPP_MARKDOWN_HPP

#include "markdown-abstract-markdown.hpp"

#include <cstdint>
#include <string>

struct hoedown_buffer;
struct hoedown_document;
struct hoedown_renderer;

namespace Markdown
{
	/***********************************************************************//*!
	*
	*	@brief The default markdown-rendering engine.
	*
	*	@details Uses <a href="https://github.com/hoedown/hoedown">hoedown</a>.
	*
	***************************************************************************/

	class Markdown : public AbstractMarkdown
	{
	public:
		
		/*! The flags with which settings can be manipulated. */
		enum Flags
		{
			TABLES        = 0x1,
			FENCED_CODE   = 0x2,
			FOOTNOTES     = 0x4,
			AUTOLINK      = 0x8,
			STRIKE 		  = 0x10,
			UNDERLINE 	  = 0x20,
			QUOTE         = 0x40,
			SUPERSCRIPT   = 0x100
		};
		
		/*! The default settings for this markdown engine. */
		static const Configurable::settings_t default_settings;
		
		/*******************************************************************//*!
		*
		*	@brief Constructs a new Markdown engine with settings.
		*
		*	@param settings The configuration-settings for this engine.
		*
		***********************************************************************/
		
		Markdown(const Configurable::settings_t& settings = default_settings);
		
		/*******************************************************************//*!
		*
		*	@brief Constructs a new Markdown engine with flags.
		*
		*	@param flags A flag(-combination) as settings.
		*
		***********************************************************************/
		
		Markdown(flags_t flags);
		
		/*******************************************************************//*!
		*
		*	@brief Copy-constructs a Markdown-engine.
		*
		*	@param other The other Markdown object.
		*
		***********************************************************************/
		
		Markdown(const Markdown& other);

		/*******************************************************************//*!
		*
		*	@brief Move-constructs a Markdown-engine.
		*
		*	@param other The other Markdown object.
		*
		***********************************************************************/
		
		Markdown(Markdown&& other) noexcept;
		
		/*******************************************************************//*!
		*
		*	@brief Assigns the Markdown-engine.
		*
		*	@param other The other Markdown object.
		*
		***********************************************************************/
		
		Markdown& operator=(Markdown other);
		
		/*******************************************************************//*!
		*
		*	@brief Swaps members with a Markdown-engine.
		*
		*	@param other The other Markdown object.
		*
		***********************************************************************/
		
		void swap(Markdown& other) noexcept;
		
		/*******************************************************************//*!
		*
		*	@brief Swaps members of two Markdown-engines.
		*
		*	@param first The first Markdown object.
		*
		*	@param second The second Markdown object.
		*
		***********************************************************************/
		
		friend void swap(Markdown& first, Markdown& second) noexcept;
		
		/*******************************************************************//*!
		*
		*	@brief Destructs a Markdown object.
		*
		***********************************************************************/
		
		~Markdown();

		/*******************************************************************//*!
		*
 		*	@brief Renders Markdown to HTML.
		*
		*	@param markdown The Markdown to render.
		*
		*	@return A one-to-one translation of the markdown to HTML
		*			(i.e. no <body>, <html> or other enclosing tags.)
		*
		***********************************************************************/
		
		std::string render(const std::string& markdown) override;
		
		/*******************************************************************//*!
		*
 		*	@brief Sets configuration-settings from flags.
		*
		*	@param flags A flag(-combination) for the new settings.
		*
		***********************************************************************/
		
		void settings(AbstractMarkdown::flags_t flags) override;
		
	private:
		
		/*******************************************************************//*!
		*
 		*	@brief Extracts the extensions from the settings.
		*
		*	@details `auto` return type because you can't forward-declare
		*			 an unscoped enum (and avoid including hoedown headers).
		*
		*	@return An flag combination of type `hoedown_extensions`.
		*
		***********************************************************************/
		
		auto _load_extensions() const;
		
		/*******************************************************************//*!
		*
 		*	@brief Verifies the buffer size and potentially grows it.
		*
		*	@param new_size The necessary size of the buffer.
		*
		***********************************************************************/
		
		inline hoedown_buffer*
		_verify_buffer_size(std::size_t new_size) const;
		
		/*******************************************************************//*!
		*
 		*	@brief Converts a string to a const std::uint8_t* safely.
		*
		*	@param string A std::string.
		*
		*	@return A `std::unique_ptr<std::uint8_t[]>` from which the pointer
		*			can safely be retrieved via `std::unique_ptr<T>::get()`.
		*
		***********************************************************************/
		
		inline std::unique_ptr<std::uint8_t[]>
		_to_uint8_string(const std::string& string) const;
		
		/*******************************************************************//*!
		*
 		*	@brief Converts a hoedown_buffer to a string.
		*
		*	@param buffer A pointer to a hoedown_buffer.
		*
		*	@return A std::string with the contents of the buffer.
		*
		***********************************************************************/
		
		inline std::string
		_buffer_to_string(const hoedown_buffer* buffer) const;
		
		
		/*! The main hoedown_renderer. */
		hoedown_renderer* _renderer;
		
		/*! The sole (resizing) hoedown_buffer. */
		hoedown_buffer* _buffer;
		
	};
}

#endif /* MARKDOWNPP_MARKDOWN_HPP */
