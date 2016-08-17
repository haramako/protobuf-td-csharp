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
#include <algorithm>

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <strutil.h>
#include <google/protobuf/wire_format.h>
#include <google/protobuf/wire_format_lite.h>

#include <csharp_enum.h>
#include <csharp_extension.h>
#include <csharp_message.h>
#include <csharp_helpers.h>
#include <csharp_field_base.h>
#include <csharp_writer.h>

using google::protobuf::internal::scoped_ptr;

namespace google {
namespace protobuf {
namespace compiler {
namespace csharp {

bool CompareFieldNumbers(const FieldDescriptor* d1, const FieldDescriptor* d2) {
  return d1->number() < d2->number();
}

MessageGenerator::MessageGenerator(const Descriptor* descriptor)
    : SourceGeneratorBase(descriptor->file()),
      descriptor_(descriptor) {

  // sorted field names
  for (int i = 0; i < descriptor_->field_count(); i++) {
    field_names_.push_back(descriptor_->field(i)->name());
  }
  std::sort(field_names_.begin(), field_names_.end());

  // fields by number
  for (int i = 0; i < descriptor_->field_count(); i++) {
    fields_by_number_.push_back(descriptor_->field(i));
  }
  std::sort(fields_by_number_.begin(), fields_by_number_.end(),
            CompareFieldNumbers);
}

MessageGenerator::~MessageGenerator() {
}

std::string MessageGenerator::class_name() {
  return descriptor_->name();
}

std::string MessageGenerator::full_class_name() {
  return GetClassName(descriptor_);
}

const std::vector<std::string>& MessageGenerator::field_names() {
  return field_names_;
}

const std::vector<const FieldDescriptor*>& MessageGenerator::fields_by_number() {
  return fields_by_number_;
}

/// Get an identifier that uniquely identifies this type within the file.
/// This is used to declare static variables related to this type at the
/// outermost file scope.
std::string GetUniqueFileScopeIdentifier(const Descriptor* descriptor) {
  std::string result = descriptor->full_name();
  std::replace(result.begin(), result.end(), '.', '_');
  return "static_" + result;
}

void MessageGenerator::GenerateStaticVariables(Writer* writer) {
  // Because descriptor.proto (Google.ProtocolBuffers.DescriptorProtos) is
  // used in the construction of descriptors, we have a tricky bootstrapping
  // problem.  To help control static initialization order, we make sure all
  // descriptors and other static data that depends on them are members of
  // the proto-descriptor class.  This way, they will be initialized in
  // a deterministic order.

  std::string identifier = GetUniqueFileScopeIdentifier(descriptor_);

  for (int i = 0; i < descriptor_->nested_type_count(); i++) {
    MessageGenerator messageGenerator(descriptor_->nested_type(i));
    messageGenerator.GenerateStaticVariables(writer);
  }
}

void MessageGenerator::GenerateStaticVariableInitializers(Writer* writer) {
  std::string identifier = GetUniqueFileScopeIdentifier(descriptor_);

  // Generate static member initializers for all nested types.
  for (int i = 0; i < descriptor_->nested_type_count(); i++) {
    MessageGenerator messageGenerator(descriptor_->nested_type(i));
    messageGenerator.GenerateStaticVariableInitializers(writer);
  }

}

void MessageGenerator::Generate(Writer* writer) {
  writer->WriteLine(
      "[global::System.Diagnostics.DebuggerNonUserCodeAttribute()]");
  WriteGeneratedCodeAttributes(writer);
  writer->WriteLine(
      "$0$ sealed partial class $1$ : pb.Message {",
      class_access_level(), class_name());
  writer->Indent();
  writer->WriteLine("private $0$() { }", class_name());
  writer->WriteLine("public static $0$ CreateInstance() { var obj = new $0$(); obj.Finish(); return obj; }", class_name());
  writer->WriteLine("public static $0$ CreateEmpty() { return new $0$(); }", class_name());
  // Must call MakeReadOnly() to make sure all lists are made read-only
  writer->WriteLine(
      "private static readonly $0$ defaultInstance = new $0$();",
      class_name());

  writer->WriteLine("public static $0$ DefaultInstance {", class_name());
  writer->WriteLine("  get { return defaultInstance; }");
  writer->WriteLine("}");
  writer->WriteLine();

  if (descriptor_->enum_type_count() + descriptor_->nested_type_count() > 0) {
    writer->WriteLine("#region Nested types");
    writer->WriteLine(
        "[global::System.Diagnostics.DebuggerNonUserCodeAttribute()]");
    WriteGeneratedCodeAttributes(writer);
    writer->WriteLine("public static partial class Types {");
    writer->Indent();
    for (int i = 0; i < descriptor_->enum_type_count(); i++) {
      EnumGenerator enumGenerator(descriptor_->enum_type(i));
      enumGenerator.Generate(writer);
    }
    for (int i = 0; i < descriptor_->nested_type_count(); i++) {
      MessageGenerator messageGenerator(descriptor_->nested_type(i));
      messageGenerator.Generate(writer);
    }
    writer->Outdent();
    writer->WriteLine("}");
    writer->WriteLine("#endregion");
    writer->WriteLine();
  }

  for (int i = 0; i < descriptor_->field_count(); i++) {
    const FieldDescriptor* fieldDescriptor = descriptor_->field(i);

    // Rats: we lose the debug comment here :(
    scoped_ptr<FieldGeneratorBase> generator(
        CreateFieldGeneratorInternal(fieldDescriptor));
    generator->GenerateMembers(writer);
    writer->WriteLine();
  }

  GenerateLiteRuntimeMethods(writer);

  GenerateMessageSerializationMethods(writer);
  GenerateParseFromMethods(writer);
  GenerateBuilder(writer);
  GenerateInitMethods(writer);

  writer->Outdent();
  writer->WriteLine("}");
  writer->WriteLine();
}

void MessageGenerator::GenerateLiteRuntimeMethods(Writer* writer) {
  writer->WriteLine("#region Lite runtime methods");
#if 0
  writer->WriteLine(
      "public override void PrintTo(global::System.IO.TextWriter writer) {");
  writer->Indent();

  for (int i = 0; i < fields_by_number().size(); i++) {
    scoped_ptr<FieldGeneratorBase> generator(
        CreateFieldGeneratorInternal(fields_by_number()[i]));
    generator->WriteToString(writer);
  }

  if (callbase) {
    writer->WriteLine("base.PrintTo(writer);");
  }
  writer->Outdent();
  writer->WriteLine("}");
#endif  
  writer->WriteLine("#endregion");
  writer->WriteLine();
}

bool CompareExtensionRangesStart(const Descriptor::ExtensionRange* r1,
                                 const Descriptor::ExtensionRange* r2) {
  return r1->start < r2->start;
}

void MessageGenerator::GenerateMessageSerializationMethods(Writer* writer) {
  std::vector<const Descriptor::ExtensionRange*> extension_ranges_sorted;
  for (int i = 0; i < descriptor_->extension_range_count(); i++) {
    extension_ranges_sorted.push_back(descriptor_->extension_range(i));
  }
  std::sort(extension_ranges_sorted.begin(), extension_ranges_sorted.end(),
            CompareExtensionRangesStart);

  writer->WriteLine(
      "public override void WriteTo(pb::CodedOutputStream output) {");
  writer->Indent();
  // Make sure we've computed the serialized length, so that packed fields are generated correctly.
  writer->WriteLine("CalcSerializedSize();");
  if (descriptor_->extension_range_count()) {
    writer->WriteLine(
        "pb::ExtendableMessage$1$<$0$, $0$.Builder>.ExtensionWriter extensionWriter = CreateExtensionWriter(this);",
        class_name(), runtime_suffix());
  }

  // Merge the fields and the extension ranges, both sorted by field number.
  for (int i = 0, j = 0;
      i < fields_by_number().size() || j < extension_ranges_sorted.size();) {
    if (i == fields_by_number().size()) {
		//GenerateSerializeOneExtensionRange(writer, extension_ranges_sorted[j++]);
    } else if (j == extension_ranges_sorted.size()) {
      GenerateSerializeOneField(writer, fields_by_number()[i++]);
    } else if (fields_by_number()[i]->number()
        < extension_ranges_sorted[j]->start) {
      GenerateSerializeOneField(writer, fields_by_number()[i++]);
    } else {
		//GenerateSerializeOneExtensionRange(writer, extension_ranges_sorted[j++]);
    }
  }

  writer->Outdent();
  writer->WriteLine("}");
  writer->WriteLine();
  writer->WriteLine("public override int SerializedSize {");
  writer->Indent();
  writer->WriteLine("get {");
  writer->Indent();
  writer->WriteLine("return CalcSerializedSize();");
  writer->Outdent();
  writer->WriteLine("}");
  writer->Outdent();
  writer->WriteLine("}");
  writer->WriteLine();

  writer->WriteLine("private int CalcSerializedSize() {");
  writer->Indent();
  writer->WriteLine("int size = 0;");
  for (int i = 0; i < descriptor_->field_count(); i++) {
    scoped_ptr<FieldGeneratorBase> generator(
        CreateFieldGeneratorInternal(descriptor_->field(i)));
    generator->GenerateSerializedSizeCode(writer);
  }
  writer->WriteLine("return size;");
  writer->Outdent();
  writer->WriteLine("}");
  
}

void MessageGenerator::GenerateSerializeOneField(
    Writer* writer, const FieldDescriptor* fieldDescriptor) {
  scoped_ptr<FieldGeneratorBase> generator(
      CreateFieldGeneratorInternal(fieldDescriptor));
  generator->GenerateSerializationCode(writer);
}

void MessageGenerator::GenerateParseFromMethods(Writer* writer) {
  // Note:  These are separate from GenerateMessageSerializationMethods()
  //   because they need to be generated even for messages that are optimized
  //   for code size.

  writer->WriteLine("public static $0$ ParseFrom(byte[] data) {", class_name());
  writer->WriteLine("  var mes = CreateInstance(); mes.MergeFrom(data); return mes;");
  writer->WriteLine("}");
  writer->WriteLine("public static $0$ ParseFrom(global::System.IO.Stream input) {", class_name());
  writer->WriteLine("  var mes = CreateInstance(); mes.MergeFrom(input); return mes;");
  writer->WriteLine("}");
  writer->WriteLine("public static $0$ ParseFrom(pb::CodedInputStream input) {", class_name());
  writer->WriteLine("  var mes = CreateInstance(); mes.MergeFrom(input); return mes;");
  writer->WriteLine("}");
}

void MessageGenerator::GenerateBuilder(Writer* writer) {
  GenerateCommonBuilderMethods(writer);
  GenerateBuilderParsingMethods(writer);
}

	
void MessageGenerator::GenerateCommonBuilderMethods(Writer* writer) {
}

void MessageGenerator::GenerateBuilderParsingMethods(Writer* writer) {
  writer->WriteLine("public override void MergeFrom(pb::CodedInputStream input) {", class_name());
  writer->Indent();
  writer->WriteLine("uint tag;");
  writer->WriteLine("while (input.ReadTag(out tag)) {");
  writer->Indent();

  writer->WriteLine("switch (tag) {");
  writer->Indent();
  writer->WriteLine("case 0: {");  // 0 signals EOF / limit reached
  writer->WriteLine("  throw pb::InvalidProtocolBufferException.InvalidTag();");
  writer->WriteLine("}");
  writer->WriteLine("default: {");
  writer->WriteLine("  if (pb::WireFormat.IsEndGroupTag(tag)) {");
  writer->WriteLine("    return;");  // it's an endgroup tag
  writer->WriteLine("  }");
  writer->WriteLine("  break;");
  writer->WriteLine("}");

  for (int i = 0; i < fields_by_number().size(); i++) {
    const FieldDescriptor* field = fields_by_number()[i];
    internal::WireFormatLite::WireType wt =
        internal::WireFormat::WireTypeForFieldType(field->type());
    uint32 tag = internal::WireFormatLite::MakeTag(field->number(), wt);
    if (field->is_repeated()
        && (wt == internal::WireFormatLite::WIRETYPE_VARINT
            || wt == internal::WireFormatLite::WIRETYPE_FIXED32
            || wt == internal::WireFormatLite::WIRETYPE_FIXED64)) {
      writer->WriteLine(
          "case $0$:",
          SimpleItoa(
              internal::WireFormatLite::MakeTag(
                  field->number(),
                  internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED)));
    }

    writer->WriteLine("case $0$: {", SimpleItoa(tag));
    writer->Indent();
    scoped_ptr<FieldGeneratorBase> generator(
        CreateFieldGeneratorInternal(field));
    generator->GenerateParsingCode(writer);
    writer->WriteLine("break;");
    writer->Outdent();
    writer->WriteLine("}");
  }

  writer->Outdent();
  writer->WriteLine("}");
  writer->Outdent();
  writer->WriteLine("}");
  writer->Outdent();
  writer->WriteLine("}");
  writer->WriteLine();
}

void MessageGenerator::GenerateInitMethods(Writer* writer) {
	
	writer->WriteLine("public override void Init() {");
	for (int i = 0; i < descriptor_->field_count(); i++) {
		scoped_ptr<FieldGeneratorBase> generator(
            CreateFieldGeneratorInternal(descriptor_->field(i)));
		generator->GenerateInitCode(writer);
	}
	writer->WriteLine("}");
	
	writer->WriteLine("public override void Finish() {");
	for (int i = 0; i < descriptor_->field_count(); i++) {
		scoped_ptr<FieldGeneratorBase> generator(
            CreateFieldGeneratorInternal(descriptor_->field(i)));
		generator->GenerateInitCode(writer);
	}
	writer->WriteLine("}");
}

void MessageGenerator::GenerateExtensionRegistrationCode(Writer* writer) {
  for (int i = 0; i < descriptor_->nested_type_count(); i++) {
    MessageGenerator messageGenerator(descriptor_->nested_type(i));
    messageGenerator.GenerateExtensionRegistrationCode(writer);
  }
}

int MessageGenerator::GetFieldOrdinal(const FieldDescriptor* descriptor) {
  for (int i = 0; i < field_names().size(); i++) {
    if (field_names()[i] == descriptor->name()) {
      return i;
    }
  }
  GOOGLE_LOG(DFATAL)<< "Could not find ordinal for field " << descriptor->name();
  return -1;
}

FieldGeneratorBase* MessageGenerator::CreateFieldGeneratorInternal(
    const FieldDescriptor* descriptor) {
  return CreateFieldGenerator(descriptor, GetFieldOrdinal(descriptor));
}

}  // namespace csharp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
