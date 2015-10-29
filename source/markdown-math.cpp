#include "markdown-math.hpp"

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
		static const std::string arguments = "{'displayMode': true}";
		
		v8::Isolate::Scope isolate_scope(_isolate);
		
		// Stack-allocated handle-scope (takes care of handles such
		// that object are garbage-collected after the scope ends)
		v8::HandleScope handle_scope(_isolate);
		
		// Get a local context handle from the persistent handle.
		auto context = v8::Local<v8::Context>::New(_isolate,
												   _persistent_context);
		
		v8::Context::Scope context_scope(context);
		
		std::string source = "katex.renderToString('";
		
		source += _escape(expression);
		
		if (_flags.test(DisplayMode))
		{
			source += "', " + arguments;
		}
		
		source += ");";
		
		auto value = _run(source, context);
		
		std::string html = *static_cast<v8::String::Utf8Value>(value);
		
		return "<div class='math'>\n" + html + "</div>\n";
	}
}