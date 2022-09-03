Cコードをコンパイルするために必要なツールをディストリビューションごとにインストールしなければいけない。
Ubunts: build-essentialをインストール
CentOS: yum groupinstall "Development Tools"

"""""""""""""""""""""""""""""""""""""""""
In compile_normally
=====================================
gcc main.c vector.c angle.c -o output
=====================================
-------------------
既存のファイル
vector.c
angle.c
main.c
vector.h
angle_related.h
extern_variable.h
-------------------

--------------------
生成されたファイル
output
--------------------

"""""""""""""""""""""""""""""""""""""""""
In static_link/without_archive

[コマンド概要]
gcc -c -I(ヘッダーファイルの場所指定)　vector.c  <= リンクされるオブジェクトファイルを生成
gcc -c -I(ヘッダーファイルの場所指定)  angle.c   <= リンクされるオブジェクトファイルを生成
gcc -I(ヘッダーファイルの場所指定) main.c (<=最後にリンクするファイル)　vector.o angle.o -o output
(すでに作成したリンクするオブジェクトファイルをパスも含めて指定)
========================================================
gcc -c -I../../includes vector.c
gcc -c -I../../includes angle.c
gcc -I../../includes main.c vector.o angle.o -o output
========================================================

[.hファイルの基本的な書き方：2重インクルードを回避する]
(基本)
================================
#ifndef 大文字ファイル名_H
#define 大文字ファイル名_H

定義する関数・変数・構造体など

#endif
================================
ifndefに続く条件が偽の場合、プリプロセッサは#ifndefから#endifの間の内容をコンパイラに渡さない。

(In vector.h)
==============================
#ifndef VECTOR_H
#define VECTOR_H
typedef struct {
    double x;
    double y;
} Vector;

void ShowVector(Vector *v);
#endif
==============================
------------------
既存のファイル
main.c
vector.c
angle.c
------------------
生成されたファイル
vector.o
angle.o
output
-----------------

""""""""""""""""""""""""""""""
In static_link/with_archive

Cライブラリとはプログラムに組み込めるコンパイラ済みの共通コンポーネントの集まりであり、
実際にはオブジェクトファイルをまとめたもの。
.aで終わるライブラリファイルを静的ライブラリという。

[コマンド概要]
1. リンクするオブジェクトファイルを生成
gcc -c -I(.hファイルのパス) vector.c <=　アーカイブでまとめるオブジェクトファイルを生成
gcc -c -I(.hファイルのパス) angle.c  <=  アーカイブでまとめるオブジェクトファイルを生成

2. アーカイブ作成
この時、アーカイブの名前は必ずlib<名前>.aにすること。
ここではmathtoolsという名前をつけ、libmathtools.aアーカイブを作成した。
ar -rcs libmathtools.a vector.o angle.o
(オプション)
r: .aファイルが既存の場合は上書き
c: フィードバックなしにアーカイブを作成
s: .aファイルの先頭にインデックスを作成

アーカイブファイルは、
(i):  /usr/local/libに格納するか
(ii): 別のディレクトリに格納し、-Lでコンパイル時にパスを指定する

3. アーカイブとリンクさせ実行ファイルを生成
gcc -I(main.cに必要な.hファイルのパス) -L(.aファイルのパス) -lアーカイブ名 main.c -o output 
===============================
gcc -c -I../../includes vector.c
gcc -c -I../../includes angle.c
ar -rcs libmathtools.a vector.o angle.o
gcc -I../../includes -L./ -lmathtools main.c -o output
===============================
------------------
既存のファイル
main.c
vector.c
angle.c
------------------
生成されたファイル
vector.o
angle.o
libmathtools.a
output
-----------------

""""""""""""""""""""""""""""""
In dynamic_link

[ダイナミックリンク]
シェアードライブラリ(共有ライブラリ/ダイナミックリンクライブラリ(DLL))でリンクすることを
ダイナミックリンクと呼ぶ。
シェアードライブラリは、アーカイブと同様に複数の.oオブジェクトファイルから作成される。
シェアードライブラリには、オブジェクトコードに加えOSがライブラリを他のものとリンクさせるために
必要な情報が含まれており、言わばメタデータを備えた再配置可能なオブジェクトファイルと言える。
別の言い方をすれば、共有ライブラリに対してプログラムはリンクすると、最終的な実行ファイルにコードが
コピーされるのではなく、ライブラリファイルのコード内の名前への参照が追加されるだけ。
プログラムを実行する場合、システムは必要な時だけライブラリのコードをプロセスのメモリ空間にロードする。
多くのプロセスが同じ共有ライブラリのコードをメモリ上で共有できる。
LinuxとMacOSとでは扱い方が異なる点が多少あるので注意。

Linuxディストリビューションのソフトウェアをアップデートする際に、アップデートするパッケージに共有ライブラリが
含まれていることがある。
アップデートマネージャがマシンの再起動を求めるのは、場合によってはシステムの各部分が新しいバージョンの共有ライブラリを
使うためである。


[コマンド概要]
1. リンクするオブジェクトファイルを生成
gcc -c -I(.hファイルのパス) -fPIC vector.c <=　シェアードライブラリに含める.oファイルを生成
gcc -c -I(.hファイルのパス) -fPIC angle.c  <=  シェアードライブラリに含める.oファイルを生成
gcc -c -I(.hファイルのパス) -fPIC different_angle.c <=  シェアードライブラリに.oファイルを生成

2. シェアードライブラリ作成：-shared オプションをつける
gcc -shared シェアードライブラリに含める複数の.oファイル　-o ../libs/libmathtool_1.dylib
最後に、macではlib<名前>.dylib、Linuixではlib<名前>.soとなるシェアードライブラリを
配置するパスを指定して記述する。この時-oオプションを忘れずに。
angle.cとdifferent_angle.cで異なるシェアードファイルを作成する。
シェアードライブラリを2つ作り、実行ファイルごとに別々にリンクさせる。
gcc -shared vector.o angle.o -o ../libs/libmathtool_1.dylib
gcc -shared vector.o different_angle.o  -o ../libs/libmathtool_2.dylib

3. ダイナミックリンクを行い、実行ファイルを作成する
(main.cとlibmathtool_1をリンクさせる)
gcc -I(main.cで必要な.hファイルのパス) -L(libmathtool_1のパス) -lmathtool_1 main.c -o output
-L指定をしてシェアードライブラリが配置されている場所を指定する。
そして-l<名前>でシェアードライブラリを指定する。

(main.cとlibmathtool_2をリンクさせる)
gcc -I(main.cで必要な.hファイルのパス) -L(libmathtool_2のパス) -lmathtool_2 main.c -o output_2

あるいは上の作業を別々に行う。
gcc -I(main.cに必要な.hファイルのパス) main.c -o main.o
gcc -L(シェアードライブラリのパス) -lmathtool_1 -o output

==================================================================
gcc -c -I../includes -fPIC vector.c -o vector.o
gcc -c -I../includes -fPIC angle.c -o angle.o
gcc -c -I../includes -fPIC different_angle.c -o different_angle.o
gcc -shared vector.o angle.o ../libs/libmathtool_1.dylib
gcc -shared vector.o different_angle.o ../libs/libmathtool_2.dylib
gcc -I../includes -L../libs -lmathtool_1 main.c -o output
gcc -I../includes -L../libs -lmathtool_2 main.c -o output_2
=================================================================
------------------
既存のファイル
main.c
vector.c
angle.c
different_angle.c
------------------
生成されたファイル
vector.o
angle.o
different_angle.o
libmathtool_1.dylib
libmathtool_2.dylib
output
output_2
-----------------

[ダイナミックリンクの特徴]
ダイナミックリンクで使用するシェアードライブラリ(libmathtool.dylib)には、
関数名などのシンボルが登録されている。
nmコマンドで、nm libmathtool.dylibで確認できる。
このシンボルが一致しているファイルを動的に探してリンクする。
全く異なる機能を有する関数であってもシンボルが一致ならば、ソースコードとのリンクも問題なく行える。
この性質を生かして、angle.cとdifferent_angle.cに異なる処理をする同じ名前の関数を用意して、
その2つで使われる関数定義を、angle_related.hで共通して行う。
main.cではangle_related.hをインクルードすることにする。
angle.cとdifferent_angleのどちらかからgcc -shared vector.o angle.o -o libmathtool.dylibを作成し、
このシェアードライブラリを用いてmain.cとダイナミックリンクさせる。
libmathtool.dylibが参照しているシンボルがangle.cにもdifferent_angle.cにも存在するため、
1つのシェアードライブラリからダイナミックリンクできる。
=========================================================
gcc -shared vector.o angle.o -o ./libmathtool.dylib
gcc -I../includes -L../libs -lmathtool main.c -o output
gcc -I../includes -L../libs -lmathtool main.c -o output_2
=========================================================
outputは、main.cとlibmathtool_1をリンクさせ、
output_2は、main.cとlibmathtool_2をリンクさせて実行ファイルを作っている。
*実装は、./henachoko_practiceを参照せよ。
hena.c、choko.cには同一の名前の関数が記述されているが全く内容は異なる。
関数の宣言に共通のhenachoko.hファイルを用意して、main.cにインクルードさせている。
なおこの方法がグッドノウハウかは現時点では不明。
各用途とソースコードごとにシェアードライブラリを作成したほうがいいのかもしれない。

[ダイナミックリンクのメリット]
スタティックリンクでは、main.cとリンクさせたいライブラリのソースコードが変わると、
オブジェクトコードを生成し直し、新しいオブジェクトコードからアーカイブを生成して、
最後に、main.cとリンクさせる必要があった。
ダイナミックリンクでは、main.cとリンクさせたいライブラリのソースコードが変わっても、
ソースコードを書き直したら、main.cだけコンパイルしなおせばいい。
つまり、vector.c/angle.c/different_angleのいずれのファイルが変更されても、
gcc -I(.hファイルパス) -L(リンクさせたいシェアードライブラリ) main.c
========================================
gcc -I../includes -L./ -lmathtool main.c
========================================
だけでmain.cから最新のa.outが生成される。
この時リンクさせるシェアードライブラリは、変更したソースコードを含むもの。

[Linuxの場合のいくつかの注意点]
.hファイルに関して、/usr/local/includeに格納されていれば、-Iオプションはいらない。
シェアードライブラリに関して、Macでプログラムをコンパイルすると、シェアードライブラリへの
フルパスが実行ファイル内に格納されるが、Linuxの場合、/usr/lib以外に格納すると見つけてくれない。
シェアードライブラリのパスを実行ファイルに与える必要がある。
方法は主に2つ
1. コンパイル時に-rpathでパスを指定する方法
main.cにリンクするシェアードライブラリが/opt/obscure/libにあると仮定する。
===============================================================================
gcc -I../includes -rpath=/opt/obscure/lib -L/opt/obscure/lib main.c -o output
===============================================================================
-rpathオプションを指定しても-Lオプションは常に必要である。

2. シェアードライブラリが置かれているパスをLD_LIBRARY_PATH環境変数に格納する方法
=============================================================================================
gcc -I../includes -L/opt/obscure/lib main.c -o output
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/obscure/lib
./output
==============================================================================================
ただし、2つめの方法は基本的に用いない方がいい。
LD_LIBRARY_PATHにパスを指定すると、ld.soは指定したディレクトリを何度も探索するため効率が悪くなるなどの
デメリットが発生する。

一方、Macの場合、.hファイルが/usr/local/includeに入っており、かつシェアードライブラリが、
/usr/libに入っていれば、main.cのコンパイルは
========================
gcc main.c
========================
だけでいいことになる。

(実行ファイルがどの共有ライブラリを使っているかを確認する方法)
最後に実行可能ファイルがどういったシェアードライブラリとリンクしているのかということは、
lddコマンド、Macではotool -Lで確認できる。
実行ファイルは通常共有ライブラリの場所を知らない。
ld.soというローダーが実行時にプログラムの共有ライブラリを見つけ出してロードする。
============================
ldd /bin/bash

otool -L ./a.out
============================
間違ったシェアードライブラリにリンクしてないか確かめるようにする。


""""""""""""""""""""""""""""""
In dynamic_link/dl_with_symbolic_link

[コマンド概要]
1. シェアードライブラリの実体ファイルを生成する
2. シェアードライブラリのシンボリックリンクを生成する 
4. シェアードライブラリのシンボリックリンクでダイナミックリンクする
5. 変更したソースコードとそれらのオブジェクトファイルからシェアードライブラリを作成する
   この時、ファイルのマイナーバージョンを変えて区別する
6. 問題なく動作すレバ、メジャーバージョンを変えて実体ファイルからシンボリックリンクを作り直す
   古いバージョンのリンクは削除しても大丈夫。
   古いバージョンの実体ファイルは残っているから必要なら戻せる。
7. 問題があれば、古いバージョンのシェアードライブラリのシンボリックリンクから
   実体ファイルへ内容を復旧する

以下では、./libsディレクトリのlibmathtool.1.0.dylibが実体ファイル
./sym_libsディレクトリのlibmathtool.1.dylibがシンボリックリンクとする
===============================
gcc -c -fPIC -I../../includes vector.c -o vector.o
gcc -c -fPIC -I../../includes angle.c -o angle.o
gcc -shared vector.o angle.o -o ./libs/libmathtool.1.0.dylib
ln -s ./libs/libmathtool..0.dylib ./sym_libs/libmathtool.1.dylib
gcc -I../../includes -L./sym_libs -lmathtool.1 main.c -o output
===============================

vector.cとangle.cのソースコードを変更後に、
./libs/libmathtool.1.1.dylibとしてシェアードライブラリを新たに作成する。
このlibmathtool.1.1.dylibで問題なく動けば、このファイルのシンボリックリンクを生成する。
もし問題があれば、libmathtool.1.1.dylibを削除してソースコードを描き直す。
=======================
vim angle.c (<= angle.cを変更)
gcc -c -fPIC -I../../includes angle.c -o angle.o
gcc -shared vector.o angle.o -o ./libs/libmathtool.1.1.dylib
gcc -I../../includes -L./libs -lmathtool.1.1 main.c -o updated_output

(問題なければ)
ln -s ./libs/libmathtool.1.1.dylib ./sym_libs/libmathtool.2.dylib (<= シンボリックリンクをアップデート)
rm ./libs/sym_libs/libmathtool.1.dylib (<= 古いバージョンのシンボリックリンクを削除)

(問題あれば)
rm ./libs/libmathtool.1.1.dylib (<=動かない最新のシェアードライブラリの実体を削除)
vim angle.c
からやり直し
=======================

""""""""""""""""""""""""""""""""""""""""""""
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
---------------------------------------------------------------------------------------------
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
----------------------------------------------------------------------------------------------

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
allを設定しているため、makeだけでoutputまでコンパイルする。
======
make
======

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


""""""""""""""""""""""""""""""
In build_software/Download_sf.txt

[ソフトウェアのソースコード(圧縮ファイル)のダウンロード方法]

1. webサイトからソースコードをダウンロード
wgetコマンドで、サイトからソースコードをダウンロードする。
wgetコマンドとは、ソースコードやバイナリコードをダウンロードするためのコマンド。
wget URLで実行できる。
curlコマンドでも同じようなことはできるがwgetの方がダウンロードに関しては使い勝手が良い。
wget http:ftp.gnu.org/gnu/hello/hello-2.7.tar.gz
wgetがインストールされていなければ、
sudo install wget
でインストールしてから実行
配布パッケージの形式としては、tar.gzまたはtar.bz2が主流。
ソースコードをアーカイブにまとめて圧縮して配布している。

2. ソースコードの真正判断
・ハッシュ値で確認する方法
・ソフトウェアが提供する公開鍵をサーバーに配置して、電子署名で確認する方法

<ハッシュ値で確認する方法>
md5sumコマンドやsha1sumコマンドでハッシュ値がダウンロードした圧縮ファイルと一致するか確かめる。

<電子署名での確認>
電子署名での検証には、PGPという暗号ソフトウェアを使う。
ソフトウェア名はGnuPG(gnupg)で、コマンド名はgpg。
まずgnupgをインストールする。
その後、webサイトから電子署名のファイルをダウンロードして、
gpg --verifyコマンドで署名が正しいのものか確認する。

wgetのインストールとソフトウェアのソースコード(圧縮ファイル)のダウンロード
TEST_DIRディレクトリにソースコードをダウンロードすると仮定する。
=============================================================
mkdir TEST_DIR 
cd TEST_DIR

sudo install wget
wget https://downloads.apache.org/httpd/httpd-2.4.54.tar.bz2
=============================================================
TEST_DIR/
httpd-2.4.54.tar.bz2

sha256のハッシュ値をwebサイトからダウンロード
===================================================================
wget https://downloads.apache.org/httpd/httpd-2.4.54.tar.bz2.sha256
===================================================================
TEST_DIR/
httpd-2.4.54.tar.bz2 
httpd-2.4.54.tar.bz2.sha256

1または2の方法でハッシュ値が一致するか確認
============================================
1. sha256sum -b httpd-2.4.54.tar.bz2
2. sha256sum -c httpd-2.4.54.tar.bz2.sha256
============================================

gnupgをインストール
その後、--list-keysで設定ファイルの自動生成
そしてwebサイトから電子署名ファイルのダウンロード
=================================================================
sudo yum install gnupg
gpg --list-keys
wget https://downloads.apache.org/httpd/httpd-2.4.54.tar.bz2.asc
=================================================================
TEST_DIR/
httpd-2.4.54.tar.bz2 
httpd-2.4.54.tar.bz2.sha256
httpd-2.4.54.tar.bz2.asc

署名を確認する方法
============================================================
gpg --verify httpd-2.4.18.tar.gz.asc
===========================================================

もしこのようにCan't check signature: No public keyとエラーが出た場合、
2つの方法で電子署名をクリアできる。
-----------------------------------------------------------
gpg --verify foobar.asc
gpg: Signature made xxx xxx xx xx:xx:xx xxxx
gpg:                using RSA key XXXXXXXXXXXXXXXX
gpg: Can't check signature: No public key
------------------------------------------------------------

1. 電子署名の公開鍵をpgp.mit.eduにある鍵サーバーから
RSA keyを使って取ってくる方法
XXXXXXXXXXXXXXXXのところは「using RSA key XXX.......」で与えられたRSA keyを使う。
===========================================================
gpg --keyserver pgp.mit.edu --search-keys XXXXXXXXXXXXXXXXX
gpg --verify httpd-2.4.54.tar.bz2.asc
===========================================================

2. 署名者の公開鍵を手動で取得(インポート)する方法
鍵サーバーに公開鍵が登録されていなくて取得できない場合にサイトから取得する
webサイトにある公開鍵ファイル(KEYS)をwgetでダウンロード後に、
--importオプションで公開鍵ファイルをインポート
その後署名を確認する
=========================================================
wget https://downloads.apache.org/httpd/KEYS
gpg --import KEYS
gpg --verify httpd-2.4.18.tar.gz.asc
=========================================================
TEST_DIR/
httpd-2.4.54.tar.bz2 
httpd-2.4.54.tar.bz2.sha256
httpd-2.4.54.tar.bz2.asc
KEYS

次回のためにGNU Helloからhello-2.12.tar.gzを
gnuhello/ディレクトリを作ってそこにダウンロードしておく。
=====================================================
mkdir gnuhello
cd gnuhello
wget https://ftp.gnu.org/gnu/hello/hello-2.12.tar.gz
=====================================================


""""""""""""""""""""""""""""""
In build_software/Tar_Config.txt

圧縮ファイルを展開する
gnuhelloディレクトリで圧縮ファイルを展開すると、
gnuhelloディレクトリ配下に新しいhello-1.12ディレクトリができる
そこに全てが入っている
ファイル名が大文字になっているものはほとんどドキュメント
=============================================================
tar zxvf hello-2.12.tar.gz
cd hello-1.12
ls -p
=============================================================
tarコマンドについて
(オプション)
z: gzip形式
x: アーカイブを展開
v: 処理したファイルを詳しく出力
f: アーカイブファイル名を指定

圧縮ファイルの展開後、ディレクトリに移動し、ビルドする
(基本ビルド方法)
===================
./configure
make
sudo make install
===================

[最低限確認すべきドキュメント]

README: ソフトウェアの紹介や作者の連絡先やインストール手順が書かれているファイル
ChangeLog/CHANGES: バージョンごとの変更点がまとめられているファイル
INSTALL: インストール手順をまとめたファイル
COPYING/LICENSE: ライセンス条項が記されているファイル
代表的なライセンスにGPLがある。これはFOSSから派生したソフトウェアを再配布する時GPLで配布しなければならないという制約がある。
派生したソフトウェアの販売は自由だが、入手した人が無料で配布するのもまた自由になってしまうためそのようなライセンスで公開されているFOSSを
ビジネスで使えないケースがある。

[Makefileの生成とconfigureファイル]

[最低限確認すべきドキュメント]

README: ソフトウェアの紹介や作者の連絡先やインストール手順が書かれているファイル
ChangeLog/CHANGES: バージョンごとの変更点がまとめられているファイル
INSTALL: インストール手順をまとめたファイル
COPYING/LICENSE: ライセンス条項が記されているファイル
代表的なライセンスにGPLがある。
これはFOSSから派生したソフトウェアを再配布する時GPLで配布しなければならないという制約がある。
派生したソフトウェアの販売は自由だが、
入手した人が無料で配布するのもまた自由になってしまうためそのようなライセンスで公開されているFOSSを
ビジネスで使えないケースがある。

[Makefileの生成とconfigureファイル]

展開したhello-2.12ディレクトリにはまだMakefileが存在しない。
つまり、このままではmakeコマンドを実行できないということ。
Makefileを生成するためには、展開したhello-2.12に入っている、configureと呼ばれるシェルスクリプトを実行する必要がある。
configureは環境差異を吸収し環境に適しているMakefileを自動生成するシェルスクリプトである
configureのおかげでCentOSでもDebianでもFreeBSDでもそれぞれの環境にあったMakefileを手に入れられる。
============
./configure
===========

configureの役割は2つ
1. 環境の違いを吸収して移植性高いMakefileを生成すること
2. ビルド時にユーザーが設定を変更できるようにすること
configureスクリプトで設定する主な項目は、
・インストールディレクトリ
・コンパイラやそのオプション
・機能や追加オプションのオン・オフ
・ソフトウェア実行時の設定デフォルト値

configure実行時に設定内容を指定する方法には、コマンドライン引数(オプション)と環境変数がある。

<インストールディレクトリを変更する>
--prefix=ディレクトリ名で指定する
======================================
./configure --prefix=/opt/hello-2.12
make
sudo make install
======================================

/opt/hello-2.12の確認と実行
========================
ls -p /opt/hello-2.12
=> bin/ share/
ls -p /opt/hello-2.12/bin
=> hello

/opt/hello-2.12/bin/hello
=> Hello, World!
=========================

(注意)
======================================
./configure --prefix=/opt
make
sudo make install
======================================
としてしまうと、optディレクトリが上書きされてしまう。

ソフトウェアの設定を変更したい場合は、オプション指定または環境変数指定で設定を与えてビルドしなおす。
1度ビルドしたならば--prefixで同じ場所を指定して、その他のオプションをつけてビルドし直せば、上書きされる。
==================================================
./configure --prefix=/opt/hello_dir --disable-nls
make
sudo make install
==================================================

--helpオプションでソフトウェア固有の設定にどのようなものが存在しているのかわかる。
===========================
./configure --help=short
==========================

[依存関係にあるモジュールのインストール]
(実例)
apacheをビルドしようとしたら、aprとapr-utilが依存していて、存在しないため
apacheのビルド前に、aprとapr-utilをビルドした。
apr-utilのビルド中にexpat.hがないというエラーが出たため、apr-utilのためにexpatをビルド
apr, apr-util, expatをビルド後に、再度apacheをビルドし成功。
依存するソフトウェアのパスを--with-aprなどとオプション指定して、
configureスクリプトを実行することで解決する。
with引数のオプションは、外部ソフトウェアを組み込むか否かの指示と、
外部ソフトウェアが存在するディレクトリまでのパスを指示する2つの役割がある。
外部ソフトウェアが必須ならば前者の指定はいらない。また、外部ソフトウェアが/usrなどの
標準的な場所に入っている場合も指定はいらない。
yumコマンドなどでインストールした外部ソフトウェアは標準ディレクトリに自動で配置されるため、withでの指定はいらない。
1から手動でビルドして別の場所においた場合はwithで指定する。


[ビルドまとめ]
webサイトからwgetで圧縮ファイルをダウンロードする
gpgで電子署名を確認し、圧縮ファイルが本物であることを確認する
bunzip2/tarで圧縮ファイルを展開し、ディレクトリに移動
./configureでスクリプトファイルconfigureを実行する
もし依存関係が解決されずにエラーがでたら
依存しているソフトウェアをビルドする
make
sudo make install
完了

----------------------------------------------------------------------------
(復習)
[環境変数]
環境変数とは、OS上で保持され、全てのプロセスで設定・参照・共有できる変数。

(追加)
1. export FOO="bar"
2. FOO="bar"
   export FOO

(確認)
1. printenv
2. export -p

(削除)
export -n FOO

.bashrcや.bash_profileに環境変数を書き込んで保存しない限り、
環境変数の生存時間はシェルを閉じるまでとなる。

一方、次に打つコマンドに対してだけ環境変数を適用・変更したいような場合は、
env LANG=ja_JP.UTF-8 ./hello
などと、envコマンドを使う。


----------------------------------------------------------------
In build_software/Yum.txt

ビルド時に必要になった依存関係にある外部ソフトウェアはパッケージ管理yumコマンドを使って、
OSやディストリビューションから引っ張ってくる方がベター。
もちろん、OSやディストリビューションを探しても見つからなかったり、バージョンが古くて使えなかったりする場合は、ソースコードからビルドするしかない。

依存関係にあるソフトウェアを検索して、
インストールする。
=====================
yum search zlib
sudo yum install zlib
======================

""""""""""""""""""""""""""""""
In real_build/nginx_build.txt

・Nginxをビルドする
    必要なソフトや依存関係を整理する
・Nginxをsystemdで操作できるようにする
    systemdで操作できるように設定ファイルを作成する

[Download Nginx]
====================================================
mkdir nginx_dir
cd nginx_dir

wget https://nginx.org/download/nginx-1.23.1.tar.gz
tar -zxvf nginx-1.23.1.tar.gz
cd nginx-1.23.1
====================================================

[Install C compiler]  
====================================
sudo yum groupinstall "Development tools"
====================================

[Dependence package for Nginx]
=================================================================
sudo yum install pcre pcre-devel zlib zlib-devel openssl openssl-devel
=================================================================

[configuration option]
--sbin-path= Nginx Excutable(実行ファイル=実体)が格納される
--conf-path= nginx.confが置かれる場所で、パッケージマネージャーを使う場合と同じ
==========================================================
./configure \
--sbin-path=/usr/bin/nginx \
--conf-path=/etc/nginx/nginx.conf \
--error-log-path=/var/log/nginx/error.log \
--http-log-path=/var/log/nginx/access.log \
--with-pcre \
--pid-path=/var/run/nginx.pid \
--with-http_ssl_module
==========================================================

[Build Nginx]
=====================
make
sudo make install
=====================

[Start, Check and Stop Nginx]
=======================
sudo nginx
ps aux | grep nginx
sudo nginx -s stop
=======================

[Systemdで操作できるように設定ファイルを作成する]
1からビルドしたソフトウェアをsystemdで動かすにはsystemd用にファイルを作成しなければならない。
ここではそのファイルを/lib/systemd/system/においている。
ファイルの具体的書き方はwebなどを参照する。

[Nginx.service]
=====================================
sudo vim /lib/systemd/system/nginx.service
======================================

[Paste script and Modify it]
--------------------------------------------------------------------------------
[Unit]
Description=The NGINX HTTP and reverse proxy server
After=syslog.target network-online.target remote-fs.target nss-lookup.target
Wants=network-online.target

[Service]
Type=forking
#PIDFile=/run/nginx.pid
PIDFile=/var/run/nginx.pid
#ExecStartPre=/usr/sbin/nginx -t
ExecStartPre=/usr/bin/nginx -t
#ExecStart=/usr/sbin/nginx
ExecStart=/usr/bin/nginx
ExecReload=/usr/sbin/nginx -s reload
ExecStop=/bin/kill -s QUIT $MAINPID
PrivateTmp=true

[Install]
WantedBy=multi-user.target
--------------------------------------------------------------------------------

[Start Nginx with systemd(systemctl command)]
===============================
sudo systemctl start nginx
sudo systemctl status nignx
sudo systemctl stop nginx
sudo systemctl enable nginx
sudo systemctl reboot
================================


