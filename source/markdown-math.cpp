#include "markdown-math.hpp"
#include "markdown-exceptions.hpp"

#include <boost/filesystem.hpp>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <libplatform/libplatform.h>
#include <regex>

namespace Markdown
{
	Math::V8 Math::_v8;
	
	const Configurable::settings_t Math::default_settings = {
		{"all-display-math", "0"},
		{"throw-on-error", "1"},
		{"error-color", "#CC0000"},
		{"log-errors", "1"}
	};
	
	Math::Math(const std::string& katex_path,
			   const Configurable::settings_t& settings)
	: AbstractMath(settings)
	, _isolate(_new_isolate())
	, _katex_path(katex_path)
	{		
		v8::HandleScope handle_scope(_isolate);
		
		v8::Isolate::Scope isolate_scope(_isolate);
		
		auto context = v8::Context::New(_isolate);
		
		v8::Context::Scope context_scope(context);
		
		_persistent_context = v8::UniquePersistent<v8::Context>(_isolate, context);
		
		_load_katex(context);
	}
	
	Math::Math(const Math& other)
	: Math(other._katex_path)
	{ }
	
	Math::Math(Math&& other) noexcept
	: Math()
	{
		swap(other);
	}
	
	Math& Math::operator=(Math other)
	{
		swap(other);
		
		return *this;
	}
	
	void Math::swap(Math &other) noexcept
	{
		// Enable ADL
		using std::swap;
		
		swap(_allocator, other._allocator);
		
		swap(_isolate, other._isolate);
		
		swap(_persistent_context, other._persistent_context);
	}
	
	void swap(Math& first, Math& second) noexcept
	{
		first.swap(second);
	}
	
	Math::~Math() = default;
	
	std::string Math::render(const std::string &expression,
							 bool display_math)
	{
		v8::Isolate::Scope isolate_scope(_isolate);
		
		// Stack-allocated handle-scope (takes care of handles such
		// that object are garbage-collected after the scope ends)
		v8::HandleScope handle_scope(_isolate);
		
		// Get a local context handle from the persistent handle.
		auto context = v8::Local<v8::Context>::New(_isolate,
												   _persistent_context);
		
		v8::Context::Scope context_scope(context);
		
		auto source = _get_javascript(expression, display_math);
		
		v8::Local<v8::Value> value;
		
		try
		{
			value = _run(source, context);
		}
		
		catch(const ParseException& exception)
		{
			if (! Configurable::get<bool>("throw-on-error"))
			{
				return _handle_error(expression);
			}
			
			else throw exception;
		}
		
		std::string html = *static_cast<v8::String::Utf8Value>(value);
		
		return "<span class='math'>\n" + html + "</span>\n";
	}
	
	const std::string& Math::katex_path() const noexcept
	{
		return _katex_path;
	}
	
	void Math::katex_path(const std::string& path)
	{
		_katex_path = path;
		
		v8::Isolate::Scope isolate_scope(_isolate);
		
		v8::HandleScope handle_scope(_isolate);
		
		// Get a local context handle from the persistent handle.
		auto context = v8::Local<v8::Context>::New(_isolate,
												   _persistent_context);
		
		_load_katex(context);
	}
	
	v8::Isolate* Math::_new_isolate() const
	{
		v8::Isolate::CreateParams parameters;
		
		parameters.array_buffer_allocator = &_allocator;
		
		// Isolated JavaScript Virtual Environment
		return v8::Isolate::New(parameters);
	}
	
	v8::Local<v8::Value> Math::_run(const std::string& source,
									 const v8::Local<v8::Context>& context) const
	{
		v8::EscapableHandleScope handle_scope(_isolate);
		
		auto unchecked = v8::String::NewFromUtf8(_isolate,
												 source.c_str(),
												 v8::NewStringType::kNormal);
		
		auto checked = unchecked.ToLocalChecked();
		
		// Compile the source code.
		auto script = v8::Script::Compile(context, checked).ToLocalChecked();
		
		// V8 engine's try-catch mechanism
		v8::TryCatch try_catch(_isolate);
		
		auto result = script->Run(context);
		
		if (result.IsEmpty())
		{
			// Grab last exception
			auto exception = try_catch.Exception();
			
			std::string what = *static_cast<v8::String::Utf8Value>(exception);
			
			// Remove the 'ParseError' (redundant)
			throw ParseException(what.substr(12));
		}
		
		// Allows us to return local-scope objects to the outside scope
		return handle_scope.Escape(result.ToLocalChecked());
	}
	
	std::string Math::_get_javascript(const std::string &expression,
									  bool display_math) const
	{
		std::string source = "katex.renderToString('";
		
		source += _escape(expression);
		
		source += "', {'displayMode': ";
		
		// all-display-math | display_math | result
		// 		 0		    |	  0		   |   0
		// 		 0		    |	  1		   |   1
		// 		 1		    |	  0		   |   1
		// 		 1		    |	  1		   |   1
		display_math |= Configurable::get<bool>("all-display-math");
		
		source += display_math ? "true})" : "false})";
		
		return source;
	}
	
	std::string Math::_escape(std::string source) const
	{
		static const std::regex bad_backslashes("(?:\\\\)+(\\s|$)");
		static const std::regex good_backslashes("\\\\");
		static const std::regex space("[\\t\\n]+");
		
		source = std::regex_replace(source, bad_backslashes, "$1");
		source = std::regex_replace(source, good_backslashes, "\\\\");
		source = std::regex_replace(source, space, "");
		
		return source;
	}
	
	void Math::_load_katex(const v8::Local<v8::Context>& context) const
	{
		auto path = boost::filesystem::path(_katex_path);
		
		path /= boost::filesystem::path("katex.min.js");
		
		std::ifstream file(path.string());
		
		if (! file)
		{
			throw FileException("Could not load katex.min.js!");
		}
		
		std::string source;
		
		std::copy(std::istreambuf_iterator<char>{file},
				  std::istreambuf_iterator<char>{},
				  std::back_inserter(source));
		
		_run(source, context);
	}
	
	std::string Math::_handle_error(const std::string &expression) const
	{
		if (Configurable::get<bool>("log-errors"))
		{
			std::clog << "Could not parse expression '"
			<< expression
			<< "'!\n";
		}
		
		std::string html = "<span style='color: ";
		
		html += Configurable::get("error-color");
		
		html += "'>" + expression + "</span>";
		
		return html;
	}
	
	void* Math::Allocator::Allocate(std::size_t length)
	{
		auto data = AllocateUninitialized(length);
		
		return data ? std::memset(data, 0, length) : data;
	}
	
	void Math::Allocator::Free(void *data, std::size_t)
	{
		free(data);
	}
	
	void* Math::Allocator::AllocateUninitialized(std::size_t length)
	{
		return malloc(length);
	}
	
	Math::V8::V8()
	: platform(v8::platform::CreateDefaultPlatform())
	{
		v8::V8::InitializeICU();
		
		v8::V8::InitializePlatform(platform.get());
		
		v8::V8::Initialize();
	}
	
	Math::V8::~V8()
	{
		v8::V8::Dispose();
		
		v8::V8::ShutdownPlatform();
	}
}