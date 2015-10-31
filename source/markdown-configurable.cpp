#include "markdown-configurable.hpp"

namespace Markdown
{	
	Configurable::Configurable(const settings_t& settings)
	: _settings(settings)
	{ }
	
	Configurable::~Configurable() = default;
	
	void Configurable::configure(const std::string &key,
								 const std::string &value)
	{
		_get(key) = value;
	}
	
	void Configurable::configure(const std::string &key,
								 const char *value)
	{
		configure(key, std::string(value));
	}
	
	std::string& Configurable::operator[](const std::string &key)
	{
		return _get(key);
	}
	
	const std::string& Configurable::operator[](const std::string &key) const
	{
		return _get(key);
	}
	
	void Configurable::settings(const settings_t &settings)
	{
		_settings = settings;
	}
	
	const Configurable::settings_t& Configurable::settings() const
	{
		return _settings;
	}
	
	std::string& Configurable::_get(const std::string& key)
	{
		auto setting = _settings.find(key);
		
		if (setting == _settings.end())
		{
			throw ConfigurationKeyException(key);
		}
		
		return setting->second;
	}
	
	const std::string& Configurable::_get(const std::string& key) const
	{
		auto setting = _settings.find(key);
		
		if (setting == _settings.end())
		{
			throw ConfigurationKeyException(key);
		}
		
		return setting->second;
	}
}