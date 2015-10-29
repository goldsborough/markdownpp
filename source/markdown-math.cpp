#include "markdown-math.hpp"

#include <cstdlib>

namespace Markdown
{
	Math::Math()
	: _isolate(_new_isolate())
	{		
		v8::HandleScope handle_scope(_isolate);
		
		v8::Isolate::Scope isolate_scope(_isolate);
		
		auto context = v8::Context::New(_isolate);
		
		v8::Context::Scope context_scope(context);
		
		_persistent_context = v8::UniquePersistent<v8::Context>(_isolate, context);
	}
	
	Math::Math(const Math& other)
	: Math()
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
	
	std::string Math::render(const std::string &expression) const
	{
		v8::Isolate::Scope isolate_scope(_isolate);
		
		// Stack-allocated handle-scope (takes care of handles such
		// that object are garbage-collected after the scope ends)
		v8::HandleScope handle_scope(_isolate);
		
		// Get a local context handle from the persistent handle.
		auto context = v8::Local<v8::Context>::New(_isolate,
												   _persistent_context);
		
		v8::Context::Scope context_scope(context);
		
		auto source = _get_javascript(expression);
		
		auto value = _run(source, context);
		
		std::string html = *static_cast<v8::String::Utf8Value>(value);
		
		return "<div class='math'>\n" + html + "</div>\n";
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
	
	std::string Math::_get_javascript(const std::string &expression) const
	{
		std::string source = "katex.renderToString('";
		
		source += _escape(expression);
		
		if (_flags.test(DisplayMode))
		{
			source += "', {'displayMode': true}";
		}
		
		return source +");";
	}
	
	std::string Math::_escape(std::string source) const
	{
		for (auto i = source.begin(); i != source.end(); ++i)
		{
			if (*i == '\\')
			{
				i = source.insert(i, '\\');
				
				++i;
			}
		}
		
		return source;
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
}