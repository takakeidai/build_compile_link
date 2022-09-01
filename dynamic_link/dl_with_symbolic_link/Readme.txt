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

