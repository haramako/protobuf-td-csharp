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
#include <csharp_enum_field.h>
#include <csharp_writer.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace csharp {

EnumFieldGenerator::EnumFieldGenerator(const FieldDescriptor* descriptor,
                                       int fieldOrdinal)
    : FieldGeneratorBase(descriptor, fieldOrdinal) {
}

EnumFieldGenerator::~EnumFieldGenerator() {

}

void EnumFieldGenerator::GenerateMembers(Writer* writer) {
  writer->WriteLine("public $0$ $1$ = $2$;", type_name(), name(),
                    default_value());
  AddDeprecatedFlag(writer);
  AddPublicMemberAttributes(writer);
}

void EnumFieldGenerator::GenerateParsingCode(Writer* writer) {
  writer->WriteLine("input.ReadEnum(ref this.$0$);",
                    name());
}

void EnumFieldGenerator::GenerateSerializationCode(Writer* writer) {
  writer->WriteLine("if ($0$ != $1$) {", property_name(), default_value());
  writer->WriteLine(
      "  output.WriteEnum($0$, (int) $1$, $1$);", number(),
      property_name(), field_ordinal());
  writer->WriteLine("}");
}

void EnumFieldGenerator::GenerateSerializedSizeCode(Writer* writer) {
  writer->WriteLine("if ($0$ != $1$) {", property_name(), default_value());
  writer->WriteLine(
      "  size += pb::CodedOutputStream.ComputeEnumSize($0$, (int) $1$);",
      number(), property_name());
  writer->WriteLine("}");
}

void EnumFieldGenerator::GenerateInitCode(Writer* writer) {
}	

void EnumFieldGenerator::GenerateFinishCode(Writer* writer) {
}	
	
}  // namespace csharp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
