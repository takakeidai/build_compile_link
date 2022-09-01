Cコードをコンパイルするために必要なツールをディストリビューションごとにインストールしなければいけない。
Ubunts: build-essentialをインストール
CentOS: yum groupinstall "Development Tools"
""""""""""""""""""""""""""""""
In make_makefile

[makeコマンドとMakefileファイル概要]

makefileには、ターゲット、依存関係、ルールを記述する。
makeには暗黙のルールがあるため、ターゲットと依存関係に暗黙のルールを適用できる。
必要ならばそのルールを上書きできる。
依存ファイルは.oオブジェクトファイル

-------------------------------------------------------------------
ターゲット: 依存ファイル1(.o) 依存ファイル2 ...
   [TAB]    コマンド1
            コマンド2
            .
            .
-------------------------------------------------------------------

makeが実行するターゲットは、makeコマンドの引数で指定する。
make hello
make output
make install
make clean
makeだけでターゲットを省略した場合、Makefileに最初に現れるターゲットのみ実行される。
いくつもターゲットがある場合は、最終ゴールとなるターゲットをmakeで指定する。

そもそもmakeはソースコードのコンパイル・ビルドのためというよりも、
指定したルールに基づくコマンドを実行するためのツールである。
そのためビルドした結果の削除などビルドとは直接的に関係ないことでも使われる。
=========================
(Makefile)
clean:
    rm -f *.o hello

(console)
make clean
=========================
上記のcleanのように、依存ファイルがない場合にターゲットを指定してmakeコマンドを
実行した場合、常にコマンドが実行されることになる。

代表的なマクロ
all: = allで指定したターゲットを最終的なターゲットにするマクロ
OBJS: = オブジェクトファイルをまとめるマクロ
CC: =　コンパイラを指定するマクロ
CFLAG: = コンパイラへのオプションを指定するマクロ
LDFLAGS: = リンクへのオプションを指定するマクロ
$(マクロ)の形式でマクロの参照ができる。
ターゲットもマクロも1行で記述する。行を分割するときは[\]を入れる。

[コマンド概要]
--------------------------------------------------------------------------------------------------
PROGRAM = 実行ファイルの名前
OBJS =  リンクするオブジェクトファイル
SRCS = $(OBJS:%.o=%.c)　<= ソースコードのファイル名を指定
                           ここでは、OBJSファイルの拡張子を.cに変える命令をしている
CC = gcc
LD = gcc　<= リンカーのコマンド名
CFLAGS = -g -Wall
LDFLAGS = -I../includes -L../libs -lmathtool  <= リンカーのオプションを指定
                                                .hファイルやシェアードライブラリとそのパスを指定する

all:$(PROGRAM) <= allで、makeでビルドする最終的なターゲットを明確化する
                  allに指定すると、makeだけで$(PROGRAM)まで全てビルドできるようになる
clean:
	rm -rf $(PROGRAM) *.o <= make cleanをするとビルド過程で生成されたファイルが全てrmされる

$(PROGRAM):$(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@  <= $@とはターゲットのことで、ここでは$(PROGRAM)のこと

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ <= .c.oは.oが必要ならば元となるファイルは.cであることを示す
                                    $<は依存ファイル名
                                    $@はターゲットファイル名
-------------------------------------------------------------------------------------------------------

In basic_make_2/
main.c
vector.c
angle.c
vector.h
angle_related.h
extern_variable.h
===============================
PROGRAM = output
OBJS =  main.o vector.o angle.o
SRCS = $(OBJS:%.o=%.c)
CC = gcc
LD = gcc
CFLAGS = -g -Wall
LDFLAGS =
all:$(PROGRAM)
clean:
	rm -rf $(PROGRAM) *.o
$(PROGRAM):$(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
==================================

.hファイルを別のディレクトリにまとめた場合、
-I./includesなどで場所をリンカーに教える必要がある。
仮に、シェアードリンクを自作して別のディレクトリに配置する場合は、
-L./libsなどのシェアードリンクのパスをLDFLAGSに渡す。

In basic_make_3/
main.c
vector.c
angle.c
includes/
===============================
PROGRAM = output
OBJS =  main.o vector.o angle.o
SRCS = $(OBJS:%.o=%.c)
CC = gcc
LD = gcc
CFLAGS = -g -Wall
INCLUDES = -I./includes
LDFLAGS = $(INCLUDES)
all:$(PROGRAM)
clean:
	rm -rf $(PROGRAM) *.o
$(PROGRAM):$(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@
.c.o:
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
==================================


