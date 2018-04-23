
//関数使用

#include <iostream>
#include <vector>

struct Node{                       //このノードから伸びるエッジの情報
  std::vector<int> edges_to;       //各エッジの接続先のノード番号
  std::vector<int> edges_cost;     //各エッジのコスト
  //Dijkstra法のためのデータ
  bool done;                      //確定ノードか否か
  int cost;                       //このノードへの現時点で判明している最小コスト                
};  

void search_node(Node node[], int s){
  //各ノードまでのコストを計算
  int i = 0;
  while(i<s){
    int j = 0;
    int n = node[i].edges_to.size();	//node[i].edges_toの要素数
    if(i==0){
      while(j<n){
	if(node[node[i].edges_to[j]].cost == -1){
	  node[node[i].edges_to[j]].cost = node[i].edges_cost[j];
	  j += 1;
	}
      }
      j = 0;
    }
    else{
      while(j<n){
	if(node[node[i].edges_to[j]].cost == -1){
	  node[node[i].edges_to[j]].cost = node[i].cost + node[i].edges_cost[j];
	}
	else if(node[i].cost + node[i].edges_cost[j] < node[node[i].edges_to[j]].cost){
	  node[node[i].edges_to[j]].cost = node[i].cost + node[i].edges_cost[j];
	}
	j += 1;
      }
      j = 0;
    }
    i += 1;
  }
}
//各ノードまでのコストが確定

void shortest_path(std::vector<int>& d, std::vector<int>& d_2, Node node[]){
  //最短経路の計算
  int i = 0;
  int j = 0;
  int flag = 1;
  while(1){
    if(flag == 2){
      for(int k=0; k<d_2.size(); k++){
	if(node[i].edges_to[j] == d_2[k]){
	  j += 1;
	}
      }
    }
    flag = 1;
    int n = node[i].edges_to.size();
    while(j<n){
      if(node[i].cost < node[node[i].edges_to[j]].cost && (node[i].cost + node[i].edges_cost[j]) == node[node[i].edges_to[j]].cost){
	node[node[i].edges_to[j]].done = true;
	d.push_back(node[i].edges_to[j]);
	i = node[i].edges_to[j];     //iを次のノードにする
	flag = 0;
      }
      if(flag==0){
	break;
      }
      j += 1;
    }
    if(flag==1){	      	//現在ノードから次へ進めないとき
      d_2.push_back(i);		//ノードiをkに保存
      i = d[d.size()-2];       	//ノードを一つ前に戻す		
      d.pop_back();	       	//最後のノードを削除
      flag = 2;
    }
    if(i==5){		       	//iがゴールに到着したら
      break;		       	//終了
    }
    j = 0;
  }
}
//最短経路が確定

void print_array(Node node[], int s, std::vector<int>& d){
  //各ノードまでのコストを表示
  for(int i=0; i<s; i++){
    std::cout << "node:" << i << " cost:" << node[i].cost << '\n';
  }

  //最短経路を表示
  std::cout << "Dijkstra_route\n";
  for(int i=0; i<d.size(); i++){
    std::cout << d[i] << " ";
  }
  std::cout << '\n';
}

int main(){

  Node node[6] = 
    {
      {{1,2,3},   {5,4,2},   false, -1},
      {{0,2,5},   {5,2,6},   false, -1},
      {{0,1,3,4}, {4,2,3,2}, false, -1},
      {{0,2,4},   {2,3,6},   false, -1},
      {{2,3,5},   {2,6,4},   false, -1},
      {{1,4},     {6,4},     false, -1}
    };	

  node[0].cost = 0;    	//初期化
  node[0].done = true;	//初期化

  search_node(node, 6);

  std::vector<int> d;		//最短経路を保存していく
  d.push_back(0);	     	//スタートノードをpush_back ここからゴールまでの最短経路をpush_backしていく
  std::vector<int> d_2;

  shortest_path(d, d_2, node);
  print_array(node, 6, d);

  return 0;
}

/*
[備考]
構造体Nodeのメンバ変数の bool done;
の使い道がなかった。
→方法が間違ってる？
/*

node:0 cost:0
node:1 cost:5
node:2 cost:4
node:3 cost:2
node:4 cost:6
node:5 cost:10
Dijkstra_route
0 2 4 5 
*/
