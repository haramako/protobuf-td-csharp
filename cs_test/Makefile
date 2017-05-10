MCS=mcs
MONO=mono

.PHONY: test make_test_data

test: test.dll test_data.bin 
	nunit-console4 -nologo test.dll 

test.dll: *.cs ../cs_lib/*.cs
	$(MCS) -debug -r:System.Runtime.Serialization.dll -r:nunit.framework.dll -out:$@ *.cs ../cs_lib/*.cs -t:library

test_data.bin: make_test_data.rb
	ruby make_test_data.rb > test_data.bin

