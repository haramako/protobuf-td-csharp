using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using System.Runtime.Serialization.Json;
using Google.ProtocolBuffers;
using NUnit.Framework;

namespace Test
{
	[TestFixture]
	public class ProgocolBufferTest {
		[SetUp]
		public void SetUp(){
		}

		// JSONにダンプして比較する
		static bool comp(object a, object b){
            var s = new DataContractJsonSerializer(typeof(TestMessage));
			
			var sa = new MemoryStream();
			s.WriteObject(sa, a);
			var ja = Encoding.UTF8.GetString(sa.ToArray());
		
			var sb = new MemoryStream();
			s.WriteObject(sb, b);
			var jb = Encoding.UTF8.GetString(sb.ToArray());

			if( ja != jb ){
				Console.WriteLine("Difficult!");
				Console.WriteLine(ja);
				Console.WriteLine(jb);
				return false;
			}else{
				return true;
			}
		}

		[Test]
		public void EmptyMessage ()
		{
			var mes = TestMessage.CreateInstance();
			var restored = TestMessage.ParseFrom(mes.Dump());

			Assert.IsTrue(comp(mes, restored));
		}

		[Test]
		public void SimpleMessage ()
		{
			var mes = TestMessage.CreateInstance();
			
			mes.Int32Value = 1;
			mes.Int64Value = 2;
			mes.StringValue = "3";
			mes.EnumValue = TestEnum.BAZ;

			mes.Sint32Value = -5;
			mes.Sint64Value = -6;

			mes.MessageValue = EmbedMessage.CreateInstance();

			mes.RepeatedInt32 = new List<int>(){11,12,13};
			mes.RepeatedInt64 = new List<long>(){12,13};
			mes.RepeatedMessage = new List<EmbedMessage>(){EmbedMessage.CreateInstance()};
			mes.PackedInt32 = new List<int>(){21,22,23};
			mes.PackedInt64 = new List<long>(){32,33};

			Assert.IsTrue(comp(mes, TestMessage.ParseFrom(mes.Dump())));
		}
		
	}
}
