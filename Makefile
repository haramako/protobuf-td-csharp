SMCS=/Applications/Unity5.3.4p3/Unity.app/Contents/Frameworks/Mono/bin/smcs

test:
	$(SMCS) *.cs

dll:
	$(SMCS) cs_lib/*.cs -target:library -out:GoogleProtocolBuffer.dll

