/*
[変更点]
・関数Node_in()は不使用
・Node node[6]の中身はソースコード内でベタ打ちした
・ベタ打ちによる都合上, node[i].edges_cost[j]はすべて0で初期化した
・struct Node{}内にx,y座標のデータメンバを追加した
・x,y座標間のユークリッド距離を計算する関数calc_distance()を
　node[6]のedges_costをinputする関数input_edges_cost()内で使用している
・関数print_array()を少し変えて、各node間の距離を表示するようにした
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <sstream>

struct Node{                       //このノードから伸びるエッジの情報
  std::vector<int> edges_to;       //各エッジの接続先のノード番号
  std::vector<double> edges_cost;  //各エッジのコスト
  double x;                        //x座標
  double y;                        //y座標
  double cost;                        //このノードへの現時点で判明している最小コスト                
};  

double calc_distance(double x, double y){
  return std::sqrt(x*x + y*y);
}

void input_edges_cost(Node node[], int s){
  for(int i=0; i<s; i++){
    int n = node[i].edges_to.size();
    for(int j=0; j<n; j++){
      node[i].edges_cost[j] = calc_distance((node[i].x - node[node[i].edges_to[j]].x), (node[i].y - node[node[i].edges_to[j]].y));   
    }
  }
}

void Node_in(struct Node *node){//ノードに値を入力する関数

    int node_num, edges_to, edges_cost;
    std::string name;
    std::cout<<"FILENAME:";
    std::cin>>name;//ノードに関するデータを規定するdatを指定する
    std::ifstream file(name.c_str());//指定したdatファイルを開く
    std::string str;

    while(std::getline(file,str)){
      std::istringstream iss(str);
      iss >> node_num >> edges_to >> edges_cost;
      std::cout << node_num << "," << edges_to << "," << edges_cost << "\n";
      node[node_num].edges_to.push_back(edges_to);
      node[node_num].edges_cost.push_back(edges_cost);
    } 

    file.close();
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
      int s = d_2.size();
      for(int k=0; k<s; k++){
		if(node[i].edges_to[j] == d_2[k]){
	  		j += 1;
		}
      }
    }
    flag = 1;
    int n = node[i].edges_to.size();
    while(j<n){
      if(node[i].cost < node[node[i].edges_to[j]].cost && (node[i].cost + node[i].edges_cost[j]) == node[node[i].edges_to[j]].cost){
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
  std::cout << "[cost of nodes]\n";
  for(int i=0; i<s; i++){
    std::cout << "node:" << i << " cost:" << node[i].cost << '\n';
  }
  std::cout << '\n';

  //各ノード間の距離を表示
  int label[s];
  for(int i=0; i<s; i++){
      label[i] = 0;
  }
  std::cout << "[distance between nodes]\n";
  for(int i=0; i<s; i++){
    int n = node[i].edges_to.size();
    for(int j=0; j<n; j++){
      if(label[node[i].edges_to[j]] == 0){
        std::cout << i << "-" << node[i].edges_to[j] << " : " << node[i].edges_cost[j] << '\n';
      }
    }
    label[i] = 1;
  }
  std::cout << '\n';

  //最短経路を表示
  std::cout << "[Dijkstra route]\n";
  int s_d = d.size();
  for(int i=0; i<s_d; i++){
    std::cout << d[i] << " ";
  }
  std::cout << '\n';
}

int main(){
  int n_num;
  std::cin >> n_num;
  Node node[n_num];
  /*Node node[6] = 
    {
      {{1,2,3},   {0,0,0},   0.0, 0.0, -1},
      {{0,2,5},   {0,0,0},   4.0, 9.0, -1},
      {{0,1,3,4}, {0,0,0,0}, 3.0, 1.0, -1},
      {{0,2,4},   {0,0,0},   1.0,-3.0, -1},
      {{2,3,5},   {0,0,0},   8.0,-2.0, -1},
      {{1,4},     {0,0},     6.0, 2.0, -1}
    };
    */
  Node_in(node);

  input_edges_cost(node, 6);  //edges_costを計算

  node[0].cost = 0;    	//初期化
  search_node(node, 6);

  std::vector<int> d;		//最短経路を保存していく
  d.push_back(0);	     	//スタートノードをpush_back ここからゴールまでの最短経路をpush_backしていく
  std::vector<int> d_2;

  shortest_path(d, d_2, node);
  print_array(node, 6, d);

  return 0;
}

/*
[cost of nodes]
node:0 cost:0
node:1 cost:9.84886
node:2 cost:3.16228
node:3 cost:3.16228
node:4 cost:8.99323
node:5 cost:13.4654

[distance between nodes]
0-1 : 9.84886
0-2 : 3.16228
0-3 : 3.16228
1-2 : 8.06226
1-5 : 7.28011
2-3 : 4.47214
2-4 : 5.83095
3-4 : 7.07107
4-5 : 4.47214

[Dijkstra route]
0 2 4 5
*/