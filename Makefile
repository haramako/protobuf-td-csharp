CXX=g++
SRC=$(wildcard *.cc) 
OBJ=$(SRC:%.cc=%.o)
CXXFLAGS=-O2 -I .
LDFLAGS=-l protobuf -l protoc
PROG=protoc-gen-dfcsharp
SMCS=/Applications/Unity5.3.4p3/Unity.app/Contents/Frameworks/Mono/bin/smcs

.PHONY: default test
default: $(PROG)

ifeq (,$(wildcard .depends))
	include .depends
endif

.cc.o:
	$(CXX) -c $< $(CXXFLAGS)


$(PROG): $(OBJ)
	$(CXX) -o $(PROG) $(CXXFLAGS) $(LDFLAGS) $(OBJ)

clean:
	rm -rf *.o $(PROG)

depends:
	$(CXX) -MM $(CXXFLAGS) $(SRC) > .depends

test:
	protoc --plugin=./protoc-gen-dfcsharp --dfcsharp_out=. master.proto
	$(SMCS) -r:ProtocolBuffers.dll Master.cs Test.cs

