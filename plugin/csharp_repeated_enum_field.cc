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
#include <google/protobuf/wire_format.h>

#include <csharp_helpers.h>
#include <csharp_repeated_enum_field.h>
#include <csharp_writer.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace csharp {

RepeatedEnumFieldGenerator::RepeatedEnumFieldGenerator(
    const FieldDescriptor* descriptor, int fieldOrdinal)
    : FieldGeneratorBase(descriptor, fieldOrdinal) {
}

RepeatedEnumFieldGenerator::~RepeatedEnumFieldGenerator() {

}

void RepeatedEnumFieldGenerator::GenerateMembers(Writer* writer) {
  AddDeprecatedFlag(writer);
  writer->WriteLine("public $0$[] $1$;", type_name(),
                    property_name());
}

void RepeatedEnumFieldGenerator::GenerateParsingCode(Writer* writer) {
  writer->WriteLine(
      "input.ReadEnumArray<$0$>(tag, this.$1$);",
      type_name(), name());
}

void RepeatedEnumFieldGenerator::GenerateSerializationCode(Writer* writer) {
  writer->WriteLine("if ($0$.Count > 0) {", name());
  writer->Indent();
  if (descriptor_->is_packed()) {
    writer->WriteLine(
        "output.WritePackedEnumArray($0$, $1$);",
        number(), name(), field_ordinal());
  } else {
    writer->WriteLine("output.WriteEnumArray($0$, $1$);",
                      number(), name(), field_ordinal());
  }
  writer->Outdent();
  writer->WriteLine("}");
}

void RepeatedEnumFieldGenerator::GenerateSerializedSizeCode(Writer* writer) {
  writer->WriteLine("{");
  writer->Indent();
  writer->WriteLine("int dataSize = 0;");
  writer->WriteLine("if ($0$.Count > 0) {", name());
  writer->Indent();
  writer->WriteLine("foreach ($0$ element in $1$) {", type_name(), name());
  writer->WriteLine(
      "  dataSize += pb::CodedOutputStream.ComputeEnumSizeNoTag((int) element);");
  writer->WriteLine("}");
  writer->WriteLine("size += dataSize;");
  int tagSize = internal::WireFormat::TagSize(descriptor_->number(), descriptor_->type());
  if (descriptor_->is_packed()) {
    writer->WriteLine("size += $0$;", SimpleItoa(tagSize));
    writer->WriteLine(
        "size += pb::CodedOutputStream.ComputeRawVarint32Size((uint) dataSize);");
  } else {
    writer->WriteLine("size += $0$ * $1$.Count;", SimpleItoa(tagSize), name());
  }
  writer->Outdent();
  writer->WriteLine("}");
  // cache the data size for packed fields.
  writer->Outdent();
  writer->WriteLine("}");
}

void RepeatedEnumFieldGenerator::GenerateInitCode(Writer* writer) {
}	

void RepeatedEnumFieldGenerator::GenerateFinishCode(Writer* writer) {
  writer->WriteLine("if( $0$ == null ){", property_name());
  writer->WriteLine("  $0$ = new List<$1$>();", property_name(), type_name());
  writer->WriteLine("}");
}	

}  // namespace csharp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
