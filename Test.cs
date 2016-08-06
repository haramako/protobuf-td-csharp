using pb = global::Google.ProtocolBuffers;
using pbc = global::Google.ProtocolBuffers.Collections;
using pbd = global::Google.ProtocolBuffers.Descriptors;
using scg = global::System.Collections.Generic;

public interface IPbMessage {
	void MergeFrom(pb::ICodedInputStream input, pb::ExtensionRegistry extensionRegistry);
}

public class Test {
	public static void Main(){
    }

	public static void Input_ReadMessage(IPbMessage mes, object ex){
	}

	public static void Input_ReadMessageArray<T>(uint tag, out T[] field)
		where T: IPbMessage {
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
