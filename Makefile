CXX			:= c++
CXXFLAGS	:= -std=c++1y -stdlib=libc++

INCLUDES := -I/usr/local/Cellar/v8/4.5.103.35 -I/usr/local/Cellar/v8/4.5.103.35/include -I/usr/local/Cellar/v8/4.5.103.35/include/libplatform -I/usr/local/include -Iinclude -I/usr/local/Cellar/boost/include

LIBS :=  -L/usr/local/Cellar/v8/4.5.103.35/lib -L/usr/local/lib -L/usr/local/Cellar/boost/1.58.0/lib -lboost_system -lboost_filesystem -lboost_program_options -lv8_nosnapshot -lv8_snapshot -lv8_base -lv8_libbase -lv8_libplatform -lv8 -lhoedown

OBJECTS := main.o markdown-parser.o markdown-configurable.o markdown-markdown.o markdown-math.o markdown-abstract-math.o markdown-abstract-markdown.o

build: $(OBJECTS)
	$(MAKE) markdownpp
	$(MAKE) clean

markdownpp: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o markdownpp $(LIBS)

markdown-parser.o: source/markdown-parser.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c source/markdown-parser.cpp -o markdown-parser.o

markdown-configurable.o: source/markdown-configurable.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c source/markdown-configurable.cpp -o markdown-configurable.o

markdown-markdown.o: source/markdown-markdown.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c source/markdown-markdown.cpp -o markdown-markdown.o

markdown-math.o: source/markdown-math.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c source/markdown-math.cpp -o markdown-math.o

markdown-abstract-math.o: source/markdown-abstract-math.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c source/markdown-abstract-math.cpp -o markdown-abstract-math.o

markdown-abstract-markdown.o: source/markdown-abstract-markdown.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c source/markdown-abstract-markdown.cpp -o markdown-abstract-markdown.o

main.o: main.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c main.cpp -o main.o

clean:
	rm -f *.o

reset:
	$(MAKE) clean
	rm -f *.html
	rm -f markdownpp

.PHONY: clean reset
