#include "markdown-markdown.hpp"

#include <hoedown/html.h>
#include <hoedown/document.h>

namespace Markdown
{
	const Configurable::settings_t Markdown::default_settings = {
		{"tables", "1"},
		{"fenced-code", "1"},
		{"footnotes", "1"},
		{"autolink", "1"},
		{"strike", "1"},
		{"underline", "1"},
		{"quote", "1"},
		{"superscript", "1"},
	};
	
	Markdown::Markdown(const Configurable::settings_t& settings)
	: AbstractMarkdown(settings)
	, _renderer(hoedown_html_renderer_new(static_cast<hoedown_html_flags>(0), 16))
	, _buffer(hoedown_buffer_new(100))
	{ }
	
	Markdown::Markdown(flags_t flags)
	: Markdown()
	{
		settings(flags);
	}
	
	Markdown::Markdown(const Markdown& other)
	: AbstractMarkdown(other._settings)
	, _renderer(hoedown_html_renderer_new(static_cast<hoedown_html_flags>(0), 16))
	, _buffer(hoedown_buffer_new(other._buffer->size))
	{ }
	
	Markdown::Markdown(Markdown&& other) noexcept
	: Markdown()
	{
		swap(other);
	}

	Markdown& Markdown::operator=(Markdown other)
	{
		swap(other);
		
		return *this;
	}
	
	void Markdown::swap(Markdown& other) noexcept
	{
		// Enable ADL
		using std::swap;
		
		swap(_renderer, other._renderer);
		
		swap(_buffer, other._buffer);
	}
	
	void swap(Markdown& first, Markdown& second) noexcept
	{
		first.swap(second);
	}
	
	Markdown::~Markdown()
	{
		hoedown_buffer_free(_buffer);
		hoedown_html_renderer_free(_renderer);
	}
	
	auto Markdown::_load_extensions() const
	{
		flags_t extensions = 0;
		
		if (get<bool>("tables")) extensions |= Flags::TABLES;
		
		if (get<bool>("fenced-code")) extensions |= Flags::FENCED_CODE;
		
		if (get<bool>("footnotes")) extensions |= Flags::FOOTNOTES;
		
		if (get<bool>("autolink")) extensions |= Flags::AUTOLINK;
		
		if (get<bool>("strike")) extensions |= Flags::STRIKE;
		
		if (get<bool>("underline")) extensions |= Flags::UNDERLINE;
		
		if (get<bool>("quote")) extensions |= Flags::QUOTE;
		
		if (get<bool>("superscript")) extensions |= Flags::SUPERSCRIPT;
		
		return static_cast<hoedown_extensions>(extensions);
	}
	
	std::string Markdown::render(const std::string& markdown)
	{
		static const std::size_t nesting_depth = 16;
		
		auto document = hoedown_document_new(_renderer,
											 _load_extensions(),
											 nesting_depth);
		
		_buffer = _verify_buffer_size(markdown.size());
		
		auto data = _to_uint8_string(markdown);
		
		hoedown_document_render(document,
								_buffer,
								data.get(),
								markdown.size());
		
		auto result = _buffer_to_string(_buffer);
		
		hoedown_buffer_reset(_buffer);

		return result;
	}
	
	void Markdown::settings(flags_t flags)
	{
		Configurable::configure("tables", flags & Flags::TABLES);
		
		Configurable::configure("fenced-code", flags & Flags::FENCED_CODE);
		
		Configurable::configure("tables", flags & Flags::FOOTNOTES);
		
		Configurable::configure("autolink", flags & Flags::AUTOLINK);
		
		Configurable::configure("strike", flags & Flags::STRIKE);
		
		Configurable::configure("underline", flags & Flags::UNDERLINE);
		
		Configurable::configure("quote", flags & Flags::QUOTE);
		
		Configurable::configure("superscript", flags & Flags::SUPERSCRIPT);
	}
	
	inline hoedown_buffer*
	Markdown::_verify_buffer_size(std::size_t new_size) const
	{
		if (_buffer->size < new_size)
		{
			hoedown_buffer_grow(_buffer, new_size);
		}
		
		return _buffer;
	}
	
	inline std::unique_ptr<std::uint8_t[]>
	Markdown::_to_uint8_string(const std::string &string) const
	{
		auto result = std::make_unique<std::uint8_t[]>(string.size());
		
		std::copy(string.begin(), string.end(), result.get());
		
		return result;
	}
	
	inline std::string
	Markdown::_buffer_to_string(const hoedown_buffer *buffer) const
	{
		std::string string(buffer->size, '\0');
		
		std::copy(buffer->data,
				  buffer->data + buffer->size,
				  string.begin());
		
		return string;
	}
}