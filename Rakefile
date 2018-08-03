SMCS = "C:/Windows/Microsoft.NET/Framework/v4.0.30319/csc.exe"

task :dll do
	sh SMCS, "-target:library", "-out:GoogleProtocolBuffer.dll", *FileList["cs_lib/*.cs"].map{|s| s.gsub('/','\\')}
end

