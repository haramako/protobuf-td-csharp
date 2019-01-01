package generator

import "../descriptor"

// simpleField is not weak, not a oneof, not an extension. Can be required, optional or repeated.
type simpleField struct {
	fieldCommon
	protoTypeName string                               // Proto type name, empty if primitive, e.g. ".google.protobuf.Duration"
	protoType     descriptor.FieldDescriptorProto_Type // Actual type enum value, e.g. descriptor.FieldDescriptorProto_TYPE_FIXED64
	deprecated    string                               // Deprecation comment, if any, e.g. "// Deprecated: Do not use."
	getterDef     string                               // Default for getters, e.g. "nil", `""` or "Default_MessageType_FieldName"
	protoDef      string                               // Default value as defined in the proto file, e.g "yoshi" or "5"
	comment       string                               // The full comment for the field, e.g. "// Useful information"
	shared        bool                                 // Shared field
}

// decl prints the declaration of the field in the struct (if any).
func (f *simpleField) decl(g *Generator, mc *msgCtx) {
	t := &f.fieldCommon.goType
	if f.shared {
		f.declShared(g, mc)
	} else if t.size != 0 {
		g.P(f.comment, "public ", t.ftype, " ", f.goName, "_", f.deprecated, " = ", getDefaultValue(f.protoType, &f.fieldCommon), ";")
		g.P(f.comment, "public ", t.typ, " ", f.goName, f.deprecated, " { get { return (", t.typ, ")", f.goName, "_; } set { ", f.goName, "_ = (", t.ftype, ")value; }")
		g.P("}")
	} else {
		g.P(f.comment, "public ", t.typ, " ", f.goName, f.deprecated, " = ", getDefaultValue(f.protoType, &f.fieldCommon), ";")
	}
}

func (f *simpleField) declShared(g *Generator, mc *msgCtx) {
	g.P(f.comment, "public ", f.fieldCommon.goType.typ, " ", f.goName, f.deprecated, " { ")
	g.P("get { var found = getShared(", f.fieldCommon.proto.GetNumber(), "); if( found != null ){ return (", f.fieldCommon.goType.typ, ")found; }else{ return ", getNullValue2(&f.fieldCommon), ";} }")
	g.P("set { setShared(", f.fieldCommon.proto.GetNumber(), ", value); }")
	g.P("}")
}

// getter prints the getter for the field.
func (f *simpleField) getter(g *Generator, mc *msgCtx) {
}

func (f *simpleField) writeTo(g *Generator, mc *msgCtx) {
	d := f.fieldCommon.proto
	g.P("if( ", f.goName, "!=", getNullValue(f.proto.GetType(), &f.fieldCommon), ") {")
	if isRepeated(d) {
		if f.protoType == descriptor.FieldDescriptorProto_TYPE_MESSAGE {
			g.P("output.WriteMessageArray(", f.proto.GetNumber(), ",", f.goName, ");")
		} else if f.protoType == descriptor.FieldDescriptorProto_TYPE_ENUM {
			//g.P("output.WriteEnumArray(", f.proto.GetNumber(), ",", f.goName, ");") // TODO: WriteEnumArray対応
		} else {
			// TODO: 各Array対応
			g.P("output.Write", getFunctionPostfix(f.proto.GetType()), "Array(", f.proto.GetNumber(), ",", f.goName, ");")
		}
	} else if f.protoType == descriptor.FieldDescriptorProto_TYPE_ENUM {
		g.P("output.WriteEnum(", f.proto.GetNumber(), ", (int)", f.goName, ", ", f.goName, ");") // TODO: rawValueはいらない？
	} else {
		g.P("output.Write", getFunctionPostfix(f.proto.GetType()), "(", f.proto.GetNumber(), ",", f.goName, ");")
	}
	g.P("}")
}

func (f *simpleField) calcSize(g *Generator, mc *msgCtx) {
	d := f.fieldCommon.proto
	g.P("if( ", f.goName, "!=", getNullValue(f.proto.GetType(), &f.fieldCommon), ") {")
	if isRepeated(d) {
		g.P("foreach (var element in ", f.goName, ") {")
		//g.P("size += pb::CodedOutputStream.ComputeArraySize(", f.proto.GetNumber(), ", element);")
		g.P("}")
	} else if f.protoType == descriptor.FieldDescriptorProto_TYPE_ENUM {
		g.P("size += pb::CodedOutputStream.ComputeEnumSize(", f.proto.GetNumber(), ", (int)", f.goName, ");")
	} else {
		g.P("size += pb::CodedOutputStream.Compute", getFunctionPostfix(f.proto.GetType()), "Size(", f.proto.GetNumber(), ",", f.goName, ");")
	}
	g.P("}")
}

// WireType is presentation in binary format.
type WireType int

const (
	WireTypeVarint          = 0
	WireTypeFixed64         = 1
	WireTypeLengthDelimited = 2
	WireTypeStartGroup      = 3
	WireTypeEndGroup        = 4
	WireTypeFixed32         = 5
)

func GetWireTypeTag(wireType string) WireType {
	switch wireType {
	case "varint", "zigzag64", "zigzag32":
		return WireTypeVarint
	case "fixed64":
		return WireTypeFixed64
	case "bytes":
		return WireTypeLengthDelimited
	case "group":
		return WireTypeStartGroup // TODO: どういう意味か不明
	case "fixed32":
		return WireTypeFixed32
	default:
		panic("Unkown wire type " + wireType)
	}
}

func (f *simpleField) mergeFrom(g *Generator, mc *msgCtx) {
	d := f.fieldCommon.proto
	g.P("case ", f.proto.GetNumber()<<3|int32(GetWireTypeTag(f.fieldCommon.goType.wire)), ": {")
	if isRepeated(d) {
		if f.protoType == descriptor.FieldDescriptorProto_TYPE_MESSAGE {
			g.P("if( ", f.goName, " == null ) ", f.goName, " = new ", f.goType.typ, "();")
			g.P("input.ReadMessageArray(tag, this.", f.goName, ",", f.fieldCommon.goType.etype, ".CreateInstance );")
		} else if f.protoType == descriptor.FieldDescriptorProto_TYPE_ENUM {
			g.P("if( ", f.goName, " == null ) ", f.goName, " = new ", f.goType.typ, "();")
			g.P("input.ReadEnumArray(tag, this.", f.goName, ");")
		} else {
			g.P("if( ", f.goName, " == null ) ", f.goName, " = new ", f.goType.typ, "();")
			g.P("input.Read", getFunctionPostfix(f.proto.GetType()), "Array(tag, this.", f.goName, ");")
		}
	} else if f.protoType == descriptor.FieldDescriptorProto_TYPE_MESSAGE {
		if f.shared {
			g.P("var temp = ", f.fieldCommon.goType.typ, ".CreateInstance();")
			g.P("input.ReadMessage(temp);")
			g.P("pb::SharedItem.PushTemp(", f.fieldCommon.proto.GetNumber(), ", temp);")
			g.P("sharedNum++;")
		} else {
			g.P("input.ReadMessage(this.", f.goName, ");")
		}
	} else if f.protoType == descriptor.FieldDescriptorProto_TYPE_ENUM {
		if f.shared {
			g.P(f.fieldCommon.goType.typ, " temp = ", getDefaultValue(f.fieldCommon.proto.GetType(), &f.fieldCommon), ";")
			g.P("input.ReadEnum(ref temp);")
			g.P("pb::SharedItem.PushTemp(", f.fieldCommon.proto.GetNumber(), ", temp);")
			g.P("sharedNum++;")
		} else {
			g.P("input.ReadEnum(ref this.", f.goName, ");")
		}
	} else {
		if f.shared {
			g.P(f.fieldCommon.goType.typ, " temp = ", getDefaultValue(f.fieldCommon.proto.GetType(), &f.fieldCommon), ";")
			g.P("input.Read", getFunctionPostfix(f.proto.GetType()), "(ref temp);")
			g.P("pb::SharedItem.PushTemp(", f.fieldCommon.proto.GetNumber(), ", temp);")
			g.P("sharedNum++;")
		} else if f.size != 32 {
			g.P("Int32 temp = ", getDefaultValue(f.fieldCommon.proto.GetType(), &f.fieldCommon), ";")
			g.P("input.Read", getFunctionPostfix(f.proto.GetType()), "(ref temp);")
			g.P(f.goName, "= (", f.fieldCommon.goType.typ, ")temp;")
		} else {
			g.P("input.Read", getFunctionPostfix(f.proto.GetType()), "(ref this.", f.goName, ");")
		}
	}
	g.P("break;")
	g.P("}")
}

func (f *simpleField) baseType() string {
	d := f.fieldCommon.proto
	switch *d.Type {
	case descriptor.FieldDescriptorProto_TYPE_MESSAGE:
		return "Message"
	default:
		return f.goType.typ
	}
}

func (f *simpleField) baseElementType() string {
	d := f.fieldCommon.proto
	switch *d.Type {
	case descriptor.FieldDescriptorProto_TYPE_MESSAGE:
		return "Message"
	default:
		return f.fieldCommon.goType.etype
	}
}

// setter prints the setter method of the field.
func (f *simpleField) setter(g *Generator, mc *msgCtx) {
	// No setter for regular fields yet
}

// getProtoDef returns the default value explicitly stated in the proto file, e.g "yoshi" or "5".
func (f *simpleField) getProtoDef() string {
	return f.protoDef
}

// getProtoTypeName returns the protobuf type name for the field as returned by field.GetTypeName(), e.g. ".google.protobuf.Duration".
func (f *simpleField) getProtoTypeName() string {
	return f.protoTypeName
}

// getProtoType returns the *field.Type value, e.g. descriptor.FieldDescriptorProto_TYPE_FIXED64.
func (f *simpleField) getProtoType() descriptor.FieldDescriptorProto_Type {
	return f.protoType
}
