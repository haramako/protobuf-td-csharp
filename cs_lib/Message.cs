using System.IO;
using pb = global::Google.ProtocolBuffers;

namespace Google.ProtocolBuffers {
	public abstract class Message {
		public abstract void MergeFrom(pb::CodedInputStream input);
		public void MergeFrom(Stream input){
			MergeFrom(pb::CodedInputStream.CreateInstance(input));
		}
		public void MergeFrom(byte[] buf){
			MergeFrom(pb::CodedInputStream.CreateInstance(buf));
		}
	}
}
