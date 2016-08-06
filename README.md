# Progocol Buffer プラグイン (protoc-gen-dfcsharp)

オリジナルのC# のプラグインにドラゴンファング用に、下記のカスタマイズをしたものです。

- リフレクションを使わないように変更
- 読み取り専用だったプロパティを読み書き可能に

MacOS向けのビルド済みのものが Tools/Bin の下に含まれています。



## ビルド方法

以下、MacOS X で homebrew がセットアップされている状態での下記のコマンドを実行します。

    $ brew install --devel protobuf     # MEMO: protobufのversion 3以上をインストールするために --devel を指定している
	$ make



## 使い方

protocプラグインの標準的な使い方です。

    $ protoc --plugin=./protoc-gen-dfcsharp --dfcsharp_out=output_dir input.proto
	
See: https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.compiler.plugin



## オリジナルの情報

オリジナルのソースコードは、下記のものです。

https://github.com/google/protobuf/tree/protoc-alpha-3.1

`src/google/protobuf/compiler/csharp` ディレクトリ

