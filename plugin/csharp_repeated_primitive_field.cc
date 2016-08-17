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
#include <csharp_repeated_primitive_field.h>
#include <csharp_writer.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace csharp {

RepeatedPrimitiveFieldGenerator::RepeatedPrimitiveFieldGenerator(
    const FieldDescriptor* descriptor, int fieldOrdinal)
    : FieldGeneratorBase(descriptor, fieldOrdinal) {
}

RepeatedPrimitiveFieldGenerator::~RepeatedPrimitiveFieldGenerator() {

}

void RepeatedPrimitiveFieldGenerator::GenerateMembers(Writer* writer) {
  AddPublicMemberAttributes(writer);
  writer->WriteLine("public List<$0$> $1$ = new List<$0$>();", type_name(),
                    property_name());
}

void RepeatedPrimitiveFieldGenerator::GenerateParsingCode(Writer* writer) {
  writer->WriteLine("input.Read$0$Array(tag, this.$1$);",
                    capitalized_type_name(), name());
}

void RepeatedPrimitiveFieldGenerator::GenerateSerializationCode(
    Writer* writer) {
  writer->WriteLine("if ($0$.Count > 0) {", name());
  writer->Indent();
  if (descriptor_->is_packed()) {
    writer->WriteLine(
        "output.WritePacked$0$Array($1$, $2$);",
        capitalized_type_name(), number(), name(), field_ordinal());
  } else {
    writer->WriteLine("output.Write$0$Array($1$, $2$);",
                      capitalized_type_name(), number(), name(),
                      field_ordinal());
  }
  writer->Outdent();
  writer->WriteLine("}");
}

void RepeatedPrimitiveFieldGenerator::GenerateSerializedSizeCode(
    Writer* writer) {
  writer->WriteLine("{");
  writer->Indent();
  writer->WriteLine("int dataSize = 0;");
  int fixedSize = GetFixedSize(descriptor_->type());
  if (fixedSize == -1) {
    writer->WriteLine("foreach ($0$ element in $1$) {", type_name(),
                      property_name());
    writer->WriteLine(
        "  dataSize += pb::CodedOutputStream.Compute$0$SizeNoTag(element);",
        capitalized_type_name(), number());
    writer->WriteLine("}");
  } else {
    writer->WriteLine("dataSize = $0$ * $1$.Count;", SimpleItoa(fixedSize), name());
  }
  writer->WriteLine("size += dataSize;");
  int tagSize = internal::WireFormat::TagSize(descriptor_->number(), descriptor_->type());
  if (descriptor_->is_packed()) {
    writer->WriteLine("if ($0$.Count != 0) {", name());
    writer->WriteLine(
        "  size += $0$ + pb::CodedOutputStream.ComputeInt32SizeNoTag(dataSize);",
        SimpleItoa(tagSize));
    writer->WriteLine("}");
  } else {
    writer->WriteLine("size += $0$ * $1$.Count;", SimpleItoa(tagSize), name());
  }
  writer->Outdent();
  writer->WriteLine("}");
}

void RepeatedPrimitiveFieldGenerator::GenerateInitCode(Writer* writer) {
}	

void RepeatedPrimitiveFieldGenerator::GenerateFinishCode(Writer* writer) {
  writer->WriteLine("if( $0$ == null ){", property_name());
  writer->WriteLine("  $0$ = new List<$1$>();", property_name(), type_name());
  writer->WriteLine("}");
}	
	

}  // namespace csharp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
