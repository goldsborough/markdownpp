```Python
def memoize(function):
	cache = {}
	def proxy(argument):
		if argument not in cache:
			cache[argument] = function(argument)
		return cache[argument]
	return proxy

@memoize
def fibonacci(n):
	if n <= 2:
		return n
	return fibonacci(n - 1) + fibonacci(n - 2)

if __name__ == '__main__':
	print(fibonacci(5))
```
