package generator

import (
	"github.com/golang/protobuf/proto"
	"github.com/golang/protobuf/protoc-gen-go/descriptor"
)

// oneofField represents the oneof on top level.
// The alternative fields within the oneof are represented by oneofSubField.
type oneofField struct {
	fieldCommon
	subFields []*oneofSubField // All the possible oneof fields
	comment   string           // The full comment for the field, e.g. "// Types that are valid to be assigned to MyOneof:\n\\"
}

// decl prints the declaration of the field in the struct (if any).
func (f *oneofField) decl(g *Generator, mc *msgCtx) {
	comment := f.comment
	for _, sf := range f.subFields {
		comment += "//\t*" + sf.oneofTypeName + "\n"
	}
	//g.P(comment, Annotate(mc.message.file, f.fullPath, f.goName), " ", f.goType, " `", f.tags, "`")
}

// getter for a oneof field will print additional discriminators and interfaces for the oneof,
// also it prints all the getters for the sub fields.
func (f *oneofField) getter(g *Generator, mc *msgCtx) {
}

// setter prints the setter method of the field.
func (f *oneofField) setter(g *Generator, mc *msgCtx) {
	// No setters for oneof yet
}

func (f *oneofField) writeTo(g *Generator, mc *msgCtx) {
	panic("writeTo not supported")
}

func (f *oneofField) calcSize(g *Generator, mc *msgCtx) {
	panic("calcSize not supported")
}

func (f *oneofField) mergeFrom(g *Generator, mc *msgCtx) {
	panic("mergeFrom not supported")
}

// oneofSubFields are kept slize held by each oneofField. They do not appear in the top level slize of fields for the message.
type oneofSubField struct {
	fieldCommon
	protoTypeName string                               // Proto type name, empty if primitive, e.g. ".google.protobuf.Duration"
	protoType     descriptor.FieldDescriptorProto_Type // Actual type enum value, e.g. descriptor.FieldDescriptorProto_TYPE_FIXED64
	oneofTypeName string                               // Type name of the enclosing struct, e.g. "MessageName_FieldName"
	fieldNumber   int                                  // Actual field number, as defined in proto, e.g. 12
	getterDef     string                               // Default for getters, e.g. "nil", `""` or "Default_MessageType_FieldName"
	protoDef      string                               // Default value as defined in the proto file, e.g "yoshi" or "5"
	deprecated    string                               // Deprecation comment, if any.
}

// wireTypeName returns a textual wire type, needed for oneof sub fields in generated code.
func (f *oneofSubField) wireTypeName() string {
	switch f.protoType {
	case descriptor.FieldDescriptorProto_TYPE_FIXED64,
		descriptor.FieldDescriptorProto_TYPE_SFIXED64,
		descriptor.FieldDescriptorProto_TYPE_DOUBLE:
		return "WireFixed64"
	case descriptor.FieldDescriptorProto_TYPE_FIXED32,
		descriptor.FieldDescriptorProto_TYPE_SFIXED32,
		descriptor.FieldDescriptorProto_TYPE_FLOAT:
		return "WireFixed32"
	case descriptor.FieldDescriptorProto_TYPE_GROUP:
		return "WireStartGroup"
	case descriptor.FieldDescriptorProto_TYPE_MESSAGE,
		descriptor.FieldDescriptorProto_TYPE_STRING,
		descriptor.FieldDescriptorProto_TYPE_BYTES:
		return "WireBytes"
	default: // All others are Varints
		return "WireVarint"
	}
}

// typedNil prints a nil casted to the pointer to this field.
// - for XXX_OneofFuncs
func (f *oneofSubField) typedNil(g *Generator) {
	g.P("(*", f.oneofTypeName, ")(nil),")
}

// marshalCase prints the case matching this oneof subfield in the marshalling code.
func (f *oneofSubField) marshalCase(g *Generator) {
	g.P("case *", f.oneofTypeName, ":")
	wire := f.wireTypeName()
	var pre, post string
	val := "x." + f.goName // overridden for TYPE_BOOL
	switch f.protoType {
	case descriptor.FieldDescriptorProto_TYPE_DOUBLE:
		pre = "b.EncodeFixed64(" + g.Pkg["math"] + ".Float64bits("
		post = "))"
	case descriptor.FieldDescriptorProto_TYPE_FLOAT:
		pre = "b.EncodeFixed32(uint64(" + g.Pkg["math"] + ".Float32bits("
		post = ")))"
	case descriptor.FieldDescriptorProto_TYPE_INT64, descriptor.FieldDescriptorProto_TYPE_UINT64:
		pre, post = "b.EncodeVarint(uint64(", "))"
	case descriptor.FieldDescriptorProto_TYPE_INT32, descriptor.FieldDescriptorProto_TYPE_UINT32, descriptor.FieldDescriptorProto_TYPE_ENUM:
		pre, post = "b.EncodeVarint(uint64(", "))"
	case descriptor.FieldDescriptorProto_TYPE_FIXED64, descriptor.FieldDescriptorProto_TYPE_SFIXED64:
		pre, post = "b.EncodeFixed64(uint64(", "))"
	case descriptor.FieldDescriptorProto_TYPE_FIXED32, descriptor.FieldDescriptorProto_TYPE_SFIXED32:
		pre, post = "b.EncodeFixed32(uint64(", "))"
	case descriptor.FieldDescriptorProto_TYPE_BOOL:
		g.P("t := uint64(0)")
		g.P("if ", val, " { t = 1 }")
		val = "t"
		pre, post = "b.EncodeVarint(", ")"
	case descriptor.FieldDescriptorProto_TYPE_STRING:
		pre, post = "b.EncodeStringBytes(", ")"
	case descriptor.FieldDescriptorProto_TYPE_GROUP:
		pre, post = "b.Marshal(", ")"
	case descriptor.FieldDescriptorProto_TYPE_MESSAGE:
		pre, post = "b.EncodeMessage(", ")"
	case descriptor.FieldDescriptorProto_TYPE_BYTES:
		pre, post = "b.EncodeRawBytes(", ")"
	case descriptor.FieldDescriptorProto_TYPE_SINT32:
		pre, post = "b.EncodeZigzag32(uint64(", "))"
	case descriptor.FieldDescriptorProto_TYPE_SINT64:
		pre, post = "b.EncodeZigzag64(uint64(", "))"
	default:
		g.Fail("unhandled oneof field type ", f.protoType.String())
	}
	g.P("b.EncodeVarint(", f.fieldNumber, "<<3|", g.Pkg["proto"], ".", wire, ")")
	if t := f.protoType; t != descriptor.FieldDescriptorProto_TYPE_GROUP && t != descriptor.FieldDescriptorProto_TYPE_MESSAGE {
		g.P(pre, val, post)
	} else {
		g.P("if err := ", pre, val, post, "; err != nil {")
		g.P("return err")
		g.P("}")
	}
	if f.protoType == descriptor.FieldDescriptorProto_TYPE_GROUP {
		g.P("b.EncodeVarint(", f.fieldNumber, "<<3|", g.Pkg["proto"], ".WireEndGroup)")
	}
}

// unmarshalCase prints the case matching this oneof subfield in the unmarshalling code.
func (f *oneofSubField) unmarshalCase(g *Generator, origOneofName string, oneofName string) {
	g.P("case ", f.fieldNumber, ": // ", origOneofName, ".", f.getProtoName())
	g.P("if wire != ", g.Pkg["proto"], ".", f.wireTypeName(), " {")
	g.P("return true, ", g.Pkg["proto"], ".ErrInternalBadWireType")
	g.P("}")
	lhs := "x, err" // overridden for TYPE_MESSAGE and TYPE_GROUP
	var dec, cast, cast2 string
	switch f.protoType {
	case descriptor.FieldDescriptorProto_TYPE_DOUBLE:
		dec, cast = "b.DecodeFixed64()", g.Pkg["math"]+".Float64frombits"
	case descriptor.FieldDescriptorProto_TYPE_FLOAT:
		dec, cast, cast2 = "b.DecodeFixed32()", "uint32", g.Pkg["math"]+".Float32frombits"
	case descriptor.FieldDescriptorProto_TYPE_INT64:
		dec, cast = "b.DecodeVarint()", "int64"
	case descriptor.FieldDescriptorProto_TYPE_UINT64:
		dec = "b.DecodeVarint()"
	case descriptor.FieldDescriptorProto_TYPE_INT32:
		dec, cast = "b.DecodeVarint()", "int32"
	case descriptor.FieldDescriptorProto_TYPE_FIXED64:
		dec = "b.DecodeFixed64()"
	case descriptor.FieldDescriptorProto_TYPE_FIXED32:
		dec, cast = "b.DecodeFixed32()", "uint32"
	case descriptor.FieldDescriptorProto_TYPE_BOOL:
		dec = "b.DecodeVarint()"
		// handled specially below
	case descriptor.FieldDescriptorProto_TYPE_STRING:
		dec = "b.DecodeStringBytes()"
	case descriptor.FieldDescriptorProto_TYPE_GROUP:
		g.P("msg := new(", f.goType[1:], ")") // drop star
		lhs = "err"
		dec = "b.DecodeGroup(msg)"
		// handled specially below
	case descriptor.FieldDescriptorProto_TYPE_MESSAGE:
		g.P("msg := new(", f.goType[1:], ")") // drop star
		lhs = "err"
		dec = "b.DecodeMessage(msg)"
		// handled specially below
	case descriptor.FieldDescriptorProto_TYPE_BYTES:
		dec = "b.DecodeRawBytes(true)"
	case descriptor.FieldDescriptorProto_TYPE_UINT32:
		dec, cast = "b.DecodeVarint()", "uint32"
	case descriptor.FieldDescriptorProto_TYPE_ENUM:
		dec, cast = "b.DecodeVarint()", f.goType
	case descriptor.FieldDescriptorProto_TYPE_SFIXED32:
		dec, cast = "b.DecodeFixed32()", "int32"
	case descriptor.FieldDescriptorProto_TYPE_SFIXED64:
		dec, cast = "b.DecodeFixed64()", "int64"
	case descriptor.FieldDescriptorProto_TYPE_SINT32:
		dec, cast = "b.DecodeZigzag32()", "int32"
	case descriptor.FieldDescriptorProto_TYPE_SINT64:
		dec, cast = "b.DecodeZigzag64()", "int64"
	default:
		g.Fail("unhandled oneof field type ", f.protoType.String())
	}
	g.P(lhs, " := ", dec)
	val := "x"
	if cast != "" {
		val = cast + "(" + val + ")"
	}
	if cast2 != "" {
		val = cast2 + "(" + val + ")"
	}
	switch f.protoType {
	case descriptor.FieldDescriptorProto_TYPE_BOOL:
		val += " != 0"
	case descriptor.FieldDescriptorProto_TYPE_GROUP,
		descriptor.FieldDescriptorProto_TYPE_MESSAGE:
		val = "msg"
	}
	g.P("m.", oneofName, " = &", f.oneofTypeName, "{", val, "}")
	g.P("return true, err")
}

// sizerCase prints the case matching this oneof subfield in the sizer code.
func (f *oneofSubField) sizerCase(g *Generator) {
	g.P("case *", f.oneofTypeName, ":")
	val := "x." + f.goName
	var varint, fixed string
	switch f.protoType {
	case descriptor.FieldDescriptorProto_TYPE_DOUBLE:
		fixed = "8"
	case descriptor.FieldDescriptorProto_TYPE_FLOAT:
		fixed = "4"
	case descriptor.FieldDescriptorProto_TYPE_INT64, descriptor.FieldDescriptorProto_TYPE_UINT64, descriptor.FieldDescriptorProto_TYPE_INT32, descriptor.FieldDescriptorProto_TYPE_UINT32, descriptor.FieldDescriptorProto_TYPE_ENUM:
		varint = val
	case descriptor.FieldDescriptorProto_TYPE_FIXED64, descriptor.FieldDescriptorProto_TYPE_SFIXED64:
		fixed = "8"
	case descriptor.FieldDescriptorProto_TYPE_FIXED32, descriptor.FieldDescriptorProto_TYPE_SFIXED32:
		fixed = "4"
	case descriptor.FieldDescriptorProto_TYPE_BOOL:
		fixed = "1"
	case descriptor.FieldDescriptorProto_TYPE_STRING:
		fixed = "len(" + val + ")"
		varint = fixed
	case descriptor.FieldDescriptorProto_TYPE_GROUP:
		fixed = g.Pkg["proto"] + ".Size(" + val + ")"
	case descriptor.FieldDescriptorProto_TYPE_MESSAGE:
		g.P("s := ", g.Pkg["proto"], ".Size(", val, ")")
		fixed = "s"
		varint = fixed
	case descriptor.FieldDescriptorProto_TYPE_BYTES:
		fixed = "len(" + val + ")"
		varint = fixed
	case descriptor.FieldDescriptorProto_TYPE_SINT32:
		varint = "(uint32(" + val + ") << 1) ^ uint32((int32(" + val + ") >> 31))"
	case descriptor.FieldDescriptorProto_TYPE_SINT64:
		varint = "uint64(" + val + " << 1) ^ uint64((int64(" + val + ") >> 63))"
	default:
		g.Fail("unhandled oneof field type ", f.protoType.String())
	}
	// Tag and wire varint is known statically,
	// so don't generate code for that part of the size computation.
	tagAndWireSize := proto.SizeVarint(uint64(f.fieldNumber << 3)) // wire doesn't affect varint size
	g.P("n += ", tagAndWireSize, " // tag and wire")
	if varint != "" {
		g.P("n += ", g.Pkg["proto"], ".SizeVarint(uint64(", varint, "))")
	}
	if fixed != "" {
		g.P("n += ", fixed)
	}
	if f.protoType == descriptor.FieldDescriptorProto_TYPE_GROUP {
		g.P("n += ", tagAndWireSize, " // tag and wire")
	}
}

// getProtoDef returns the default value explicitly stated in the proto file, e.g "yoshi" or "5".
func (f *oneofSubField) getProtoDef() string {
	return f.protoDef
}

// getProtoTypeName returns the protobuf type name for the field as returned by field.GetTypeName(), e.g. ".google.protobuf.Duration".
func (f *oneofSubField) getProtoTypeName() string {
	return f.protoTypeName
}

// getProtoType returns the *field.Type value, e.g. descriptor.FieldDescriptorProto_TYPE_FIXED64.
func (f *oneofSubField) getProtoType() descriptor.FieldDescriptorProto_Type {
	return f.protoType
}
