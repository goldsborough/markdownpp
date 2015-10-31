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
	class Math : public AbstractMath
	{
	public:
		
		enum Flags { DisplayMode };
		
		struct ParseException : public std::runtime_error
		{
			ParseException(const std::string& what)
			: std::runtime_error(what)
			{ }
		};
		
		static const Configurable::settings_t default_settings;
		
		Math(const Configurable::settings_t& settings = default_settings);
		
		Math(const Math& other);
		
		Math(Math&& other) noexcept;
		
		Math& operator=(Math other);
		
		void swap(Math& other) noexcept;
		
		friend void swap(Math& first, Math& second) noexcept;
		
		~Math();
		
		
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

		v8::Isolate* _new_isolate() const;
		
		v8::Local<v8::Value> _run(const std::string& source,
								  const v8::Local<v8::Context>& context) const;

		std::string _get_javascript(const std::string& expression,
									bool display_math) const;
		
		std::string _escape(std::string source) const;
		
		void _load_katex(const v8::Local<v8::Context>& context) const;
		
		
		mutable Allocator _allocator;
		
		v8::Isolate* _isolate;
		
		v8::UniquePersistent<v8::Context> _persistent_context;
		
	};
	
}

#endif /* MARKDOWN_MATH_HPP */