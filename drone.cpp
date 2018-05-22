struct drone{
    int x,y;        //ドローンの現在地の座標
    bool fil[5][5];      //レーザーレンジファインダーより入手した情報を格納 
    void avoidance();     //緊急回避用プログラム
    void extract();       //地図から端点を抽出する関数
    void Dijkstra();      //Dijkstraによる経路計算をする関数
    void get_map();     //マップ情報の取得
    void update_fil();
    
    private:
        Node node[];           //ノード
        int map[][][];        //マップ情報を格納

}