#include <iostream>
#include <vector>
#include <string>
#include <fstream>

struct Node{                       //このノードから伸びるエッジの情報
  std::vector<int> edges_to;       //各エッジの接続先のノード番号
  std::vector<int> edges_cost;     //各エッジのコスト
  //Dijkstra法のためのデータ
  int cost = -1;                       //このノードへの現時点で判明している最小コスト                
};  

void Node_in(struct Node *node){//ノードに値を入力する関数

    int node_num, edges_to, edges_cost;
    std::string name;
    std::cout<<"FILENAME of dat:";
    std::cin>>name;//ノードに関するデータを規定するdatを指定する
    std::ifstream file(name);//指定したdatファイルを開く
    std::string str;

    while(std::getline(file,str)){
        node_num=std::stoi(&str[0]);
        edges_to=std::stoi(&str[2]);
        edges_cost=std::stoi(&str[4]);
        node[node_num].edges_to.push_back(edges_to);
        node[node_num].edges_cost.push_back(edges_cost);
        } 
        
        std::cout << "a\n";

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
  for(int i=0; i<s; i++){
    std::cout << "node:" << i << " cost:" << node[i].cost << '\n';
  }

  //最短経路を表示
  std::cout << "Dijkstra_route\n";
  int s_d = d.size();
  for(int i=0; i<s_d; i++){
    std::cout << d[i] << " ";
  }
  std::cout << '\n';
}

int main(){

  Node node[6];
  Node_in(node);

  node[0].cost = 0;    	//初期化

  search_node(node, 6);

  std::vector<int> d;		//最短経路を保存していく
  d.push_back(0);	     	//スタートノードをpush_back ここからゴールまでの最短経路をpush_backしていく
  std::vector<int> d_2;

  shortest_path(d, d_2, node);
  print_array(node, 6, d);

  return 0;
}