#ifndef MARKDOWNPP_MARKDOWN_HPP
#define MARKDOWNPP_MARKDOWN_HPP

#include "markdown-configurable.hpp"

#include <cstdint>
#include <string>

struct hoedown_buffer;
struct hoedown_document;
struct hoedown_renderer;

namespace Markdown
{
	class Markdown : public Configurable
	{
	public:
		
		enum Flags
		{
			TABLES        = 0x0,
			FENCED_CODE   = 0x1,
			FOOTNOTES     = 0x2,
			AUTOLINK      = 0x4,
			STRIKE 		  = 0x8,
			UNDERLINE 	  = 0x10,
			QUOTE         = 0x20,
			SUPERSCRIPT   = 0x40
		};
		
		using flags_t = unsigned char;
		
		static const Configurable::settings_t default_settings;
		
		Markdown(const Configurable::settings_t& settings = default_settings);
		
		Markdown(flags_t flags);
		
		Markdown(const Markdown& other);
		
		Markdown(Markdown&& other) noexcept;
		
		Markdown& operator=(Markdown other);
		
		void swap(Markdown& other) noexcept;
		
		friend void swap(Markdown& first, Markdown& second) noexcept;
		
		~Markdown();
		
		
		std::string render(const std::string& markdown);
		
		
		void settings(flags_t flags);
		
	private:
		
		auto _load_extensions() const;
		
		inline hoedown_buffer* _verify_buffer_size(std::size_t new_size) const;
		
		inline std::unique_ptr<std::uint8_t[]>
		_to_uint8_string(const std::string& string) const;
		
		inline std::string _buffer_to_string(const hoedown_buffer* buffer) const;
		
		
		hoedown_renderer* _renderer;
		
		hoedown_buffer* _buffer;
		
	};
}

#endif /* MARKDOWNPP_MARKDOWN_HPP */
