# SMCS=/Applications/Unity5.3.4p3/Unity.app/Contents/Frameworks/Mono/bin/smcs
SMCS=mcs

test:
	$(SMCS) cs_test/*.cs

dll:
	$(SMCS) cs_lib/*.cs -target:library -out:GoogleProtocolBuffer.dll

