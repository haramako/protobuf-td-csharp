using pb = global::Google.ProtocolBuffers;

namespace Google.ProtocolBuffers {
	public interface IMessage {
		void MergeFrom(pb::CodedInputStream input);
	}
}

public class Test {
	public static void Main(){
    }

	public static void Input_ReadMessage(pb::IMessage mes, object ex){
	}

	public static void Input_ReadMessageArray<T>(uint tag, out T[] field)
		where T: pb.IMessage {
		field = new T[0];
	}

	 public static void Input_ReadStringArray(uint tag, out string[] field){
		 field = null;
	 }
	 public static void Input_ReadInt32Array(uint tag, out int[] field){
		 field = null;
	 }
	 public static void Input_ReadInt64Array(uint tag, out long[] field){
		 field = null;
	 }
}
