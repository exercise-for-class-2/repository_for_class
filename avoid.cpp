#include<iostream>

struct drone{
    int x,y;        //ドローンの現在地の座標
    map[][];        //マップ情報を格納
    fil[5][5];      //レーザーレンジファインダーより入手した情報を格納 
    int nextx,nexty    //次に進む予定の座標(現在地からみていくつ進むか。とりあえずnextxとnextyのどちらかに1,-1が入ると想定)
    private:
        Node[]          //通るノードを格納 
        extract()       //地図から端点を抽出する関数
        Dijkstra()      //Dijkstraによる経路研鑽をする関数
        avoidance()  //緊急回避用プログラム
}

void filin(drone drone){//filに情報を格納。0は自由空間、1は障害物
	
	fil[2][2]=0;//現在地のため自由空間。
	
}

//経路上に障害物があれば仮の経路作成する関数。
void avoidance(std::vector<int> d,drone drone){ //d...地図上での最短経路  drone...drone構造の「ドローン」
	
	int flag=0;//障害物回避で逸れたあとまた経路上に戻ってきたら1になる
	while(flag==0);{
		if(drone.nextx!=0){
			drone.nextx=0;
			drone.nexty=1;
		}else if(drone.nexty!=0){
			drone.nextx=1;
			drone.nexty=0;
		}
		
		drone.x = drone.nextx;
		drone.y = drone.nexty;
		
		//if(drone.x == 経路上 && drone.y == 経路上){ //この判定法どうしよう
			flag=1;
		}
	}
	
}
//現在の座標周辺を探り経路上に障害物があればavoidanceを呼び出す。droneが一座標進むごとにこれを使う想定
void dronego(std::vector<int> d,drone drone){ //d...地図上での最短経路  drone...drone構造の「ドローン」
	
	filin(drone); //周りの状況を取得
	
	if(drone.fil[2+drone.nextx][2+drone.nexty]==1){//進もうとしてる座標が障害物でふさがってたら
		
		avoidance(d,drone);//障害物回避
		
	}else{
		drone.x = drone.nextx;
		drone.y = drone.nexty;
		//drone.nextx = ??  nextxとnextyをどうやって決めるのかわからない
		//drone.nexty = ??
	}

}