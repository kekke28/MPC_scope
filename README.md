# MPC_scope
主にOhata and Nuida, "Communication-Efficient (Client-Aided) Secure Rwo-Party Protocols and Its Application", FC2020を用いたMPCの実装．それ以外の関数も今後実装予定．

# フォルダ構成
## header
通信や乱数生成に関するフォルダ．Ohata and Nuida, FC2020のBeaver Triple Extensions(BTE)などはここに実装されている．乱数の生成に関しては，128bitセキュリティのAESを用いて実装．

- CR.h

    擬似乱数生成やBTE，またその管理の記述．
    
    - rand_seed
        
        擬似乱数生成の構造体. 初期化の際に16byteの鍵を渡し，以降next_rand関数を呼ぶことで16byte(128 bit)の乱数を生成する．

    - gen_random系の関数

        擬似乱数生成の関数．rand_seedで取り出した128bitの乱数をbool, integer_tなどに変換している．引数のnumは欲しいbool / integer_tの乱数配列の長さ．

    - BT_gen系の関数

        Ohata and Nuida, FC2020のBeaver Triple Extensionの実装．fan_numはBTEのfan数．numは欲しいBTEの数．

    - ex_BT_gen系の関数

        Ohata and Nuida, FC2020のArithmetic Blindingの実装．

    - BT_Segment

        BTEを管理する構造体．fan数やarithmetic blindingかどうかを属性荷物．call関数を呼び出すと，BTEが取り出せる．保持しているBTEが枯渇した際はsupply関数が呼ばれ，BTEの補充が行われる．

    - BT_Store

        様々なタイプ(fan数やarithmetic blindingか否かなど)のBT_Segmentを一括で管理するための構造体．call関数の引数として欲しいBTEのタイプを渡すことで，対応するBTEが取り出せる．


- tcp_connection.h

    通信に関する記述. tcp[other_id].send/receiveでother_idのパーティとデータのやり取りを行う．引数はcharの配列，boolの配列，integer_t(common.hで定義．デフォルトではunsigned long longと同じ)の配列のいずれか．まれに，Z_mの元をやり取りしたい，ということがあるので，mのバイト数(num_char)を指定することで通信量を削減できるようにしている．


## src
秘密計算用のデータ型や，大小比較，等号判定といった基本的な機能を実装している．
- data_type.hpp
    
    2-out-of-2の秘密分散で保持している変数のデータ型(s_integer)や基本演算子(+, *, ==, < など)のオーバーロードを記述.

- basic_operation.hpp
    
    基本演算(乗算，等号判定，大小比較，ビット取り出し，右シフト)を記述.


## test
最低限のサンプルコード(test.cpp)を置いている．前処理，後処理とコメントアウトしてある部分ではTCP通信のための初期化処理や後処理を行っている．その間にあるコードが秘密計算で行いたい処理になっている．

# 動作環境

WSL1 (Ubuntu 20.04.1)

cmake 3.16.3

gcc 9.3.0

での動作は確認している．

# 使い方
```
mkdir build
cd build
cmake ..
make
```
を実行するとサンプルコード(test.cpp)の実行コード(test.out)がbuildフォルダ内に生成される．ターミナルを3つ立ち上げ，それぞれで`./test.out 2`, `./test.out 1`, `./test.out 0`と実行すると，プログラムが実行される．（./test.outに渡している引数はparty_id. Party 1,2が実質の秘密計算を行い，Party 0はBTEの生成，分配を行っている．）

# 謝辞
本開発は総務省SCOPE(受付番号 182103105)の援助を受けたものである．
