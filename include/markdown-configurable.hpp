#ifndef MARKDOWN_CONFIGURABLE_HPP
#define MARKDOWN_CONFIGURABLE_HPP

#include <bitset>

namespace Markdown
{
	template<std::size_t N>
	class Configurable
	{
	public:
		
		using flags_t = std::bitset<N>;
		
		Configurable(const flags_t& flags = flags_t())
		: _flags(flags)
		{ }
		
		virtual ~Configurable() = default;
		
		
		virtual void configure(const flags_t& flags)
		{
			_flags = flags;
		}
		
		virtual const flags_t& flags() const
		{
			return _flags;
		}
		
	protected:
		
		flags_t _flags;
	};
}

#endif /* MARKDOWN_CONFIGURABLE_HPP */