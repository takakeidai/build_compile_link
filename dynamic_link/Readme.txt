""""""""""""""""""""""""""""""
In dynamic_link

[ダイナミックリンク]
シェアードライブラリ(共有ライブラリ/ダイナミックリンクライブラリ(DLL))でリンクすることを
ダイナミックリンクと呼ぶ。
シェアードライブラリは、アーカイブと同様に複数の.oオブジェクトファイルから作成される。
シェアードライブラリには、オブジェクトコードに加えOSがライブラリを他のものとリンクさせるために
必要な情報が含まれており、言わばメタデータを備えた再配置可能なオブジェクトファイルと言える。
LinuxとMacOSとでは扱い方が異なる点が多少あるので注意。

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
=====================================
gcc -shared vector.o angle.o -o ./libmathtool.dylib
gcc -I../includes -L../libs -lmathtool main.c -o output
gcc -I../includes -L../libs -lmathtool main.c -o output_2
=====================================
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
======================================
gcc -I../includes -L./ -lmathtool main.c
=====================================
だけでmain.cから最新のa.outが生成される。
この時リンクさせるシェアードライブラリは、変更したソースコードを含むもの。

[いくつかの注意点]
.hファイルに関して、/usr/local/includeに格納されていれば、-Iオプションはいらない。
シェアードライブラリに関して、Macでプログラムをコンパイルすると、シェアードライブラリへの
フルパスが実行ファイル内に格納されるが、Linuxの場合、/usr/lib以外に格納すると見つけてくれない。
そのため、シェアードライブラリが置かれているパスをLD_LIBRARY_PATH環境変数に格納しなければならない。
========================================================================================
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/workspace/comp_link_make_makefile/dynamic_link/libs
./a.out
========================================================================================

一方、Macの場合、.hファイルが/usr/local/includeに入っており、かつシェアードライブラリが、
/usr/libに入っていれば、main.cのコンパイルは
========================
gcc main.c
========================
だけでいいことになる。

最後に実行可能ファイルがどういったシェアードライブラリとリンクしているのかということは、
lddコマンド、Macではotool -Lで確認できる。
============================
ldd /bin/bash

otool -L ./a.out
============================
間違ったシェアードライブラリにリンクしてないか確かめるようにする。








