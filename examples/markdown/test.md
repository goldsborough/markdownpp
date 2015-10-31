## Markdown test file (https://github.com/suan/github-flavored-markdown-test)

### This README can be used as a reference for github-flavored-markdown styling (and possibly behavior).

---

# H1
followed by some text

## H2
followed by some text

### H3
followed by some text

#### H4
followed by some text

##### H5
followed by some text

###### H6
followed by some text

Auto-detected link: http://www.france.com

Some Ignored_multiple_underscore_italics here

A line of normal text with `inline code` and *italics*, **strong font**, and even some μ†ℱ ╋ℯ╳╋. Followed by lots of Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis id sem purus, eu commodo tortor. Donec malesuada ultricies dolor a eleifend. In hac habitasse platea dictumst. Vivamus a faucibus ligula. Nullam molestie tristique arcu, eu elementum metus ultricies sed. Aenean luctus congue lectus, vitae semper erat rhoncus non. Nulla facilisi.

Followed by another line of normal text with `inline code` and *italics*, **strong font**, and even some μ†ℱ ╋ℯ╳╋. Followed by lots of Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis id sem purus, eu commodo tortor. Donec malesuada ultricies dolor a eleifend. In hac habitasse platea dictumst. Vivamus a faucibus ligula. Nullam molestie tristique arcu, eu elementum metus ultricies sed. Aenean luctus congue lectus, vitae semper erat rhoncus non. Nulla facilisi.

Thin horizontal rule:

--

Thick horizontal rule:

------

|Table Header 1|Table Header 2           |
|--------------|-------------------------|
|Content       |http://example.org       |
|Content       |http://localhost:\<port\>|
Text right below table. Follows is a table with an empty cell, and unaligned indenting.

|Table Header 1|Table Header 2|
|--------------|--------------|
|Content  |  Cntent        |
|Content       |      |

Empty line between table and this text

    def this_is
      puts "some #{4-space-indent} code"
    end

<code>
def this_is
  puts "some #{code tag} code"
end
</code>

<pre>
def this_is
  puts "some #{pre tag} code"
end
</pre>

```
def this_is
  puts "some #{fenced} code"
end
```

```ruby
class Classy
  def this_is
    puts "some #{colored} ruby code with ruby syntax highlighting"
    @someobj.do_it(1, 2)
  end
end
```

```javascript
var test = function this_is(){
  console.log("some" + colored + "javascript code with javascript syntax highlighting really long");
}
```

```clojure
(defproject myproject "0.5.0-SNAPSHOT"
  :description "Some clojure code with syntax highlighting."
  :dependencies [[org.clojure/clojure "1.5.1"]]
  :plugins [[lein-tar "3.2.0"]])
```

```js
var test = function this_is(){
  console.log("language declared as 'js' instead");
}
```

```bogus_language
var test = function this_is(){
  console.log("language declared as bogus_language");
}
```

> here is blockquote

[Relative image link](afu.png)
