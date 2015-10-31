```C++
std::string Math::_get_javascript(const std::string &expression,
								  bool display_math) const
{
	std::string source = "katex.renderToString('";

	source += _escape(expression) + "', {'displayMode': ";

	// all-display-math | display_math | result
	// 		 0		    |	  0		   |   0
	// 		 0		    |	  1		   |   1
	// 		 1		    |	  0		   |   1
	// 		 1		    |	  1		   |   1
	display_math |= Configurable::get<bool>("all-display-math");

	source += display_math ? "true" : "false";

	return source + "});";
}
```
