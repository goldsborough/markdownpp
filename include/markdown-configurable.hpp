/***************************************************************************//*!
*
*	@file markdown-configurable.hpp
*
*	@author Peter Goldsborough.
*
*******************************************************************************/

#ifndef MARKDOWN_CONFIGURABLE_HPP
#define MARKDOWN_CONFIGURABLE_HPP

#include <string>
#include <sstream>
#include <unordered_map>

namespace Markdown
{
	/***********************************************************************//*!
	*
	*	@brief An abstract class for configurable objects.
	*
	***************************************************************************/

	class Configurable
	{
	public:
		
		/*! Data type used for storing settings. */
		using settings_t = std::unordered_map<std::string, std::string>;
		
		/*! An exception thrown when a requested key is not available. */
		struct ConfigurationKeyException : public std::invalid_argument
		{
			ConfigurationKeyException(const std::string& key)
			: std::invalid_argument("No such key '" + key + "'!")
			{ }
		};
		
		/*! An exception thrown when an invalid value is supplied for a key. */
		struct ConfigurationValueException : public std::invalid_argument
		{
			ConfigurationValueException(const std::string& key,
										const std::string& value)
			: std::invalid_argument("Invalid value '" + value + 
									"' for key '" + key + "'!")
			{ }
		};
		
		/*******************************************************************//*!
		*
		*	@brief Initializes members of the Configurable class.
		*
		*	@param settings The configuration-settings for this object.
		*
		***********************************************************************/
		
		Configurable(const settings_t& settings);
		
		/*******************************************************************//*!
		*
		*	@brief Destructs members of a configurable object.
		*
		*	@details Also a hack for making Configurable an abstract class.
		*
		***********************************************************************/
		virtual ~Configurable() = 0;
		
		/*******************************************************************//*!
		*
		*	@brief Configures a key-value pair.
		*
		*	@param key The key to configure.
		*
		*	@param value The value for the key.
		*
		***********************************************************************/
		
		virtual void configure(const std::string& key,
							   const std::string& value);
		
		/*******************************************************************//*!
		*
		*	@brief Configures a key-value pair.
		*
		*	@details Necessary to route string literals to the overload
		*			 for std::string values and not the templated one
		*			 for numeric values.
		*
		*	@param key The key to configure.
		*
		*	@param value The value for the key.
		*
		***********************************************************************/
		
		virtual void configure(const std::string& key,
							   const char* value);
		
		/*******************************************************************//*!
		*
		*	@brief Configures a key-value pair.
		*
		*	@details Allows only numeric values.
		*
		*	@param key The key to configure.
		*
		*	@param value The value for the key.
		*
		***********************************************************************/
		
		template<typename T>
		void configure(const std::string& key, const T& value)
		{
			configure(key, std::to_string(value));
		}

		/*******************************************************************//*!
		*
		*	@brief Retrieves a value for a key.
		*
		*	@param key The key to configure.
		*
		*	@return The value, as an std::string.
		*
		***********************************************************************/
		
		virtual std::string& operator[](const std::string& key);
		
		/*******************************************************************//*!
		*
		*	@brief Retrieves a value for a key.
		*
		*	@details const version.
		*
		*	@param key The key to configure.
		*
		*	@return The value, as an std::string.
		*
		***********************************************************************/
		
		virtual const std::string& operator[](const std::string& key) const;
		
		/*******************************************************************//*!
		*
		*	@brief Retrieves a value of a given type for a key.
		*
		*	@details Example: `bool truth = object.get<bool>("x-enabled")`
		*
		*	@param key The key to configure.
		*
		*	@tparam	R The type with which to return the value.
		*
		*	@return A value with the specified type.
		*
		***********************************************************************/
		
		template<typename R = std::string>
		R get(const std::string& key) const
		{
			std::istringstream stream(_get(key));
			
			R value;
			
			stream >> value;
			
			return value;
		}
		
		/*******************************************************************//*!
		*
		*	@brief Sets the settings entirely.
		*
		*	@param settings The new settings.
		*
		***********************************************************************/
		
		virtual void settings(const settings_t& settings);
		
		/*******************************************************************//*!
		*
		*	@brief Returns the current settings entirely.
		*
		***********************************************************************/
		
		virtual const settings_t& settings() const;
		
	protected:
		
		/*******************************************************************//*!
		*
		*	@brief Gets a value with existance-checking.
		*
		*	@param key The key to get the value for.
		*
		*	@throws ConfigurationKeyException if the key is
		*			not in the settings table.
		*
		***********************************************************************/
		
		std::string& _get(const std::string& key);
		
		/*******************************************************************//*!
		*
		*	@brief Gets a value with existance-checking.
		*
		*	@details const version.
		*
		*	@param key The key to get the value for.
		*
		*	@throws ConfigurationKeyException if the key is
		*			not in the settings table.
		*
		***********************************************************************/
		
		const std::string& _get(const std::string& key) const;
		
		/*! The settings. */
		settings_t _settings;
	};
}

#endif /* MARKDOWN_CONFIGURABLE_HPP */