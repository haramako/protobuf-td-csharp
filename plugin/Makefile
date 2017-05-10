CXX=g++
SRC=$(wildcard *.cc) 
OBJ=$(SRC:%.cc=%.o)
CXXFLAGS=-O2 -I . -I /usr/local/Homebrew/include
LDFLAGS=-l protobuf -l protoc -L /usr/local/Homebrew/lib
PROG=protoc-gen-dfcsharp
SMCS=/Applications/Unity5.3.4p3/Unity.app/Contents/Frameworks/Mono/bin/smcs

.PHONY: default test
default: $(PROG)

-include .depends

.cc.o:
	$(CXX) -c $< $(CXXFLAGS)

$(PROG): $(OBJ)
	$(CXX) -o $(PROG) $(CXXFLAGS) $(LDFLAGS) $(OBJ)

clean:
	rm -rf *.o $(PROG)

depends:
	$(CXX) -MM $(CXXFLAGS) $(SRC) > .depends

test:
	protoc --plugin=./protoc-gen-dfcsharp --dfcsharp_out=../cs_test/ --ruby_out=../cs_test test.proto
	make -C ../cs_test test
