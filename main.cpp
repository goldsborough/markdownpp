#include "markdown-parser.hpp"
#include "markdown-markdown.hpp"
#include "markdown-configurable.hpp"

#include "hoedown/html.h"
#include "hoedown/document.h"

#include <iostream>

int main(int argc, const char* argv[])
{

	Markdown::Parser parser;
	
	auto html = parser.render_file("../../test3.md");
	
	std::cout << html << std::endl;

	/*
	hoedown_renderer* renderer = hoedown_html_renderer_new(static_cast<hoedown_html_flags>(0), 0);
	
	auto extensions = hoedown_extensions::HOEDOWN_EXT_FENCED_CODE;
	
	std::string markdown = "```Python\nprint(1)\n```";
	
	hoedown_document* document = hoedown_document_new(renderer,
													  static_cast<hoedown_extensions>(extensions),
													  16);

	hoedown_buffer* html = hoedown_buffer_new(markdown.size());
	
	hoedown_document_render(document,
							html,
							reinterpret_cast<const uint8_t*>(markdown.c_str()),
							markdown.size());

	std::cout << html->data << std::endl;
	
	hoedown_document_free(document);
	hoedown_html_renderer_free(renderer);
	hoedown_buffer_free(html);
	*/
}
