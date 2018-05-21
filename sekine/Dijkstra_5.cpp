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
#define S_X 5   //startのx座標
#define S_Y 5   //startのy座標
#define G_X 95  //goalのx座標
#define G_Y 95  //goalのy座標
#define MAX 100

struct Node{                       //このノードから伸びるエッジの情報
  std::vector<int> edges_to;       //各エッジの接続先のノード番号
  std::vector<double> edges_cost;  //各エッジのコスト
  double x;                        //x座標
  double y;                        //y座標
  double cost = -1;                //このノードへの現時点で判明している最小コスト
  int flag = 0;                    //未探索のノードは0, 探索済みは1               
};

struct Stack{
    double s[MAX];
    int top;

    void initialize(){
      top = 0;
    }
    bool isEmpty(){
      return (top == 0);
    }
    bool isFull(){
      return (top == MAX-1);
    }
    void push(double x){
      if(isFull()){
          std::cout << "stack overflow\n";
          exit(0);
      }
      s[top++] = x;
    }
    double pop(){
      if(isEmpty()){
          std::cout << "stack underflow\n";
          exit(0);
      }
      return s[--top];
    }
};

//map[X][Y]に地図を保存する
void input_map(std::string file, int map[][Y], int x, int y);
//x*x + y*yを返す
double calc_distance(double x, double y);
//node間の距離をnode[]に代入
void input_edges_cost(Node node[], int s);
//各ノードの座標を代入 file = xy_edges.dat
void Node_in(struct Node node[], std::string file);
//各ノードまでのコストを計算
void search_node(Node node[], int s);
//探索済みのノードのうち, 未探索のedges_toを持つノードの添え字を返す
int search_next_node(Node node[], int k, int n);
//最短経路の計算
void shortest_path(std::vector<int>& d, std::vector<int>& d_2, Node node[], int goal);
//色々std::coutしてくれます
void print_array(Node node[], int s, std::vector<int>& d);
//node.datの各ノードとその向こう先を書き込む関数 file = node2.dat
void make_node(std::string file, int map[][Y], Node node[], int n);
//file = "xy_edges.dat"から, ノードの個数を返す
int number_of_node(std::string file);
//gnuplotで描画し画像をpngファイルに保存
int gnuplot_spc(std::string ofile);
//ノード間の壁の判定
bool check_wall(Node node[], int map[][Y], int i_s, int i_g);
//check_wall()内で使う関数
bool check_wall_last(int map[][Y], int s_x, int s_y, int g_x, int g_y);


int main(){

  clock_t start = clock();

  std::string file1 = "map2.dat";        //記入済み
  std::string file2 = "node2.dat";       //未記入
  std::string file3 = "xy_all2.dat";     //記入済み
  std::string file4 = "xy_edges2.dat";   //記入済み

  int n = number_of_node(file4);
  Node node[n];

  int x=X, y=Y;
  int map[X][Y];
  input_map(file1, map, x, y);  //map[x][y]にfile1に保存された0,1の地図を代入していく

  Node_in(node, file4);              //nodeの(x, y)を代入
  make_node(file2, map, node, n);    //node.datに記入, node[].edges_toに代入
  std::cout << "OK1\n";

  input_edges_cost(node, n);    //edges_costを計算

  node[0].cost = 0;    	        //初期化
  search_node(node, n);         //各ノードまでのコストを代入

  std::vector<int> d;          //最短経路を保存していく
  d.push_back(0);              //スタートノードをpush_back ここからゴールまでの最短経路をpush_backしていく
  std::vector<int> d_2;        //最短経路以外のたどったノードを保存していく

  std::cout << "OK2\n";
  //shortest_path(d, d_2, node, n-1); //最短経路をdに保存
  print_array(node, n, d);     //いろいろ出力

  //gnuplot_spc(file3);          //地図を保存
  //gnuplot_spc(file4);          //端点を保存


  clock_t end = clock();
  //演算時間の表示
  std::cout << "Time of calculation:" << (double)(end-start)/CLOCKS_PER_SEC << ".sec\n";
  //std::cout << "Please check 'xy_all2.png' & 'xy_edges2.png'\n";
    
  return 0;
}

void input_map(std::string file, int map[][Y], int x, int y){
    std::string str;
    int tmp;
    std::ifstream ifile(file.c_str());
    for(int i=0;i<x;i++){
        std::getline(ifile,str);
        std::istringstream iss(str);
        int j=0;
        while(iss >> tmp){
            map[i][j] = tmp;
            j++;
        }
    }
    map[S_X][S_Y] = 1;
    map[G_X][G_Y] = 1;
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
  int cnt = 0;    //sになったらすべて探索済み
  int i = 0;
  int j = 0;
  int n = node[i].edges_to.size();
  node[i].cost = 0;
  node[i].flag = 1;
  cnt += 1;
  while(j<n){
	  if(node[node[i].edges_to[j]].cost == -1){
	    node[node[i].edges_to[j]].cost = node[i].edges_cost[j];
      node[node[i].edges_to[j]].flag = 1;
      cnt += 1;
	    j += 1;
	  }
  }
  i = search_next_node(node, 0, n);
  while(1){
    j = 0;
    n = node[i].edges_to.size();	//node[i].edges_toの要素数
    while(j<n){
      if(node[node[i].edges_to[j]].cost == -1){
        node[node[i].edges_to[j]].cost = node[i].cost + node[i].edges_cost[j];
        node[node[i].edges_to[j]].flag = 1;
        cnt += 1;
      }
      else if(node[i].cost + node[i].edges_cost[j] < node[node[i].edges_to[j]].cost){
        node[node[i].edges_to[j]].cost = node[i].cost + node[i].edges_cost[j];
      }
      j += 1;
    }
    if(cnt == 40){
      break;
    }
    int k = i;
    i = search_next_node(node, k, n);
    j = 0;
    while(k == i){
      Stack stack;
      if(j == n){
        j = 0;
        int m = stack.pop();
        k = node[m].edges_to[j];
        n = node[m].edges_to.size();
        i = search_next_node(node, k, n);
        j += 1;
      }
      else{
        k = node[i].edges_to[j];
        stack.push(k);
        n = node[i].edges_to.size();
        i = search_next_node(node, k, n);
        j += 1;
      }
    }
  }
}

int search_next_node(Node node[], int k, int n){
  for(int j=0; j<n; j++){
    int n_edges_to = node[node[k].edges_to[j]].edges_to.size();
    for(int l=0; l<n_edges_to; l++){
      int m = node[node[k].edges_to[j]].edges_to[l];  //start(node[k])の先の探索済みノードから, 向かうノード
      if(node[m].flag == 0){                      //それがもし未探索なら
        return node[k].edges_to[j];               //未探索ノードを持つnode[k].edges_to[j]を返す
      }
    }
  }
  return k;                                       //すべて探索済みならスタート0に戻り未探索を探す
}

void shortest_path(std::vector<int>& d, std::vector<int>& d_2, Node node[], int goal){
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
    if(flag==1){	        	//現在ノードから次へ進めないとき
      d_2.push_back(i);	  	//ノードiをkに保存
      i = d[d.size()-2];    //ノードを一つ前に戻す		
      d.pop_back();	        //最後のノードを削除
      flag = 2;
    }
    if(i==goal){		       //iがゴールに到着したら
      break;		           //終了
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
/*
  //最短経路を表示
  std::cout << "[Dijkstra route]\n";
  int s_d = d.size();
  for(int i=0; i<s_d; i++){
    std::cout << d[i] << " ";
  }
  std::cout << '\n';
*/
}

//node.datに各ノードとその向かう先を書き込む関数
//file = "node2.dat";
void make_node(std::string file, int map[][Y], Node node[], int n){
    //node.datにノードを書き込む
    std::ofstream ofp(file.c_str());
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(i != j){
              if(check_wall(node, map, i, j)){
                ofp << i << " " << j << '\n';
              }
            }
        }
    }
    ofp.close();

    //node.datを読み込んでnode[].edges_to[]に代入していく
    int node_num, edges_to;
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

bool check_wall(Node node[], int map[][Y], int i_s, int i_g){
  int s_x = node[i_s].x;
  int s_y = node[i_s].y;
  int g_x = node[i_g].x;
  int g_y = node[i_g].y;

  std::cout << s_x << " "<< s_y << " "<< g_x << " "<< g_y << '\n';

  bool k = check_wall_last(map, s_x, s_y, g_x, g_y);
  return k;
}

bool check_wall_last(int map[][Y], int s_x, int s_y, int g_x, int g_y){
  int d_x = g_x - s_x;       //端点同士のx座標の差difference_x
  int d_y = g_y - s_y;       //端点同士のy座標の差difference_y
  int slope1, slope2;
  int flag_d = 0;
  if(d_x == 0)  flag_d = 1;
  else          slope1 = d_y / d_x;  //2点間の傾きd_y / d_x
  if(d_y == 0)  flag_d = 2;
  else          slope2 = d_x / d_y;  //2点間の傾きd_x / d_y

  int sign_x, sign_y;
  if((d_x==0) || (d_y==0)){
    if(d_x==0){
      if(d_y>0) sign_y = 1;
      else      sign_y = -1;
    }
    if(d_y==0){
      if(d_x>0) sign_x = 1;
      else      sign_x = -1;
    }
  }
  else{
    if((d_x>0) && (d_y>0)){ sign_x =  1; sign_y =  1;}
    if((d_x<0) && (d_y>0)){ sign_x = -1; sign_y =  1;}
    if((d_x<0) && (d_y<0)){ sign_x = -1; sign_y = -1;}
    if((d_x>0) && (d_y<0)){ sign_x =  1; sign_y = -1;}
  }

  int flag = 0;
  int i_x, j_y;
  if(flag_d == 1){//x座標が等しい
    int j;
    for(j=1; j<=std::abs(d_y); j++){
      j_y = j * sign_y;
      //std::cout << s_y+j_y << " " << map[s_x][s_y+j_y] << '\n';
      if(flag == 0){
          if(map[s_x][s_y+j_y] == 0)  flag = 1;
        }
      if(flag == 1){
        if(map[s_x][s_y+j_y] == 1)  flag = 2;
      }
      if(flag == 2){
        if(map[s_x][s_y+j_y] == 0)  flag = 3;
        if(map[s_x-1][s_y+j_y]==1 && map[s_x+1][s_y+j_y]==1)  flag = 3;
      }
      if(flag == 3){
        std::cout << "false1\n";
        return false;
      }
    }
    std::cout << "flag:" << flag << "true_01\n";
    return true;
  }
  else if(flag_d == 2){//y座標が等しい
    int i;
    for(i=1; i<=std::abs(d_x); i++){
      i_x = i * sign_x;
      //std::cout << s_x+i_x << " " << map[s_x+i_x][s_y] << '\n';
      if(flag == 0){
          if(map[s_x+i_x][s_y] == 0)  flag = 1;
        }
      if(flag == 1){
        if(map[s_x+i_x][s_y] == 1)  flag = 2;
      }
      if(flag == 2){
        if(map[s_x+i_x][s_y] == 0)  flag = 3;
        if(map[s_x+i_x][s_y-1]==1 && map[s_x+i_x][s_y+1]==1)  flag = 3;
      }
      if(flag == 3){
        std::cout << "false1\n";
        return false;
      }
    }
    std::cout << "flag:" << flag << "true_02\n";
    return true;
  }
  else{
    if(std::abs(slope2) <= std::abs(slope1)){
      int i, j = 1;
      for(i=1; i<=std::abs(d_x); i++){
        int cnt = 0;
        i_x = i * sign_x;
        while(cnt != std::abs(slope1)){
          int flag_ij = 1;
          j_y = j * sign_y;
          if(flag_ij == 1 && flag == 0){
            //if(map[s_x+i_x][s_y    ] == 0){ flag_ij = 0; flag = 1;}
            if(map[s_x+i_x][s_y+j_y] == 0){ flag_ij = 0; flag = 1;}
          }
          if(flag_ij == 1 && flag == 1){
            //if(map[s_x+i_x][s_y    ] == 1){ flag_ij = 0; flag = 2;}
            if(map[s_x+i_x][s_y+j_y] == 1){ flag_ij = 0; flag = 2;}
          }
          if(flag_ij == 1 && flag == 2){
            //if(map[s_x+i_x][s_y    ] == 0)  flag = 3;
            if(map[s_x+i_x][s_y+j_y] == 0)  flag = 3;
          }
          if(flag == 3){
            std::cout << "false1\n";
            return false;
          }
          cnt += 1;
          j += 1;
        }
      }
      //ここまでくれば現在地のx座標は目的地のx座標と一致する(はず)
      while((s_y+j_y) != g_y){
        j_y = j * sign_y;
        if(flag == 0){
          if(map[s_x+i_x][s_y+j_y] == 0)  flag = 1;
        }
        if(flag == 1){
          if(map[s_x+i_x][s_y+j_y] == 1)  flag = 2;
        }
        if(flag == 2){
          if(map[s_x+i_x][s_y+j_y] == 0)  flag = 3;
        }
        if(flag == 3){
          std::cout << "false1\n";
          return false;
        }
        j += 1;
      }
      std::cout << "true1\n";
      return true;
    }
    else{
      int i = 1, j;
      for(j=1; j<=std::abs(d_y); j++){
        int cnt = 0;
        j_y = j * sign_y;
        while(cnt != std::abs(slope2)){
          int flag_ij = 1;
          i_x = i * sign_x;
          if(flag_ij == 1 && flag == 0){
            //if(map[s_x    ][s_y+j_y] == 0){ flag_ij = 0; flag = 1;}
            if(map[s_x+i_x][s_y+j_y] == 0){ flag_ij = 0; flag = 1;}
          }
          if(flag_ij == 1 && flag == 1){
            //if(map[s_x    ][s_y+j_y] == 1){ flag_ij = 0; flag = 2;}
            if(map[s_x+i_x][s_y+j_y] == 1){ flag_ij = 0; flag = 2;}
          }
          if(flag_ij == 1 && flag == 2){
            //if(map[s_x    ][s_y+j_y] == 0)  flag = 3;
            if(map[s_x+i_x][s_y+j_y] == 0)  flag = 3;
          }
          if(flag == 3){
            std::cout << "false2\n";
            return false;
          }
          cnt += 1;
          i += 1;
        }
      }
      //ここまでくれば現在地のy座標は目的地のy座標と一致する(はず)
      while((s_x+i_x) != g_x){
        i_x = i * sign_x;
        if(flag == 0){
          if(map[s_x+i_x][s_y+j_y] == 0)  flag = 1;
        }
        if(flag == 1){
          if(map[s_x+i_x][s_y+j_y] == 1)  flag = 2;
        }
        if(flag == 2){
          if(map[s_x+i_x][s_y+j_y] == 0)  flag = 3;
        }
        if(flag == 3){
          std::cout << "false2\n";
          return false;
        }
        i += 1;
      }
      std::cout << "true2\n";
      return true;
    }
  }
}

/*
[coodinate of nodes]
0(5,5)
1(10,30)
2(10,50)
3(10,60)
4(10,90)
5(20,10)
6(20,40)
7(20,60)
8(20,80)
9(20,90)
10(30,10)
11(30,40)
12(30,80)
13(30,90)
14(40,10)
15(40,70)
16(40,80)
17(50,20)
18(50,40)
19(50,50)
20(50,70)
21(50,80)
22(60,10)
23(60,50)
24(60,60)
25(70,20)
26(70,60)
27(70,70)
28(80,20)
29(80,30)
30(80,40)
31(80,60)
32(80,70)
33(90,20)
34(90,30)
35(90,40)
36(90,50)
37(90,60)
38(90,70)
39(95,95)

[distance between nodes]
0-1 : 25.4951
1-5 : 22.3607
1-6 : 14.1421
2-3 : 10
2-6 : 14.1421
2-7 : 14.1421
2-11 : 22.3607
2-19 : 40
2-23 : 50
2-36 : 80
3-7 : 10
3-15 : 31.6228
3-19 : 41.2311
3-20 : 41.2311
4-8 : 14.1421
4-9 : 10
4-39 : 85.1469
5-6 : 30
5-10 : 10
5-11 : 31.6228
5-14 : 20
5-22 : 40
6-10 : 31.6228
6-11 : 10
7-15 : 22.3607
7-19 : 31.6228
7-20 : 31.6228
7-24 : 40
8-9 : 10
8-12 : 10
8-13 : 14.1421
8-15 : 22.3607
8-16 : 20
9-12 : 14.1421
9-13 : 10
9-39 : 75.1665
10-11 : 30
10-14 : 10
10-22 : 30
11-14 : 31.6228
12-13 : 10
12-15 : 14.1421
12-16 : 10
13-15 : 22.3607
13-16 : 14.1421
13-21 : 22.3607
13-39 : 65.192
14-17 : 14.1421
14-22 : 20
14-25 : 31.6228
15-16 : 10
15-20 : 10
15-21 : 14.1421
15-27 : 30
16-20 : 14.1421
16-21 : 10
16-27 : 31.6228
16-32 : 41.2311
16-38 : 50.9902
17-22 : 14.1421
17-25 : 20
17-28 : 30
17-29 : 31.6228
17-33 : 40
17-34 : 41.2311
18-19 : 10
18-20 : 30
18-23 : 14.1421
18-24 : 22.3607
18-29 : 31.6228
18-30 : 30
18-35 : 40
18-36 : 41.2311
19-20 : 20
19-21 : 30
19-23 : 10
19-24 : 14.1421
19-30 : 31.6228
19-35 : 41.2311
19-36 : 40
20-21 : 10
20-23 : 22.3607
20-24 : 14.1421
20-26 : 22.3607
20-27 : 20
20-32 : 30
20-38 : 40
21-27 : 22.3607
21-32 : 31.6228
21-38 : 41.2311
22-25 : 14.1421
23-24 : 10
23-26 : 14.1421
23-27 : 22.3607
23-30 : 22.3607
23-31 : 22.3607
23-35 : 31.6228
23-36 : 30
23-37 : 31.6228
24-26 : 10
24-27 : 14.1421
24-31 : 20
24-36 : 31.6228
25-28 : 10
25-29 : 14.1421
25-33 : 20
25-34 : 22.3607
26-27 : 10
26-31 : 10
26-32 : 14.1421
26-36 : 22.3607
26-37 : 20
27-31 : 14.1421
27-32 : 10
27-38 : 20
28-29 : 10
28-30 : 20
28-33 : 10
28-34 : 14.1421
28-35 : 22.3607
28-36 : 31.6228
29-30 : 10
29-33 : 14.1421
29-34 : 10
29-35 : 14.1421
29-36 : 22.3607
30-33 : 22.3607
30-34 : 14.1421
30-35 : 10
30-36 : 14.1421
31-32 : 10
31-36 : 14.1421
31-37 : 10
31-38 : 14.1421
32-36 : 22.3607
32-37 : 14.1421
32-38 : 10
33-34 : 10
33-35 : 20
34-35 : 10
34-36 : 20
34-37 : 30
35-36 : 10
35-37 : 20
36-37 : 10
36-38 : 20
37-38 : 10

[cost of nodes]
node:0 cost:0
node:1 cost:25.4951
node:2 cost:53.7794
node:3 cost:63.7794
node:4 cost:126.785
node:5 cost:47.8558
node:6 cost:39.6372
node:7 cost:67.9215
node:8 cost:112.643
node:9 cost:122.643
node:10 cost:57.8558
node:11 cost:49.6372
node:12 cost:104.424
node:13 cost:112.643
node:14 cost:67.8558
node:15 cost:90.2822
node:16 cost:100.282
node:17 cost:175.402
node:18 cost:103.779
node:19 cost:93.7794
node:20 cost:99.5443
node:21 cost:104.424
node:22 cost:87.8558
node:23 cost:103.779
node:24 cost:107.922
node:25 cost:155.402
node:26 cost:117.922
node:27 cost:119.544
node:28 cost:145.402
node:29 cost:135.402
node:30 cost:125.402
node:31 cost:126.14
node:32 cost:129.544
node:33 cost:147.763
node:34 cost:139.544
node:35 cost:135.01
node:36 cost:133.779
node:37 cost:135.402
node:38 cost:139.544
node:39 cost:211.932

Time of calculation:3.438.sec
*/