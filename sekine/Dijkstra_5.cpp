#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <stdio.h>
#include <time.h>

#define GNPLT "C:/PROGRA~1/gnuplot/bin/gnuplot.exe"  // gnuplot.exe のある場所. PROGRA~1 は Program Files を意味する.
#define STRLN 100                                    // 文字列長の上限.
#define SIZE  100                                    // 2次元自由空間の規模.
#define X 101
#define Y 101

struct Node{                       //このノードから伸びるエッジの情報
  std::vector<int> edges_to;       //各エッジの接続先のノード番号
  std::vector<double> edges_cost;  //各エッジのコスト
  double x;                        //x座標
  double y;                        //y座標
  double cost = -1;                //このノードへの現時点で判明している最小コスト                
};

//map[X][Y]に地図を保存する
void input_map(std::string file, int map[][Y], int x, int y)
//x*x + y*yを返す
double calc_distance(double x, double y);
//node間の距離をnode[]に代入
void input_edges_cost(Node node[], int s);
//各ノードの座標を代入 file = xy_edges.dat
void Node_in(struct Node node[], std::string file);
//各ノードまでのコストを計算
void search_node(Node node[], int s);
//最短経路の計算
void shortest_path(std::vector<int>& d, std::vector<int>& d_2, Node node[]);
//色々std::coutしてくれます
void print_array(Node node[], int s, std::vector<int>& d);
//node.datの各ノードとその向こう先を書き込む関数 file = node2.dat
void make_node(std::string file, Node node[], int n);
//file = "xy_edges.dat"から, ノードの個数を返す
int number_of_node(std::string file);
//gnuplotで描画し画像をpngファイルに保存
int gnuplot_spc(std::string ofile);
//ノード間の壁の判定
bool check_wall(Node node, int map[][Y]);


int main(){

  clock_t start = clock();

  std::string file1 = "map2.dat";        //記入済み
  std::string file2 = "node2.dat";       //未記入
  std::string file3 = "xy_all2.dat";     //記入済み
  std::string file4 = "xy_edges2.dat";   //記入済み

  int n = number_of_node(file4);
  Node node[n];

  int x=0, y=0;
  int map[X][Y];
  input_map(file1, map, x, y);  //map[x][y]にfile1に保存された0,1の地図を代入していく

  Node_in(node, file4);         //nodeの(x, y)を代入
  make_node(file2, node, n);    //node.datに記入, node[].edges_toに代入

//壁の判定が未完成のためこの部分は今のところ無視
/*
  input_edges_cost(node, n);    //edges_costを計算

  node[0].cost = 0;    	        //初期化
  search_node(node, n);         //各ノードまでのコストを代入

  std::vector<int> d;          //最短経路を保存していく
  d.push_back(0);              //スタートノードをpush_back ここからゴールまでの最短経路をpush_backしていく
  std::vector<int> d_2;        //最短経路以外のたどったノードを保存していく

  shortest_path(d, d_2, node); //最短経路をdに保存
  print_array(node, 6, d);     //いろいろ出力
*/

  gnuplot_spc(file3);          //地図を保存
  gnuplot_spc(file4);          //端点を保存

  clock_t end = clock();
  //演算時間の表示
  //std::cout << "Time of calculation:" << (double)(end-start)/CLOCKS_PER_SEC << ".sec\n";
  std::cout << "Please check 'xy_all2.png' & 'xy_edges2.png'\n";
    
  return 0;
}

void input_map(std::string file, int map[][Y], int x, int y){
    std::string str;
    int tmp;
    std::ifstream ifile(file.c_str());
    for(int i=0;i<y;i++){
        std::getline(ifile,str);
        std::istringstream iss(str);
        int j=0;
        while(iss >> tmp){
            map[j][i] = tmp;
            j++;
        }
    }
}

double calc_distance(double x, double y){
  return std::sqrt(x*x + y*y);
}

void input_edges_cost(Node node[], int s){
  for(int i=0; i<s; i++){
    int n = node[i].edges_to.size();
    for(int j=0; j<n; j++){
      node[i].edges_cost.push_back(calc_distance((node[i].x - node[node[i].edges_to[j]].x), (node[i].y - node[node[i].edges_to[j]].y)));   
    }
  }
}

void Node_in(struct Node node[], std::string file){
    //std::string file = "xy_edges.dat"
    double x, y;
    std::ifstream ifile(file.c_str());
    std::string str;
    int i = 0;
    while(std::getline(ifile,str)){
      std::istringstream iss(str);
      iss >> x >> y;
      node[i].x = x;
      node[i].y = y;
      i += 1;
    } 
    ifile.close();
}

void search_node(Node node[], int s){
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

void shortest_path(std::vector<int>& d, std::vector<int>& d_2, Node node[]){
  int i = 0;
  int j = 0;
  int flag = 1;
  while(1){
    if(flag == 2){
      int s = d_2.size();
      for(int k=0; k<s; k++){
		if(node[i].edges_to[j] == d_2[k]){  //無限ループを回避するためのif文
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
      i = d[d.size()-2];    //ノードを一つ前に戻す		
      d.pop_back();	        //最後のノードを削除
      flag = 2;
    }
    if(i==5){		        //iがゴールに到着したら
      break;		        //終了
    } 
    j = 0;
  }
}

void print_array(Node node[], int s, std::vector<int>& d){
  //各ノードの座標を表示
  std::cout << "[coodinate of nodes]\n";
  for(int i=0; i<s; i++){
    std::cout << i << "(" << node[i].x << "," << node[i].y << ")\n";
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

  //各ノードまでのコストを表示
  std::cout << "[cost of nodes]\n";
  for(int i=0; i<s; i++){
    std::cout << "node:" << i << " cost:" << node[i].cost << '\n';
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

//node.datに各ノードとその向かう先を書き込む関数
//file = "node2.dat";
void make_node(std::string file, Node node[], int n){
    //node.datにノードを書き込む
    std::ofstream ofp(file.c_str());
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(i != j){
                ofp << i << " " << j << '\n';
            }
        }
    }
    ofp.close();

    //node.datを読み込んでnode[].edges_to[]に代入していく
    int node_num, edges_to, edges_cost;
    std::ifstream ifp(file.c_str());
    std::string str;
    while(std::getline(ifp,str)){
      std::istringstream iss(str);
      iss >> node_num >> edges_to;
      node[node_num].edges_to.push_back(edges_to);
    } 
    ifp.close();
}

int number_of_node(std::string file){
    int cnt = 0;
    int tmp;
    std::string str;
    std::ifstream ifile(file.c_str());
    while(std::getline(ifile, str)){
        std::istringstream iss(str);
        while(iss >> tmp){
            cnt += 1;
        }
    }
    return (cnt/2);
}

int gnuplot_spc(std::string ofile){
    char t[STRLN];
    FILE *gp; if((gp = _popen(GNPLT, "w")) == NULL) { printf("ERR\n"); exit(1); }
    fprintf(gp, "set size square\nset colorsequence classic\n");
    fprintf(gp, "set style l 1 lt 1 lc 1 lw 1 pt 5 ps 1\n");
    fprintf(gp, "set ticscale 0\nset xtics 10\nset ytics 10\n");
    fprintf(gp, "set xrange[0:100]\nset yrange[0:100]\n");
    if(ofile == "xy_all2.dat"){
        fprintf(gp, "set terminal png\n");
        fprintf(gp, "set output 'xy_all2.png'\n");
        fprintf(gp, "plot 'xy_all2.dat' linestyle 1\n");
    }
    else if(ofile == "xy_edges2.dat"){
        fprintf(gp, "set terminal png\n");
        fprintf(gp, "set output 'xy_edges2.png'\n");
        fprintf(gp, "plot 'xy_edges2.dat' linestyle 1\n");
    }
    system("pause"); fprintf(gp, "exit\n");
    return _pclose(gp);
}

//編集中
/*
bool check_wall(Node node, int map[][Y]){
  int s_x = node[0].x;
  int s_y = node[0].y;
  int g_x = node[1].x;
  int g_y = node[1].y;

  int d_x = g_x - s_x;    //端点同士のx座標の差difference_x
  int d_y = g_y - s_y;    //端点同士のy座標の差difference_y
  int slope1 = d_y / d_x;  //2点間の傾きd_y / d_x
  int slope2 = d_x / d_y;  //2点間の傾きd_x / d_y

  int k;
  if((d_x>=0) && (d_y>=0))  k = 0;
  if((d_x<=0) && (d_y>=0))  k = 1;
  if((d_x<=0) && (d_y<=0))  k = 2;
  if((d_x>=0) && (d_y<=0))  k = 3;

  int flag = 0;

  if(k==0){//s(x,y)の右上にg(x,y)がある
    if(std::abs(slope2) <= std::abs(slope1)){
      int i, j = 0;
      for(i=1; i<=std::abs(d_x); i++){
        int cnt = 0;
        while(cnt != std::abs(sleep1)){
          if(flag == 0){
            if(map[s_x+i][s_y  ] == 0)  flag = 1;
            if(map[s_x+i][s_y+j] == 0)  flag = 1;
          }
          if(flag == 1){
            if(map[s_x+i][s_y  ] == 1)  flag = 2;
            if(map[s_x+i][s_y+j] == 1)  flag = 2;
          }
          if(flag == 2){
            if(map[s_x+i][s_y  ] == 0)  flag = 3;
            if(map[s_x+i][s_y+j] == 0)  flag = 3;
          }
          if(flag == 3){
            return false;
          }
          cnt += 1;
          j += 1;
        }
      }
      //ここまでくれば現在地のx座標は目的地のx座標と一致する(はず)
      while((s_y+j) != g_y){
        if(flag == 0){
          if(map[s_x+i][s_y+j] == 0)  flag = 1;
        }
        if(flag == 1){
          if(map[s_x+i][s_y+j] == 1)  flag = 2;
        }
        if(flag == 2){
          if(map[s_x+i][s_y+j] == 0)  flag = 3;
        }
        if(flag == 3){
          return false;
        }
        j += 1;
      }
      return true;
    }
    else{
      int i = 0, j;
      for(j=1; j<=std::abs(d_y); j++){
        int cnt = 0;
        while(cnt != std::abs(sleep2)){
          if(flag == 0){
            if(map[s_x+i][s_y  ] == 0)  flag = 1;
            if(map[s_x+i][s_y+j] == 0)  flag = 1;
          }
          if(flag == 1){
            if(map[s_x+i][s_y  ] == 1)  flag = 2;
            if(map[s_x+i][s_y+j] == 1)  flag = 2;
          }
          if(flag == 2){
            if(map[s_x+i][s_y  ] == 0)  flag = 3;
            if(map[s_x+i][s_y+j] == 0)  flag = 3;
          }
          if(flag == 3){
            return false;
          }
          i += 1;
        }
      }
      //ここまでくれば現在地のx座標は目的地のy座標と一致する(はず)
      while((s_x+i) != g_x){
        if(flag == 0){
          if(map[s_x+i][s_y+j] == 0)  flag = 1;
        }
        if(flag == 1){
          if(map[s_x+i][s_y+j] == 1)  flag = 2;
        }
        if(flag == 2){
          if(map[s_x+i][s_y+j] == 0)  flag = 3;
        }
        if(flag == 3){
          return false;
        }
        i += 1;
      }
      return true;
    }
  }
  if(k==1){//s(x,y)の左上にg(x,y)がある

  }
  if(k==2){//s(x,y)の左下にg(x,y)がある

  }
  if(k==3){//s(x,y)の右下にg(x,y)がある

  }
}
*/

/*
[coodinate of nodes]
0(10,10)
1(20,50)
2(40,50)
3(60,50)
4(80,50)
5(90,90)

[distance between nodes]
0-1 : 41.2311
0-2 : 50
0-3 : 64.0312
0-4 : 80.6226
0-5 : 113.137
1-2 : 20
1-3 : 40
1-4 : 60
1-5 : 80.6226
2-3 : 20
2-4 : 40
2-5 : 64.0312
3-4 : 20
3-5 : 50
4-5 : 41.2311

[cost of nodes]
node:0 cost:0
node:1 cost:41.2311
node:2 cost:50
node:3 cost:64.0312
node:4 cost:80.6226
node:5 cost:113.137

[Dijkstra route]
0 5
続行するには何かキーを押してください . . .
続行するには何かキーを押してください . . .
*/