struct drone{
    int x,y,z;        //ドローンの現在地の座標
    int flag=0;       //ドローンが障害物回避したら1になる。関数dronego内で使用
    bool fil[5][5];      //レーザーレンジファインダーより入手した情報を格納 
    void avoidance();     //緊急回避用プログラム
    void extract();       //地図から端点を抽出する関数
    void Dijkstra();      //Dijkstraによる経路計算をする関数
    void get_map();     //マップ情報の取得
    void update_fil();
    Node node[];    //ノード
    private:
        int map[][][];        //マップ情報を格納

}
