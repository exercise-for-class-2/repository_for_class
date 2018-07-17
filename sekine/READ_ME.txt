***
目的
***
三次元空間上をダイクストラ法を用いて経路設計をし、障害物を回避しながら目的地まで自動航行させる。

[プログラムの中身]
[drone3D.zip]
   [Release]
     drone3D.exe
     drone3D.exp
     drone3D.iobj
     drone3D.ipdb
     drone3D.lib
     drone3D.pdb
     drone3D_log.html
     input_start_goal.dat
   [MapExample]
     map01.dat ~ map09.dat
     nmap01.dat ~ nmap09.dat
   [map_maker.zip]
     MapMaker.exe
     MapMaker.exp
     MapMaker.iobj
     MapMaker.ipdb
     MapMaker.lib
     MapMaker.pdb
     MapMaker_log.html

***
使い方
***
1. 0を自由空間、1を壁としたマップをMapMaker.exeを実行してプログラムの指示にしたがって作成する。
   地図データを想定したマップのファイル名はmap00~map09で作成し、障害物を想定したマップはファイル名を
   nmap00~nmap09の間で作成する。数字の下一桁はそのマップの階層を表す。
   [MapMaker.exeの特殊操作]
     W + 左クリック:クリックした場所を始点として上方向に10マスを塗りつぶす。
     A + 左クリック:クリックした場所を始点として左方向に10マスを塗りつぶす。
     S + 左クリック:クリックした場所を始点として下方向に10マスを塗りつぶす。
     D + 左クリック:クリックした場所を始点として右方向に10マスを塗りつぶす。
     B + 左クリック:クリックした場所を中心として10*10マスの正方形を塗りつぶす。
2. map_maker.zip内に作成したマップが保存されているのでそれをReleaseフォルダ内に移動する。

3. input_start_goal.datに任意の開始地点と目的地を以下のように入力する。
　　[開始地点のx座標] [開始地点のy座標] [開始地点のz座標]
　　[目的地のx座標] [目的地のy座標] [目的地のz座標]
　　入力例:
　　5 5 0
　　89 89 4

4. drone3d.exeを実行すると仮想3次元空間上でのドローンの航行が描画される。
