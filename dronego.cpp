struct drone{
    int x,y,z;        //ドローンの現在地の座標
	int flag=0;
    bool fil[5][5];      //レーザーレンジファインダーより入手した情報を格納 
    void avoidance();     //緊急回避用プログラム
    void extract();       //地図から端点を抽出する関数
    void Dijkstra();      //Dijkstraによる経路計算をする関数
    void get_map();     //マップ情報の取得
    void update_fil();  //filを更新する
    Node node[];   //ノード
    private:
        int map[][][];        //マップ情報を格納

};

//端点から端点までドローンの現在地を更新するごとにavoidanceを呼び出しつつ進む
void dronego(drone D,int nextnode){ // D...ドローン  nextnode...目標点となるノードの番号
	
	int movex,movey;  //現在地から端点に進むために移動しなきゃいけないx座標の数とy座標の数
	movex = D.node[nextnode].x - D.x;  // movexを算出
	movey = D.node[nextnode].y - D.y;  // moveyを算出
	
	double slope;  //今いる端点の座標から次向かう端点の座標への傾き
	slope=(double)(movey)/(double)(movex);  //slopeを算出
	
	int move[movex+movey];  //端点から端点に進むためにx座標とy座標をいくつずつ,どの順番で増減させるのかを格納.yが1増加するとき(方向で表すと前)は1,yが1減少(後ろ)が2,xが1増加(右)が3,xが1減少(左)が4として対応
	
	/* 
		moveの例:右に1,前に1づつ座標(0,0)から(5,5)まで進みたいとき
		move[0]=3,move[1]=1,move[2]=3,move[3]=1,move[4]=3,move[5]=1,move[6]=3,move[7]=1,move[8]=3,move[9]=1;
	*/
	
	// ↓moveの格納方法案↓
	int ycount=0,count=0;  //ycount...yが増減した回数をカウント。  count...xまたはyが増減した回数。最終的にmovex+moveyになるはず
	for(int x=1;x<=fabs(movex);x++){  //x座標を一回づつ増減、その度にy座標も増減させるか判定
		//x座標を増減
		if(movex>0){
			move[count]=3;
			count++;
		}else if(movex<0){
			move[count]=4;
			count++;
		}
		//y座標を増減
		while(slope*x>=ycount){  //各xに対してslope*xが1を超えたタイミングでyを1回増減、2を超えたタイミングでyをもう1回増減させたい。3を超えたらさらにもう一回yを...(略)
			if(movey>0){
				move[count]=1;
				count++;
			}else if(movey<0){
				move[count]=2;
				count++;
			}
			ycount++;  //yが増減した回数を増やす
		}
	}
	
	//ドローンの位置を更新していく
	for(int i=0;i<movex+movey;i++){
		
		avoidance(D);  //進もうとしてる座標が障害物でふさがってたら障害物回避。障害物回避が起こった場合D.flag==1になってる
		
		if(D.flag==0){  //障害物回避が起こらなかった場合
			if(move[i]==1){
				D.y+=1;  //前に1進む
			}else if(move[i]==2){
				D.y+=-1;  //後ろに1進む
			}else if(move[i]==3){
				D.x+=1;  //右に1進む
			}else if(move[i]==4){
				D.x+=-1;  //左に1進む
			}
		}else if(D.flag==1){  //障害物回避が起こった場合
			break;
		}
		
	}
	
	if(D.flag==1){  //障害物回避が起こった場合
		D.flag=0;  //再帰内では障害物回避は起きてないから0に戻す
		dronego(D,nextnode);  //再帰を使って回避後の地点から目的地の端点に進む
	}
	
}
