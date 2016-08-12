using System;
using System.IO;
using Google.ProtocolBuffers;

namespace cs_test
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			if( args.Length != 1 ){
				Console.WriteLine("Invalid args");
				System.Environment.Exit(1);
			}
			var buf = File.ReadAllBytes (args[0]);

			var mes = Test.TestMessage.ParseFrom(buf);

			Console.WriteLine(""+mes.Int32Value);
			Console.WriteLine(""+mes.Int64Value);
		}
	}
}
