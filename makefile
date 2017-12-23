CPPFLAGS=-g --std=c++11

test_maker: test_maker.o
	g++ $(CPPFLAGS) -o test_maker test_maker.o

test_maker.o: parser/parser.h.gch test_maker.cpp
	g++ $(CPPFLAGS) -c test_maker.cpp

parser/parser.h.gch: parser/parser.h
	g++ $(CPPFLAGS) parser/parser.h

clean:
	rm parser/parser.h.gch
	rm test_maker.o
