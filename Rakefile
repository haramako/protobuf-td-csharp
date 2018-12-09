def detect_path(pattern)
  files = FileList[pattern]
  if files.empty?
    puts "Warning: program not found #{pattern}"
    nil
  else
    file = files.select { |x| File.exist?(x) }.sort.last
    if file
      # puts "Detected #{file}"
      file
	else
	  puts "Warning: program not found #{pattern}"
	  nil
	end
  end
end

SMCS = "C:/Windows/Microsoft.NET/Framework/v4.0.30319/csc.exe"

# nunit3-console, install from https://github.com/nunit/nunit-console/releases
NUNIT3_CONSLE = "C:/Program Files (x86)/NUnit.org/nunit-console/nunit3-console.exe"

MSBUILD = detect_path([
  'C:/Program Files (x86)/Microsoft Visual Studio/2017/*/MSBuild/*/Bin/MSBuild.exe', # Visual Studio 2017 Professional
  'C:/Program Files (x86)/Microsoft Visual Studio/2017/BuildTools/MSBuild/*/Bin/MSBuild.exe', # choco install -y microsoft-build-tools
  'C:/Program Files (x86)/MSBuild/*/Bin/MSBuild.exe', # Visual Studio 2015
])



task :dll do
	sh SMCS, "-target:library", "-out:GoogleProtocolBuffer.dll", *FileList["cs_lib/*.cs"].map{|s| s.gsub('/','\\')}
end


task :test do
	Dir.chdir 'protoc-gen-tdcs' do
		sh 'go', 'build'
		sh 'protoc', '-I', '..', '--plugin=protoc-gen-tdcs.exe', '--tdcs_out=../cs_test', 'test.proto'
	end
	sh MSBUILD, 'cs_test/cs_test.csproj'
	sh NUNIT3_CONSLE, '--noheader', "cs_test/cs_test.csproj"
end
