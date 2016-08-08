using System;
using System.IO;
using System.Linq;
using System.Diagnostics;

namespace cs_test
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Console.WriteLine ("Hello World!");
			var buf = File.ReadAllBytes ("/Users/makoto/u5/dragonfangdata/UploadFiles/master-fang_template.pb");

			var watch = new Stopwatch();
			watch.Start();
			for (var i = 0; i < 100; i++) {
				var fangs = PbFile.ReadPbList<Master.FangTemplate> (buf).ToList ();
				//Console.WriteLine ("" + fangs.Count);
			}
			watch.Stop();
			Console.WriteLine(" Time Elapsed {0} ms", watch.ElapsedMilliseconds);
		}
	}
}
