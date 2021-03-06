// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <sstream>

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include <csharp_helpers.h>
#include <csharp_repeated_message_field.h>
#include <csharp_writer.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace csharp {

RepeatedMessageFieldGenerator::RepeatedMessageFieldGenerator(
    const FieldDescriptor* descriptor, int fieldOrdinal)
    : FieldGeneratorBase(descriptor, fieldOrdinal) {
}

RepeatedMessageFieldGenerator::~RepeatedMessageFieldGenerator() {

}

void RepeatedMessageFieldGenerator::GenerateMembers(Writer* writer) {
  writer->WriteLine(
      "public List<$0$> $1$ = new List<$0$>();",
      type_name(), name());
  AddDeprecatedFlag(writer);
}

void RepeatedMessageFieldGenerator::GenerateParsingCode(Writer* writer) {
  writer->WriteLine(
      "input.Read$0$Array(tag, this.$1$, $2$.CreateEmpty);",
      message_or_group(), name(), type_name());
}

void RepeatedMessageFieldGenerator::GenerateSerializationCode(Writer* writer) {
  writer->WriteLine("if ($0$ != null && $0$.Count > 0) {", name());
  writer->Indent();
  writer->WriteLine("output.Write$0$Array($1$, $2$);",
                    message_or_group(), number(), name(), field_ordinal());
  writer->Outdent();
  writer->WriteLine("}");
}

void RepeatedMessageFieldGenerator::GenerateSerializedSizeCode(Writer* writer) {
  writer->WriteLine("if( $0$ != null ) {", property_name());
  writer->Indent();
  writer->WriteLine("foreach ($0$ element in $1$) {", type_name(), property_name());
  writer->WriteLine("  size += pb::CodedOutputStream.Compute$0$Size($1$, element);", message_or_group(), number());
  writer->WriteLine("}");
  writer->Outdent();
  writer->WriteLine("}");
}

void RepeatedMessageFieldGenerator::GenerateInitCode(Writer* writer) {
}	

void RepeatedMessageFieldGenerator::GenerateFinishCode(Writer* writer) {
  writer->WriteLine("if( $0$ == null ){", property_name());
  writer->WriteLine("  $0$ = new List<$1$>();", property_name(), type_name());
  writer->WriteLine("}");
}	

}  // namespace csharp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
