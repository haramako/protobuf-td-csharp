// Code generated by protoc-gen-tdcs. DO NOT EDIT.
// source: test.proto
#pragma warning disable 1591, 0612, 3021
#region Designer generated code


using System.Collections.Generic;
using pb = global::Google.ProtocolBuffers;
using ByteString = global::Google.ProtocolBuffers.ByteString;
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
public Test.TestEnum EnumValue = 0;
public Test.EmbedMessage MessageValue = new Test.EmbedMessage();
public Int32 Sint32Value = 0;
public Int64 Sint64Value = 0;
public ByteString BytesValue = new ByteString();
public List<Int32> RepeatedInt32 = new List<Int32>();
public List<Int64> RepeatedInt64 = new List<Int64>();
public List<Test.EmbedMessage> RepeatedMessage = new List<Test.EmbedMessage>();
public List<Int32> PackedInt32 = new List<Int32>();
public List<Int64> PackedInt64 = new List<Int64>();
public Int32 SharedInt32 { 
get { var found = getShared(30); if( found != null ){ return (Int32)found; }else{ return 0;} }
set { setShared(30, value); }
}
public string SharedString { 
get { var found = getShared(31); if( found != null ){ return (string)found; }else{ return "";} }
set { setShared(31, value); }
}
public Test.EmbedMessage SharedMessage { 
get { var found = getShared(32); if( found != null ){ return (Test.EmbedMessage)found; }else{ return null;} }
set { setShared(32, value); }
}
public List<Int32> SharedInt32Array { 
get { var found = getShared(35); if( found != null ){ return (List<Int32>)found; }else{ return null;} }
set { setShared(35, value); }
}
public Test.TestEnum SharedEnum { 
get { var found = getShared(36); if( found != null ){ return (Test.TestEnum)found; }else{ return 0;} }
set { setShared(36, value); }
}
public byte Int8Value_ = 0;
public Int32 Int8Value { get { return (Int32)Int8Value_; } set { Int8Value_ = (byte)value; }
}
public Int16 Int16Value_ = 0;
public Int32 Int16Value { get { return (Int32)Int16Value_; } set { Int16Value_ = (Int16)value; }
}
public TestImported.ImportedMessage ImportedValue = new TestImported.ImportedMessage();
pb::SharedItem[] sharedList_;
object getShared(int id){
  if( sharedList_ == null ) return null;
  for(int i = 0; i < sharedList_.Length; i++){
    if( sharedList_[i].Id == 0 ){ return null; }
    if( sharedList_[i].Id == id ){ return sharedList_[i].Value; }
  }
  return null;
}
void setShared(int id, object val){
  if( sharedList_ == null ){ sharedList_ = new pb::SharedItem[4]; }
  for(int i = 0; i < sharedList_.Length; i++){
    if( sharedList_[i].Id == 0 || sharedList_[i].Id == id ){
       sharedList_[i].Id = id;
       sharedList_[i].Value = val;
       return;
     }
  }
}

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

public override void MergeFrom(pb::CodedInputStream input) {
uint tag;
int sharedNum = 0;
while (input.ReadTag(out tag)) {
switch (tag) {
case 0: {
throw pb::InvalidProtocolBufferException.InvalidTag();
}
default: {
if (pb::WireFormat.IsEndGroupTag(tag)) {
return;
}
UnityEngine.Debug.Log("unknown field " + tag);
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
case 26: {
input.ReadString(ref this.StringValue);
break;
}
case 32: {
input.ReadEnum(ref this.EnumValue);
break;
}
case 42: {
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
case 66: {
input.ReadBytes(ref this.BytesValue);
break;
}
case 80: {
if( RepeatedInt32 == null ) RepeatedInt32 = new List<Int32>();
input.ReadInt32Array(tag, this.RepeatedInt32);
break;
}
case 88: {
if( RepeatedInt64 == null ) RepeatedInt64 = new List<Int64>();
input.ReadInt64Array(tag, this.RepeatedInt64);
break;
}
case 98: {
if( RepeatedMessage == null ) RepeatedMessage = new List<Test.EmbedMessage>();
input.ReadMessageArray(tag, this.RepeatedMessage,Test.EmbedMessage.CreateInstance );
break;
}
case 160: {
if( PackedInt32 == null ) PackedInt32 = new List<Int32>();
input.ReadInt32Array(tag, this.PackedInt32);
break;
}
case 168: {
if( PackedInt64 == null ) PackedInt64 = new List<Int64>();
input.ReadInt64Array(tag, this.PackedInt64);
break;
}
case 240: {
Int32 temp = 0;
input.ReadInt32(ref temp);
pb::SharedItem.PushTemp(30, temp);
sharedNum++;
break;
}
case 250: {
string temp = "";
input.ReadString(ref temp);
pb::SharedItem.PushTemp(31, temp);
sharedNum++;
break;
}
case 258: {
var temp = Test.EmbedMessage.CreateInstance();
input.ReadMessage(temp);
pb::SharedItem.PushTemp(32, temp);
sharedNum++;
break;
}
case 280: {
if( SharedInt32Array == null ) SharedInt32Array = new List<Int32>();
input.ReadInt32Array(tag, this.SharedInt32Array);
break;
}
case 288: {
Test.TestEnum temp = 0;
input.ReadEnum(ref temp);
pb::SharedItem.PushTemp(36, temp);
sharedNum++;
break;
}
case 264: {
Int32 temp = 0;
input.ReadInt32(ref temp);
Int8Value= (Int32)temp;
break;
}
case 272: {
Int32 temp = 0;
input.ReadInt32(ref temp);
Int16Value= (Int32)temp;
break;
}
case 322: {
input.ReadMessage(this.ImportedValue);
break;
}
}
}
if( sharedNum > 0 ){ sharedList_ = pb::SharedItem.PopTemp(sharedNum); }
}
public override void WriteTo(pb::CodedOutputStream output) {
if( Int32Value!=0) {
UnityEngine.Debug.Log("write Int32Value " + Int32Value);
output.WriteInt32(1,Int32Value);
}
if( Int64Value!=0) {
UnityEngine.Debug.Log("write Int64Value " + Int64Value);
output.WriteInt64(2,Int64Value);
}
if( StringValue!="") {
UnityEngine.Debug.Log("write StringValue " + StringValue);
output.WriteString(3,StringValue);
}
if( EnumValue!=0) {
UnityEngine.Debug.Log("write EnumValue " + EnumValue);
output.WriteEnum(4, (int)EnumValue, EnumValue);
}
if( MessageValue!=null) {
UnityEngine.Debug.Log("write MessageValue " + MessageValue);
output.WriteMessage(5,MessageValue);
}
if( Sint32Value!=0) {
UnityEngine.Debug.Log("write Sint32Value " + Sint32Value);
output.WriteInt32(6,Sint32Value);
}
if( Sint64Value!=0) {
UnityEngine.Debug.Log("write Sint64Value " + Sint64Value);
output.WriteInt64(7,Sint64Value);
}
if( BytesValue!=new ByteString()) {
UnityEngine.Debug.Log("write BytesValue " + BytesValue);
output.WriteBytes(8,BytesValue);
}
if( RepeatedInt32!=null) {
UnityEngine.Debug.Log("write RepeatedInt32 " + RepeatedInt32);
output.WriteInt32Array(10,RepeatedInt32);
}
if( RepeatedInt64!=null) {
UnityEngine.Debug.Log("write RepeatedInt64 " + RepeatedInt64);
output.WriteInt64Array(11,RepeatedInt64);
}
if( RepeatedMessage!=null) {
UnityEngine.Debug.Log("write RepeatedMessage " + RepeatedMessage);
output.WriteMessageArray(12,RepeatedMessage);
}
if( PackedInt32!=null) {
UnityEngine.Debug.Log("write PackedInt32 " + PackedInt32);
output.WriteInt32Array(20,PackedInt32);
}
if( PackedInt64!=null) {
UnityEngine.Debug.Log("write PackedInt64 " + PackedInt64);
output.WriteInt64Array(21,PackedInt64);
}
if( SharedInt32!=0) {
UnityEngine.Debug.Log("write SharedInt32 " + SharedInt32);
output.WriteInt32(30,SharedInt32);
}
if( SharedString!="") {
UnityEngine.Debug.Log("write SharedString " + SharedString);
output.WriteString(31,SharedString);
}
if( SharedMessage!=null) {
UnityEngine.Debug.Log("write SharedMessage " + SharedMessage);
output.WriteMessage(32,SharedMessage);
}
if( SharedInt32Array!=null) {
UnityEngine.Debug.Log("write SharedInt32Array " + SharedInt32Array);
output.WriteInt32Array(35,SharedInt32Array);
}
if( SharedEnum!=0) {
UnityEngine.Debug.Log("write SharedEnum " + SharedEnum);
output.WriteEnum(36, (int)SharedEnum, SharedEnum);
}
if( Int8Value!=0) {
UnityEngine.Debug.Log("write Int8Value " + Int8Value);
output.WriteInt32(33,Int8Value);
}
if( Int16Value!=0) {
UnityEngine.Debug.Log("write Int16Value " + Int16Value);
output.WriteInt32(34,Int16Value);
}
if( ImportedValue!=null) {
UnityEngine.Debug.Log("write ImportedValue " + ImportedValue);
output.WriteMessage(40,ImportedValue);
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
if( BytesValue!=new ByteString()) {
size += pb::CodedOutputStream.ComputeBytesSize(8,BytesValue);
}
if( RepeatedInt32!=null) {
foreach (var element in RepeatedInt32) {
size += pb::CodedOutputStream.ComputeInt32Size(10, element);
}
}
if( RepeatedInt64!=null) {
foreach (var element in RepeatedInt64) {
size += pb::CodedOutputStream.ComputeInt64Size(11, element);
}
}
if( RepeatedMessage!=null) {
foreach (var element in RepeatedMessage) {
size += pb::CodedOutputStream.ComputeMessageSize(12, element);
}
}
if( PackedInt32!=null) {
foreach (var element in PackedInt32) {
size += pb::CodedOutputStream.ComputeInt32Size(20, element);
}
}
if( PackedInt64!=null) {
foreach (var element in PackedInt64) {
size += pb::CodedOutputStream.ComputeInt64Size(21, element);
}
}
if( SharedInt32!=0) {
size += pb::CodedOutputStream.ComputeInt32Size(30,SharedInt32);
}
if( SharedString!="") {
size += pb::CodedOutputStream.ComputeStringSize(31,SharedString);
}
if( SharedMessage!=null) {
size += pb::CodedOutputStream.ComputeMessageSize(32,SharedMessage);
}
if( SharedInt32Array!=null) {
foreach (var element in SharedInt32Array) {
size += pb::CodedOutputStream.ComputeInt32Size(35, element);
}
}
if( SharedEnum!=0) {
size += pb::CodedOutputStream.ComputeEnumSize(36, (int)SharedEnum);
}
if( Int8Value!=0) {
size += pb::CodedOutputStream.ComputeInt32Size(33,Int8Value);
}
if( Int16Value!=0) {
size += pb::CodedOutputStream.ComputeInt32Size(34,Int16Value);
}
if( ImportedValue!=null) {
size += pb::CodedOutputStream.ComputeMessageSize(40,ImportedValue);
}
return size;
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

public override void MergeFrom(pb::CodedInputStream input) {
uint tag;
int sharedNum = 0;
while (input.ReadTag(out tag)) {
switch (tag) {
case 0: {
throw pb::InvalidProtocolBufferException.InvalidTag();
}
default: {
if (pb::WireFormat.IsEndGroupTag(tag)) {
return;
}
UnityEngine.Debug.Log("unknown field " + tag);
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
public override void WriteTo(pb::CodedOutputStream output) {
if( Val!=0) {
UnityEngine.Debug.Log("write Val " + Val);
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
}

}
#endregion Designer generated code
