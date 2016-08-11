using System;
using System.Linq;
using System.Collections.Generic;
using System.IO;
using Master;
using Google.ProtocolBuffers;

public class PbFile {


	public static List<TMessage> ReadPbList<TMessage, TBuilder>(TMessage prototype, byte[] buf)
		where TMessage : IMessage<TMessage, TBuilder>
		where TBuilder : IBuilder<TMessage, TBuilder>
	{
		var result = new List<TMessage> (700);
		var cis = CodedInputStream.CreateInstance (buf,0,buf.Length);
		var magic = cis.ReadRawByte ();
		if( magic != 'C' ) throw new ArgumentException("Invalid stream header");
		var headerSize = cis.ReadRawByte ();
		cis.SkipRawBytes (headerSize); // 読み飛ばすだけ

		var builder = prototype.ToBuilder ();
		uint size = 0;
		TMessage message;
		while( !cis.IsAtEnd ){
			cis.ReadFixed32 (ref size);
			if (size <= 0) break; // 0 パディングされている場合
			var oldLimit = cis.PushLimit((int)size);
			message = builder.Clear().MergeFrom(cis).Build();
			cis.PopLimit (oldLimit);
			result.Add(message);
		}
		return result;
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
