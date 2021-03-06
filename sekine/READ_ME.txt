
***
目的
***
三次元空間上をダイクストラ法を用いて経路設計をし, 障害物を回避しながら目的地まで自動航行させる.


***
プログラムの中身
***
[drone3D.zip]
  [Release]
    drone3D.exe
    drone3D.exp
    drone3D.iobj
    drone3D.ipdb
    drone3D.lib
    drone3D.pdb
    drone3D_log.html
    [picture]
      floor.jpg
      icon.png
      wall1.png
      wood1.png
      wood2.png
    [map]
    [output]
  [MapExample]
    [map1]
      map00.dat ~ map09.dat
      nmap00.dat ~ nmap09.dat
      input_sg.dat
    [map2]
      map00.dat ~ map09.dat
      nmap00.dat ~ nmap09.dat
      input_sg.dat
  [MapMaker.zip]
    [MapMaker]
      MapMaker.exe
      MapMaker.exp
      MapMaker.iobj
      MapMaker.ipdb
      MapMaker.lib
      MapMaker.pdb
      MapMaker_log.html
  READ_ME.txt
  Algorithm.txt
      

***
使い方
***
1.  まず, MapMaker.zipを展開し, [MapMaker]フォルダができることを確認する.
    0を自遊空間, 1を壁としたマップをMapMaker.exeを実行してプログラムの指示にしたがって作成する.
    地図データを想定したマップのファイル名はmap00~map09で作成し, 障害物を想定したマップはファイル名を
    nmap00~nmap09の間で作成する. 数字の下一桁はそのマップの階層を表す. nmapは, mapと同様に0, 1で構成される.
    nmap00~nmap09は, map00~map09に対応しており, mapには映らない不意の障害物を想定している.
    MapMaker.exeでmap0x.datファイルを作成するとnmap0x.datが自動生成されるので, nmap0x.datを編集する際はそれをりようする.
    nmapを作る際にはmap上の経路を完全にふさいで目的地に絶対にたどり着けないように障害物をおかないように気を付ける.
    また, mapを作るときはmap00から順に作成すること.
    [MapMaker.exeの特殊操作]
      W + 左クリック:クリックした場所を始点として上方向に10マスを塗りつぶす.
      A + 左クリック:クリックした場所を始点として左方向に10マスを塗りつぶす.
      S + 左クリック:クリックした場所を始点として下方向に10マスを塗りつぶす.
      D + 左クリック:クリックした場所を始点として右方向に10マスを塗りつぶす.
      B + 左クリック:クリックした場所を中心として10*10マスの正方形を塗りつぶす.

2. [MapMaker]内に作成したマップ(map0x.dat, nmap0x.datがそれぞれあることを確認)が保存されているので
    それらをすべて[Release]フォルダ内の[map]フォルダ内に移動する.

3. input_sg.datというファイルを作成し, 任意の開始地点と目的地を以下のように入力する.
　　 [開始地点のx座標] [開始地点のy座標] [開始地点のz座標]
　　 [目的地のx座標] [目的地のy座標] [目的地のz座標]
　　 入力例 :
　　 5 5 0
　　 89 89 4
  その後, input_sg.datをmap0x.dat, nmap0x.datと同じように[Release]フォルダ内の[map]フォルダ内に移動する.
  [注意]
    開始地点, 目的地ともに座標はマップの自遊空間上に設定すること.

4. drone3D.exeを実行すると仮想3次元空間上でのドローンの航行が描画される.
   もしうまく描画されない場合は, 各ファイルが[Release]ファイル内の[map]フォルダ内に存在するか,
   また, 各ファイルの中身は正しいかを確認する.
   [備考]
      [MapExample]内には[map1], [map2]フォルダ内にそれぞれmap0x.dat, nmap0x.dat, input_sg.datの例があるので, 
      試しにすべて[Release]フォルダ内の[map]フォルダ内に移動してdrone3D.exeを実行してドローンの挙動を確認してほしい.
