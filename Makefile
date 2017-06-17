# SMCS=/Applications/Unity5.3.4p3/Unity.app/Contents/Frameworks/Mono/bin/smcs
SMCS2=/Applications/Unity5.5.0p1/Unity.app/Contents/Mono/bin/smcs
SMCS=mcs

test:
	$(SMCS) cs_test/*.cs

dll:
	$(SMCS) cs_lib/*.cs -target:library -out:GoogleProtocolBuffer-net45.dll
	$(SMCS2) cs_lib/*.cs -target:library -out:GoogleProtocolBuffer.dll

