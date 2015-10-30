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
		
		struct ConfigurationKeyException : public std::invalid_argument
		{
			ConfigurationKeyException(const std::string& key)
			: std::invalid_argument("No such key '" + key + "'!")
			{ }
		};
		
		struct ConfigurationValueException : public std::invalid_argument
		{
			ConfigurationValueException(const std::string& key,
										const std::string& value)
			: std::invalid_argument("Invalid value '" + value + 
									"' for key '" + key + "'!")
			{ }
		};
		
		Configurable(const settings_t& settings);
		
		Configurable(const Configurable& other) = default;
		
		Configurable(Configurable&& other) noexcept = default;
		
		Configurable& operator=(const Configurable& other) = default;
		
		virtual ~Configurable() = default;
		
		
		virtual void configure(const std::string& key,
							   const std::string& value);
		
		virtual void configure(const std::string& key,
							   const char* value);
		
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
			std::istringstream stream(_get(key));
			
			R value;
			
			stream >> value;
			
			return value;
		}
		
		
		virtual void settings(const settings_t& settings);
		
		virtual const settings_t& settings() const;
		
	protected:
		
		std::string& _get(const std::string& key);
		
		const std::string& _get(const std::string& key) const;
		
		settings_t _settings;
	};
}

#endif /* MARKDOWN_CONFIGURABLE_HPP */