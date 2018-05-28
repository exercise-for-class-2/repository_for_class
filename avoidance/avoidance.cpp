#include <iostream>
struct drone{
    int x,y,z;        //ドローンの現在地の座標
    bool fil[5][5];      //レーザーレンジファインダーより入手した情報を格納 
    void avoidance();     //緊急回避用プログラム
    void extract();       //地図から端点を抽出する関数
    void Dijkstra();      //Dijkstraによる経路計算をする関数
    void get_map();     //マップ情報の取得
    void update_fil();  //filを更新し、ドローン周辺の障害物情報を格納
    Node node[];           //ノード
    
    private:
        int map[][][];        //マップ情報を格納

}

bool chk_wall(Drone d,i,j){
	if(map[i][j] == 0){
		return false;
	}else{
		return true;
	}
}


void avoidance(drone D,bool left, bool right, bool front, bool back){
    int nextx,nexty;
    if(fil[2][0] && fil[0][0] && fil[4][0]){        //前方三つのセンサーが反応
        if(fil[4][2]){                              //右に壁判定且つ右に壁はマップに存在しないとき
            if(chk_wall(D,D.x+1,D.y)){              //前方の障害物がなくなるまで左に進む
                D.x -= 1;
                update_fil();
            }else{
                right = true;
            }
        }else if(fil[0][2]){                        //左に壁判定且つ左の壁がマップに存在しないとき
            if(chk_wall(D,D.x-1,D.y)){              //前方の障害物がなくなるまで右に進む
                D.x += 1;
                update_fil();
            }else{
                left = true;
            }
        }
    }else if(fil[2][0]){                        //前方に障害物あり
        if(!fil[0][0] && fil[4][0]){            //左斜め前に障害物なしかつ右斜め前に障害物あり
            if(chk_wall(D, D.x+1, D.y+1)){      //右斜め前に障害物がなくなるまで左斜め前に進む
                D.x += 1;
                D.y += 1;
                update_fil();
            }
        }else if(!fil[4][0] && fil[0][0]){       //右斜め前に障害物なしかつ左斜め前に障害物あり
            if(chk_wall(D, D.x-1, D.y+1)){       //左前方に障害物がなくなるまで
                D.x -= 1;
                D.y += 1;
                update_fil();
            }
        }
    }else if(fil[2][0] && fil[0][0] && front){
        if(chk_wall(D, D.x-1,D.y)){
            D.y -= 1;
            update_fil();
        }
    }//角にぶつかった場合、後ろに下がるべきか階層変更を検討するべきか？
    if(!fil[2][0]){
        return;
    }else{
        avoidance(D,left,right);
    }
}