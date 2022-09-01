""""""""""""""""""""""""""""""
In static_link/with_archive

[コマンド概要]
1. リンクするオブジェクトファイルを生成
gcc -c -I(.hファイルのパス) vector.c <=　アーカイブでまとめるオブジェクトファイルを生成
gcc -c -I(.hファイルのパス) angle.c  <=  アーカイブでまとめるオブジェクトファイルを生成

2. アーカイブ作成
この時、アーカイブの名前は必ずlib<名前>.aにすること。
ここではmathtoolsという名前をつけた、libmathtools.aアーカイブを作成した。
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

