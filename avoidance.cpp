#include <iostream>
struct drone{
    int x,y;        //ドローンの現在地の座標
    bool fil[5][5];      //レーザーレンジファインダーより入手した情報を格納 
    void avoidance();     //緊急回避用プログラム
    void extract();       //地図から端点を抽出する関数
    void Dijkstra();      //Dijkstraによる経路計算をする関数
    void get_map();     //マップ情報の取得
    void update_fil();  //filを更新する
    
    private:
        Node node[];           //ノード
        int map[][][];        //マップ情報を格納

}
void avoidance(drone D){
    int nextx,nexty;
    // bool fil[5][5] = {false};
    // int rcnt,lcnt;
    // for(int i=0;i<6;i++){
    //     for(int j=0;j<6;j++){
    //         if(fil[i][j]){
    //             fil[i][j] = true;
    //         }
    //     }
    // }

    if(fil[2][0] && fil[0][0] && fil[4][0]){    //前方三つのセンサーが反応
        if(fil[4][2]){
            while(true){
                if(!fil[0][2]){
                    D.x -= 1;
                    update_fil();
                }else{
                    break;
                }
            }
        }else if(fil[0][2]){
            while(true){
                if(!fil[4][2]){
                    D.x += 1;
                    update_fil();
                }else{
                    break;
                }
            }
        }
    }else if(fil[2][0]){
        if(!fil[0][0]){
            while(true){
                if(!fil[4][0]){
                    D.x += 1;
                    D.y += 1;
                    update_fil();
                }else{
                    break;
                }
            }
        }else if(!fil[4][0]){
            while(true){
                if(!fil[0][0]){
                    D.x -= 1;
                    D.y += 1;
                    update_fil();
                }else{
                    break;
                }
            }
        }
    }
}