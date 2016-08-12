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

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#include <algorithm>
#include <google/protobuf/stubs/hash.h>
#include <limits>
#include <vector>

#include <csharp_helpers.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/wire_format.h>
#include <strutil.h>
#include <substitute.h>

#include <csharp_field_base.h>
#include <csharp_enum_field.h>
#include <csharp_message_field.h>
#include <csharp_primitive_field.h>
#include <csharp_repeated_enum_field.h>
#include <csharp_repeated_message_field.h>
#include <csharp_repeated_primitive_field.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace csharp {

CSharpType GetCSharpType(FieldDescriptor::Type type) {
  switch (type) {
    case FieldDescriptor::TYPE_INT32:
      return CSHARPTYPE_INT32;
    case FieldDescriptor::TYPE_INT64:
      return CSHARPTYPE_INT64;
    case FieldDescriptor::TYPE_UINT32:
      return CSHARPTYPE_UINT32;
    case FieldDescriptor::TYPE_UINT64:
      return CSHARPTYPE_UINT32;
    case FieldDescriptor::TYPE_SINT32:
      return CSHARPTYPE_INT32;
    case FieldDescriptor::TYPE_SINT64:
      return CSHARPTYPE_INT64;
    case FieldDescriptor::TYPE_FIXED32:
      return CSHARPTYPE_UINT32;
    case FieldDescriptor::TYPE_FIXED64:
      return CSHARPTYPE_UINT64;
    case FieldDescriptor::TYPE_SFIXED32:
      return CSHARPTYPE_INT32;
    case FieldDescriptor::TYPE_SFIXED64:
      return CSHARPTYPE_INT64;
    case FieldDescriptor::TYPE_FLOAT:
      return CSHARPTYPE_FLOAT;
    case FieldDescriptor::TYPE_DOUBLE:
      return CSHARPTYPE_DOUBLE;
    case FieldDescriptor::TYPE_BOOL:
      return CSHARPTYPE_BOOL;
    case FieldDescriptor::TYPE_ENUM:
      return CSHARPTYPE_ENUM;
    case FieldDescriptor::TYPE_STRING:
      return CSHARPTYPE_STRING;
    case FieldDescriptor::TYPE_BYTES:
      return CSHARPTYPE_BYTESTRING;
    case FieldDescriptor::TYPE_GROUP:
      return CSHARPTYPE_MESSAGE;
    case FieldDescriptor::TYPE_MESSAGE:
      return CSHARPTYPE_MESSAGE;

      // No default because we want the compiler to complain if any new
      // types are added.
  }
  GOOGLE_LOG(FATAL)<< "Can't get here.";
  return (CSharpType) -1;
}

std::string StripDotProto(const std::string& proto_file) {
  int lastindex = proto_file.find_last_of(".");
  return proto_file.substr(0, lastindex);
}

std::string GetFileNamespace(const FileDescriptor* descriptor) {
  //if (descriptor->options().has_csharp_namespace()) {
  //  return descriptor->options().csharp_namespace();
  //}
  return descriptor->package();
}

std::string GetUmbrellaClassNameInternal(const std::string& proto_file) {
  int lastslash = proto_file.find_last_of("/");
  std::string base = proto_file.substr(lastslash + 1);
  return UnderscoresToPascalCase(StripDotProto(base));
}

std::string GetFileUmbrellaClassname(const FileDescriptor* descriptor) {
  // umbrella_classname can no longer be set using message option.
  return GetUmbrellaClassNameInternal(descriptor->name());
}

std::string GetFileUmbrellaNamespace(const FileDescriptor* descriptor) {
  // TODO(jtattermusch): reintroduce csharp_umbrella_namespace option
  bool collision = false;
  std::string umbrella_classname = GetFileUmbrellaClassname(descriptor);
  for(int i = 0; i < descriptor->message_type_count(); i++) {
    if (descriptor->message_type(i)->name() == umbrella_classname) {
      collision = true;
      break;
    }
  }
  for (int i = 0; i < descriptor->service_count(); i++) {
    if (descriptor->service(i)->name() == umbrella_classname) {
      collision = true;
      break;
    }
  }
  for (int i = 0; i < descriptor->enum_type_count(); i++) {
    if (descriptor->enum_type(i)->name() == umbrella_classname) {
      collision = true;
      break;
    }
  }
  return collision ? "Proto" : "";
}

// TODO(jtattermusch): can we reuse a utility function?
std::string UnderscoresToCamelCase(const std::string& input,
                                   bool cap_next_letter) {
  string result;
  // Note:  I distrust ctype.h due to locales.
  for (int i = 0; i < input.size(); i++) {
    if ('a' <= input[i] && input[i] <= 'z') {
      if (cap_next_letter) {
        result += input[i] + ('A' - 'a');
      } else {
        result += input[i];
      }
      cap_next_letter = false;
    } else if ('A' <= input[i] && input[i] <= 'Z') {
      if (i == 0 && !cap_next_letter) {
        // Force first letter to lower-case unless explicitly told to
        // capitalize it.
        result += input[i] + ('a' - 'A');
      } else {
        // Capital letters after the first are left as-is.
        result += input[i];
      }
      cap_next_letter = false;
    } else if ('0' <= input[i] && input[i] <= '9') {
      result += input[i];
      cap_next_letter = true;
    } else {
      cap_next_letter = true;
    }
  }
  // Add a trailing "_" if the name should be altered.
  if (input[input.size() - 1] == '#') {
	  //result += '_';
  }
  return result;
}

std::string UnderscoresToPascalCase(const std::string& input) {
  return UnderscoresToCamelCase(input, true);
}

std::string ToCSharpName(const std::string& name, const FileDescriptor* file) {
  std::string result = GetFileNamespace(file);
  if (result != "") {
    result += '.';
  }
  string classname;
  if (file->package().empty()) {
    classname = name;
  } else {
    // Strip the proto package from full_name since we've replaced it with
    // the C# namespace.
    classname = name.substr(file->package().size() + 1);
  }
  result += StringReplace(classname, ".", ".Types.", false);
  return "global::" + result;
}



std::string GetFullUmbrellaClassName(const FileDescriptor* descriptor) {
  std::string result = GetFileNamespace(descriptor);
  if (!result.empty()) {
    result += '.';
  }
  result += GetQualifiedUmbrellaClassName(descriptor);
  return "global::" + result;
}

std::string GetQualifiedUmbrellaClassName(const FileDescriptor* descriptor) {
  std::string umbrellaNamespace = GetFileUmbrellaNamespace(descriptor);
  std::string umbrellaClassname = GetFileUmbrellaClassname(descriptor);

  std::string fullName = umbrellaClassname;
  if (!umbrellaNamespace.empty()) {
    fullName = umbrellaNamespace + "." + umbrellaClassname;
  }
  return fullName;
}

std::string GetClassName(const Descriptor* descriptor) {
  return ToCSharpName(descriptor->full_name(), descriptor->file());
}

std::string GetClassName(const EnumDescriptor* descriptor) {
  return ToCSharpName(descriptor->full_name(), descriptor->file());
}

// Groups are hacky:  The name of the field is just the lower-cased name
// of the group type.  In C#, though, we would like to retain the original
// capitalization of the type name.
std::string GetFieldName(const FieldDescriptor* descriptor) {
  return descriptor->name();
}

std::string GetFieldConstantName(const FieldDescriptor* field) {
  return GetPropertyName(field) + "FieldNumber";
}

std::string GetPropertyName(const FieldDescriptor* descriptor) {
  // TODO(jtattermusch): consider introducing csharp_property_name field option
  std::string property_name = UnderscoresToPascalCase(GetFieldName(descriptor));
  if (property_name == descriptor->containing_type()->name()) {
    property_name += "_";
  }
  return property_name;
}

// TODO: c&p from Java protoc plugin
// For encodings with fixed sizes, returns that size in bytes.  Otherwise
// returns -1.
int GetFixedSize(FieldDescriptor::Type type) {
  switch (type) {
    case FieldDescriptor::TYPE_INT32   : return -1;
    case FieldDescriptor::TYPE_INT64   : return -1;
    case FieldDescriptor::TYPE_UINT32  : return -1;
    case FieldDescriptor::TYPE_UINT64  : return -1;
    case FieldDescriptor::TYPE_SINT32  : return -1;
    case FieldDescriptor::TYPE_SINT64  : return -1;
    case FieldDescriptor::TYPE_FIXED32 : return internal::WireFormatLite::kFixed32Size;
    case FieldDescriptor::TYPE_FIXED64 : return internal::WireFormatLite::kFixed64Size;
    case FieldDescriptor::TYPE_SFIXED32: return internal::WireFormatLite::kSFixed32Size;
    case FieldDescriptor::TYPE_SFIXED64: return internal::WireFormatLite::kSFixed64Size;
    case FieldDescriptor::TYPE_FLOAT   : return internal::WireFormatLite::kFloatSize;
    case FieldDescriptor::TYPE_DOUBLE  : return internal::WireFormatLite::kDoubleSize;

    case FieldDescriptor::TYPE_BOOL    : return internal::WireFormatLite::kBoolSize;
    case FieldDescriptor::TYPE_ENUM    : return -1;

    case FieldDescriptor::TYPE_STRING  : return -1;
    case FieldDescriptor::TYPE_BYTES   : return -1;
    case FieldDescriptor::TYPE_GROUP   : return -1;
    case FieldDescriptor::TYPE_MESSAGE : return -1;

    // No default because we want the compiler to complain if any new
    // types are added.
  }
  GOOGLE_LOG(FATAL) << "Can't get here.";
  return -1;
}

static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string StringToBase64(const std::string& input) {
  std::string result;
  size_t remaining = input.size();
  const unsigned char *src = (const unsigned char*) input.c_str();
  while (remaining > 2) {
    result += base64_chars[src[0] >> 2];
    result += base64_chars[((src[0] & 0x3) << 4) | (src[1] >> 4)];
    result += base64_chars[((src[1] & 0xf) << 2) | (src[2] >> 6)];
    result += base64_chars[src[2] & 0x3f];
    remaining -= 3;
    src += 3;
  }
  switch (remaining) {
    case 2:
      result += base64_chars[src[0] >> 2];
      result += base64_chars[((src[0] & 0x3) << 4) | (src[1] >> 4)];
      result += base64_chars[(src[1] & 0xf) << 2];
      result += '=';
      src += 2;
      break;
    case 1:
      result += base64_chars[src[0] >> 2];
      result += base64_chars[((src[0] & 0x3) << 4)];
      result += '=';
      result += '=';
      src += 1;
      break;
  }
  return result;
}

std::string FileDescriptorToBase64(const FileDescriptor* descriptor) {
  std::string fdp_bytes;
  FileDescriptorProto fdp;
  descriptor->CopyTo(&fdp);
  fdp.SerializeToString(&fdp_bytes);
  return StringToBase64(fdp_bytes);
}

FieldGeneratorBase* CreateFieldGenerator(const FieldDescriptor* descriptor,
                                         int fieldOrdinal) {
  switch (descriptor->type()) {
    case FieldDescriptor::TYPE_GROUP:
    case FieldDescriptor::TYPE_MESSAGE:
      if (descriptor->is_repeated()) {
        return new RepeatedMessageFieldGenerator(descriptor, fieldOrdinal);
      } else {
        return new MessageFieldGenerator(descriptor, fieldOrdinal);
      }
    case FieldDescriptor::TYPE_ENUM:
      if (descriptor->is_repeated()) {
        return new RepeatedEnumFieldGenerator(descriptor, fieldOrdinal);
      } else {
        return new EnumFieldGenerator(descriptor, fieldOrdinal);
      }
    default:
      if (descriptor->is_repeated()) {
        return new RepeatedPrimitiveFieldGenerator(descriptor, fieldOrdinal);
      } else {
        return new PrimitiveFieldGenerator(descriptor, fieldOrdinal);
      }
  }
}

bool HasRequiredFields(const Descriptor* descriptor, std::set<const Descriptor*>* already_seen) {
  if (already_seen->find(descriptor) != already_seen->end()) {
    // The type is already in cache.  This means that either:
    // a. The type has no required fields.
    // b. We are in the midst of checking if the type has required fields,
    //    somewhere up the stack.  In this case, we know that if the type
    //    has any required fields, they'll be found when we return to it,
    //    and the whole call to HasRequiredFields() will return true.
    //    Therefore, we don't have to check if this type has required fields
    //    here.
    return false;
  }
  already_seen->insert(descriptor);

  // If the type has extensions, an extension with message type could contain
  // required fields, so we have to be conservative and assume such an
  // extension exists.
  if (descriptor->extension_count() > 0) {
    return true;
  }

  for (int i = 0; i < descriptor->field_count(); i++) {
    const FieldDescriptor* field = descriptor->field(i);
    if (field->is_required()) {
      return true;
    }
    if (GetCSharpType(field->type()) == CSHARPTYPE_MESSAGE) {
      if (HasRequiredFields(field->message_type(), already_seen)) {
        return true;
      }
    }
  }
  return false;
}

bool HasRequiredFields(const Descriptor* descriptor) {
  std::set<const Descriptor*> already_seen;
  return HasRequiredFields(descriptor, &already_seen);
}

}  // namespace java
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
