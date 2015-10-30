#include "markdown-configurable.hpp"

namespace Markdown
{	
	Configurable::Configurable(const settings_t& settings)
	: _settings(settings)
	{ }
	
	void Configurable::configure(const std::string &key,
								 const std::string &value)
	{
		auto setting = _settings.find(key);
		
		if (setting == _settings.end())
		{
			throw KeyException("No such key '" + key + "'!");
		}
		
		setting->second = value;
	}
	
	std::string& Configurable::operator[](const std::string &key)
	{
		auto setting = _settings.find(key);
		
		if (setting == _settings.end())
		{
			throw KeyException("No such key '" + key + "'");
		}
		
		return setting->second;
	}
	
	const std::string& Configurable::operator[](const std::string &key) const
	{
		auto setting = _settings.find(key);
		
		if (setting == _settings.end())
		{
			throw KeyException("No such key '" + key + "'");
		}
		
		return setting->second;
	}
	
	void Configurable::settings(const settings_t &settings)
	{
		_settings = settings;
	}
	
	const Configurable::settings_t& Configurable::settings() const
	{
		return _settings;
	}
}