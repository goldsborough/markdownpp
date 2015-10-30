#ifndef MARKDOWN_CONFIGURABLE_HPP
#define MARKDOWN_CONFIGURABLE_HPP

#include <string>
#include <sstream>
#include <unordered_map>

namespace Markdown
{
	class Configurable
	{
	public:
		
		using settings_t = std::unordered_map<std::string, std::string>;
		
		struct KeyException : public std::invalid_argument
		{
			KeyException(const std::string& what)
			: std::invalid_argument(what)
			{ }
		};
		
		Configurable(const settings_t& settings);
		
		Configurable(const Configurable& other) = default;
		
		Configurable(Configurable&& other) noexcept = default;
		
		Configurable& operator=(const Configurable& other) = default;
		
		virtual ~Configurable() = default;
		
		
		virtual void configure(const std::string& key,
							   const std::string& value);
		
		template<typename T>
		void configure(const std::string& key, const T& value)
		{
			configure(key, std::to_string(value));
		}

		
		virtual std::string& operator[](const std::string& key);
		
		virtual const std::string& operator[](const std::string& key) const;
		
		
		template<typename R = std::string>
		R get(const std::string& key) const
		{
			auto setting = _settings.find(key);
			
			if (setting == _settings.end())
			{
				throw KeyException("No such key '" + key + "'!");
			}
			
			std::istringstream stream(setting->second);
			
			R value;
			
			stream >> value;
			
			return value;
		}
		
		
		virtual void settings(const settings_t& settings);
		
		virtual const settings_t& settings() const;
		
	protected:
		
		settings_t _settings;
	};
}

#endif /* MARKDOWN_CONFIGURABLE_HPP */