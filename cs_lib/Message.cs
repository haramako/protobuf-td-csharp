using System.IO;
using pb = global::Google.ProtocolBuffers;
using System.Collections.Generic;

namespace Google.ProtocolBuffers {
    public struct SharedItem
    {
        public int Id;
        public object Value;
        public SharedItem(int id, object val)
        {
            Id = id;
            Value = val;
        }

        static List<SharedItem> tempList = new List<SharedItem>();

        static public void PushTemp(int id, object val)
        {
            tempList.Add(new SharedItem(id, val));
        }

        static public SharedItem[] PopTemp(int num)
        {
            var result = new SharedItem[num];
            int head = tempList.Count - num;
            for (int i = 0; i < num; i++)
            {
                result[i] = tempList[head + i];
            }
            tempList.RemoveRange(head, num);
            return result;
        }
    }

    public abstract class Message {
		public abstract void MergeFrom(pb::CodedInputStream input);
		public abstract void WriteTo(pb::CodedOutputStream output);
		public abstract int SerializedSize { get; }
		public abstract void Init();
		public abstract void Finish();
		
		public void MergeFrom(Stream input){
			MergeFrom(pb::CodedInputStream.CreateInstance(input));
		}
		public void MergeFrom(byte[] buf){
			MergeFrom(pb::CodedInputStream.CreateInstance(buf));
		}

		public void WriteTo(Stream output){
			var cos = CodedOutputStream.CreateInstance(output);
			WriteTo(cos);
			cos.Flush();
		}

		public void WriteTo(byte[] buf, int pos, int len){
			var cos = CodedOutputStream.CreateInstance(buf, pos, len);
			WriteTo(cos);
			cos.Flush();
		}

		public byte[] Dump(){
			var ms = new MemoryStream();
			WriteTo(ms);
			return ms.ToArray();
		}
		
	}
}
