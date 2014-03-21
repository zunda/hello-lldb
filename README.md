# hello-lldb
このプロジェクトは、`lldb`で共有ライブラリのソースコードを見られる条件を見つけるために作成されました。

* `CFLAGS`に`-g`が含まれて、
* ソースコードがビルド時のパスにある場合には、

`lldb`でソースコードを見られることがわかりました。

## 使い方
### ビルトとインストール
```
$ CFLAGS="-g -O0" ./configure --prefix=$HOME/local
$ make
$ make install
```

### lldbを使ってみる
共有ライブラリのソースコードを見られる場合には下記のような出力が得られます。
この例はMac OS 10.9.2で得られたものです。

```
$ lldb $HOME/local/bin/hello
Current executable set to '/Users/zunda/local/bin/hello' (x86_64).
(lldb) br set -F greet
Breakpoint 1: where = libhello.0.dylib`greet + 42 at hello.c:7, address = 0x0000000000000f4a
(lldb) run
Process 19124 launched: '/Users/zunda/local/bin/hello' (x86_64)
1 location added to breakpoint 1
Process 19124 stopped
* thread #1: tid = 0xea9dd, 0x0000000100003f4a libhello.0.dylib`greet(buf=0x00007fff5fbffc00, size=80, name=0x0000000100000f92) + 42 at hello.c:7, queue = 'com.apple.main-thread', stop reason = breakpoint 1.2
    frame #0: 0x0000000100003f4a libhello.0.dylib`greet(buf=0x00007fff5fbffc00, size=80, name=0x0000000100000f92) + 42 at hello.c:7
   4   	int
   5   	greet(char *buf, size_t size, const char *name)
   6   	{
-> 7   		return snprintf(buf, size, "Hello, %s!\n", name);
   8   	}
```

なお、`lldb`の実行前にソースツリーを別のパスに`mv`した場合には、
下記と同様ソースコードを見ることはできませんでした。

コンパイラオプションから`-g`オプションを削除し、
`CFLAGS=-O0`とした場合には下記のようにソースコードを見ることができませんでした。
この結果は、最適化フラグを`-O0`から`-O3`に変更した場合も同様でした。

```
$ lldb $HOME/local/bin/hello
Current executable set to '/Users/zunda/local/bin/hello' (x86_64).
(lldb) br set -F greet
Breakpoint 1: where = libhello.0.dylib`greet, address = 0x0000000000000f20
(lldb) run
Process 21067 launched: '/Users/zunda/local/bin/hello' (x86_64)
1 location added to breakpoint 1
Process 21067 stopped
* thread #1: tid = 0xeb386, 0x0000000100003f20 libhello.0.dylib`greet, queue = 'com.apple.main-thread', stop reason = breakpoint 1.2
    frame #0: 0x0000000100003f20 libhello.0.dylib`greet
libhello.0.dylib`greet:
-> 0x100003f20:  pushq  %rbp
   0x100003f21:  movq   %rsp, %rbp
   0x100003f24:  subq   $0x20, %rsp
   0x100003f28:  movl   $0x0, %eax
```

### ソースコードのパス
コンパイラオプションに`-g`を含めてビルドしたライブラリや実行ファイルが
`strip`されていない場合には、ソースコードのパスを得ることができそうです。

実行ファイルだと、

```
$ file $HOME/local/bin/hello
/Users/zunda/local/bin/hello: Mach-O 64-bit executable x86_64
$ strings - $HOME/local/bin/hello  | grep ^/
/usr/lib/dyld
/Users/zunda/local/lib/libhello.0.dylib
/usr/lib/libSystem.B.dylib
/Users/zunda/local/src/hello-lldb/
/Users/zunda/local/src/hello-lldb/main.o
```

共有ライブラリだと、

```
$ file $HOME/local/lib/libhello.a
/Users/zunda/local/lib/libhello.a: current ar archive random library
$ strings - $HOME/local/lib/libhello.a | grep ^/
/Users/zunda/local/src/hellowrold
```

ここで`string`に`-`オプションを付けない場合には、
オブジェクトファイルの`(__TEXT,__text)`セクションの内容は表示されず(よくわかっていない)、
実行ファイルからソースコードのパスを得ることができなくなってしまうようです。

```
$ strings $HOME/local/bin/hello
World
```

### 参考文献
* [GDBでデバッグ時にソースコードの場所を指定する | Urban Theory](http://blog.urban-theory.net/2013/05/25/specifing_source_directories_with_gdb)
