struct drone{
    int x,y;        //ドローンの現在地の座標
    map[][];        //マップ情報を格納
    fil[5][5];      //レーザーレンジファインダーより入手した情報を格納 
    private:
        Node[]          //通るノードを格納 
        extract()       //地図から端点を抽出する関数
        Dijkstra()      //Dijkstraによる経路研鑽をする関数
        avoidance()     //緊急回避用プログラム
}