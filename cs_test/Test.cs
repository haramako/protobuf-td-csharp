using pb = global::Google.ProtocolBuffers;

namespace Google.ProtocolBuffers {
	public interface IMessage {
		void MergeFrom(pb::CodedInputStream input);
	}
}
