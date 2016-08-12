MCS=mcs
MONO=mono

.PHONY: test make_test_data

test: test.exe test_data.bin 
	$(MONO) test.exe test_data.bin

test.exe: *.cs ../cs_lib/*.cs
	$(MCS) -out:$@ *.cs ../cs_lib/*.cs

test_data.bin: make_test_data.rb
	ruby make_test_data.rb > test_data.bin

