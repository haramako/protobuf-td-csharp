// Code generated by protoc-gen-tdcs. DO NOT EDIT.
// source: test.proto
#pragma warning disable 1591, 0612, 3021
#region Designer generated code


using System.Collections.Generic;
using pb = global::Google.ProtocolBuffers;
using scg = global::System.Collections.Generic;
using System;

namespace Test {
public enum TestEnum {
FOO = 0 ,
BAR = 1 ,
BAZ = 2 ,
}
public sealed partial class TestMessage: pb.Message {
public Int32 Int32Value = 0;
public Int64 Int64Value = 0;
public string StringValue = "";
public TestEnum EnumValue = 0;
public EmbedMessage MessageValue = null;
public Int32 Sint32Value = 0;
public Int64 Sint64Value = 0;
public List<Int32> RepeatedInt32 = null;
public List<Int64> RepeatedInt64 = null;
public List<EmbedMessage> RepeatedMessage = null;
public List<Int32> PackedInt32 = null;
public List<Int64> PackedInt64 = null;

public TestMessage() { }
public static TestMessage CreateInstance() { var obj = new TestMessage(); obj.Finish(); return obj; }
public static TestMessage CreateEmpty() { return new TestMessage(); }
private static readonly TestMessage defaultInstance = new TestMessage();
public static TestMessage DefaultInstance {
  get { return defaultInstance; }
}
public override void Init() {
}
public override void Finish() {
}
public static TestMessage ParseFrom(byte[] data) {
var mes = CreateInstance(); mes.MergeFrom(data); return mes;
}
public static TestMessage ParseFrom(global::System.IO.Stream input) {
var mes = CreateInstance(); mes.MergeFrom(input); return mes;
}
public static TestMessage ParseFrom(pb::CodedInputStream input) {
var mes = CreateInstance(); mes.MergeFrom(input); return mes;
}

public override void WriteTo(pb::CodedOutputStream output) {
if( Int32Value!=0) {
output.WriteInt32(1,Int32Value);
}
if( Int64Value!=0) {
output.WriteInt64(2,Int64Value);
}
if( StringValue!="") {
output.WriteString(3,StringValue);
}
if( EnumValue!=0) {
output.WriteEnum(4, (int)EnumValue, EnumValue);
}
if( MessageValue!=null) {
output.WriteMessage(5,MessageValue);
}
if( Sint32Value!=0) {
output.WriteInt32(6,Sint32Value);
}
if( Sint64Value!=0) {
output.WriteInt64(7,Sint64Value);
}
if( RepeatedInt32!=null) {
output.WriteInt32Array(10,RepeatedInt32);
}
if( RepeatedInt64!=null) {
output.WriteInt64Array(11,RepeatedInt64);
}
if( RepeatedMessage!=null) {
output.WriteMessageArray(12,RepeatedMessage);
}
if( PackedInt32!=null) {
output.WriteInt32Array(20,PackedInt32);
}
if( PackedInt64!=null) {
output.WriteInt64Array(21,PackedInt64);
}
}
public override int SerializedSize {
get {
return CalcSerializedSize();
}
}

private int CalcSerializedSize() {
int size = 0;
if( Int32Value!=0) {
size += pb::CodedOutputStream.ComputeInt32Size(1,Int32Value);
}
if( Int64Value!=0) {
size += pb::CodedOutputStream.ComputeInt64Size(2,Int64Value);
}
if( StringValue!="") {
size += pb::CodedOutputStream.ComputeStringSize(3,StringValue);
}
if( EnumValue!=0) {
size += pb::CodedOutputStream.ComputeEnumSize(4, (int)EnumValue);
}
if( MessageValue!=null) {
size += pb::CodedOutputStream.ComputeMessageSize(5,MessageValue);
}
if( Sint32Value!=0) {
size += pb::CodedOutputStream.ComputeInt32Size(6,Sint32Value);
}
if( Sint64Value!=0) {
size += pb::CodedOutputStream.ComputeInt64Size(7,Sint64Value);
}
if( RepeatedInt32!=null) {
foreach (var element in RepeatedInt32) {
}
}
if( RepeatedInt64!=null) {
foreach (var element in RepeatedInt64) {
}
}
if( RepeatedMessage!=null) {
foreach (var element in RepeatedMessage) {
}
}
if( PackedInt32!=null) {
foreach (var element in PackedInt32) {
}
}
if( PackedInt64!=null) {
foreach (var element in PackedInt64) {
}
}
return size;
}
public override void MergeFrom(pb::CodedInputStream input) {
uint tag;
while (input.ReadTag(out tag)) {
switch (tag) {
case 0: {
throw pb::InvalidProtocolBufferException.InvalidTag();
}
default: {
if (pb::WireFormat.IsEndGroupTag(tag)) {
return;
}
input.SkipField(); // unknown field
break;
}
case 8: {
input.ReadInt32(ref this.Int32Value);
break;
}
case 16: {
input.ReadInt64(ref this.Int64Value);
break;
}
case 24: {
input.ReadString(ref this.StringValue);
break;
}
case 32: {
input.ReadEnum(ref this.EnumValue);
break;
}
case 40: {
input.ReadMessage(this.MessageValue);
break;
}
case 48: {
input.ReadInt32(ref this.Sint32Value);
break;
}
case 56: {
input.ReadInt64(ref this.Sint64Value);
break;
}
case 80: {
input.ReadInt32Array(tag, this.RepeatedInt32);
break;
}
case 88: {
input.ReadInt64Array(tag, this.RepeatedInt64);
break;
}
case 96: {
input.ReadMessageArray(tag, this.RepeatedMessage,EmbedMessage.CreateInstance );
break;
}
case 160: {
input.ReadInt32Array(tag, this.PackedInt32);
break;
}
case 168: {
input.ReadInt64Array(tag, this.PackedInt64);
break;
}
}
}
}
}

public sealed partial class EmbedMessage: pb.Message {
public Int32 Val = 0;

public EmbedMessage() { }
public static EmbedMessage CreateInstance() { var obj = new EmbedMessage(); obj.Finish(); return obj; }
public static EmbedMessage CreateEmpty() { return new EmbedMessage(); }
private static readonly EmbedMessage defaultInstance = new EmbedMessage();
public static EmbedMessage DefaultInstance {
  get { return defaultInstance; }
}
public override void Init() {
}
public override void Finish() {
}
public static EmbedMessage ParseFrom(byte[] data) {
var mes = CreateInstance(); mes.MergeFrom(data); return mes;
}
public static EmbedMessage ParseFrom(global::System.IO.Stream input) {
var mes = CreateInstance(); mes.MergeFrom(input); return mes;
}
public static EmbedMessage ParseFrom(pb::CodedInputStream input) {
var mes = CreateInstance(); mes.MergeFrom(input); return mes;
}

public override void WriteTo(pb::CodedOutputStream output) {
if( Val!=0) {
output.WriteInt32(1,Val);
}
}
public override int SerializedSize {
get {
return CalcSerializedSize();
}
}

private int CalcSerializedSize() {
int size = 0;
if( Val!=0) {
size += pb::CodedOutputStream.ComputeInt32Size(1,Val);
}
return size;
}
public override void MergeFrom(pb::CodedInputStream input) {
uint tag;
while (input.ReadTag(out tag)) {
switch (tag) {
case 0: {
throw pb::InvalidProtocolBufferException.InvalidTag();
}
default: {
if (pb::WireFormat.IsEndGroupTag(tag)) {
return;
}
input.SkipField(); // unknown field
break;
}
case 8: {
input.ReadInt32(ref this.Val);
break;
}
}
}
}
}

}
#endregion Designer generated code
