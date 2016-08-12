using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Google.ProtocolBuffers;

public class PbFile {

	public static byte[] buf = new byte[1024 * 1024];

	public static IEnumerable<T> ReadPbList<T>(byte[] buf) where T: Message, new()
	{
		//var len = s.Read (buf, 0, buf.Length);
		var cis = CodedInputStream.CreateInstance (buf,0,buf.Length);
		var magic = cis.ReadRawByte ();
		if( magic != 'C' ) throw new ArgumentException("Invalid stream header");
		var headerSize = cis.ReadRawByte ();
		cis.SkipRawBytes (headerSize); // 読み飛ばすだけ

		uint size = 0;
		while( !cis.IsAtEnd ){
			T message = new T();
			if (!cis.ReadFixed32 (ref size)) {
				break;
			}
			// TODO: サイズ０（すべてデフォルト値）の時に問題がおこるので対策を案が得ること
			if (size <= 0) break; // 0 パディングされている場合
			var oldLimit = cis.PushLimit((int)size);
			message.MergeFrom(cis);
			cis.PopLimit (oldLimit);
			yield return message;
		}
	}

	// Copy From Google.ProtocolBuffers.AbstractBuilderList.cs

	/// <summary>
	/// Stream implementation which proxies another stream, only allowing a certain amount
	/// of data to be read. Note that this is only used to read delimited streams, so it
	/// doesn't attempt to implement everything.
	/// </summary>
	public sealed class LimitedInputStream : Stream
	{
		private readonly Stream proxied;
		private int bytesLeft;

		internal LimitedInputStream(Stream proxied, int size)
		{
			this.proxied = proxied;
			bytesLeft = size;
		}

		public override bool CanRead
		{
			get { return true; }
		}

		public override bool CanSeek
		{
			get { return false; }
		}

		public override bool CanWrite
		{
			get { return false; }
		}

		public override void Flush()
		{
		}

		public override long Length
		{
			get { throw new NotSupportedException(); }
		}

		public override long Position
		{
			get { throw new NotSupportedException(); }
			set { throw new NotSupportedException(); }
		}

		public override int Read(byte[] buffer, int offset, int count)
		{
			if (bytesLeft > 0)
			{
				int bytesRead = proxied.Read(buffer, offset, Math.Min(bytesLeft, count));
				bytesLeft -= bytesRead;
				return bytesRead;
			}
			return 0;
		}

		public override long Seek(long offset, SeekOrigin origin)
		{
			throw new NotSupportedException();
		}

		public override void SetLength(long value)
		{
			throw new NotSupportedException();
		}

		public override void Write(byte[] buffer, int offset, int count)
		{
			throw new NotSupportedException();
		}
	}




}
