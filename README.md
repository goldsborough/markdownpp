# Markdownpp

[![GitHub license](https://img.shields.io/github/license/mashape/apistatus.svg?style=flat-square)](http://goldsborough.mit-license.org)

Markdownpp is a first-of-its-kind Markdown solution for C++. It unifies and handles all aspects of a modern, powerful and flexible Markdown tool including:

* Markdown conversion with a sweet object-oriented interface.
* __LaTeX__ integration for all things math.
* Syntax highlighting with [highlight.js](https://highlightjs.org) and its __65__ themes.
* __8__ Markdown themes, such as GitHub's or Solarized.
* A command-line program accompanying the library.

## Usage

```C++
#include "markdown-parser.hpp"

int main(int argc, const char* argv[])
{
	// Main interface
	Markdown::Parser parser;

	// Flexible key-value configuration
	parser.configure("markdown-style", "solarized-dark");

	// Quick and easy snippet-conversion, returns HTML as std::string
	// <p><em>hello</em></p>
	auto snippet = parser.snippet("*hello*");

	// 65 possible styles (using highlight.js)
	parser.configure("code-style", "monokai");

	// Full file rendering
	parser.render_file("input.md", "output.html");
}
```

or

```Bash
$ markdownpp -m solarized-dark -c monokai input.md output.html
```

## Demo

See this README rendered by __markdownpp__ with the *solarized-dark* markdown-theme and *xcode* syntax-theme [here](http://goldsborough.github.io/markdownpp/).

## Documentation

You can build extensive documentation with `doxygen`. See the `doxyfile` in the `docs/` folder. There are also some example programs in the `examples` folder.

## LICENSE

This project is released under the [MIT License](http://goldsborough.mit-license.org). For more information, see the `LICENSE` file.

## Authors

[Peter Goldsborough](http://goldsborough.me) + [cat](https://goo.gl/IpUmJn) :heart:

<a href="https://gratipay.com/~goldsborough/"><img src="http://img.shields.io/gratipay/goldsborough.png?style=flat-square"></a>
