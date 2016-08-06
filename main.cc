#include <google/protobuf/compiler/plugin.h>
#include <csharp_generator.h>

// entry point
int main(int argc, char* argv[]) {
	google::protobuf::compiler::csharp::Generator generator;
	return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
