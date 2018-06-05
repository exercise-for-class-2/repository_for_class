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
#define MAX_COST 10000

struct Node{                       //このノードから伸びるエッジの情報
  std::vector<int> edges_to;       //各エッジの接続先のノード番号
  std::vector<double> edges_cost;  //各エッジのコスト
  double x;                        //x座標
  double y;                        //y座標
  int path = -1;                   //このノードに入ってくるノード
  double cost = MAX_COST;          //このノードへの現時点で判明している最小コスト
  bool flag = false;               //探索済みか否か
  bool done = false;               //確定ノードか否か               
};

//map[X][Y]に地図を保存する
void input_map(std::string file, int map[][Y], int x, int y);
//仮想壁を作る
void change_map(int map[][Y], int map_buffer[][Y]);
//x*x + y*yを返す
double calc_distance(double x, double y);
//node間の距離をnode[]に代入
void input_edges_cost(Node node[], int s);
//各ノードの座標を代入 file = xy_edges.dat
void Node_in(struct Node node[], std::string file);
//各ノードまでのコストを計算
void search_node(Node node[], int s);
//探索済みノードから確定ノードを探す
int  search_confirm_node(Node node[], int s, int *cnt);
//色々std::coutしてくれます
void print_array(Node node[], int s);
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
//dijkstra経路を図示するためのファイル書き込み
void make_dijkstra(std::string file, Node node[], int n);


int main(){

  clock_t start = clock();

  std::string file1 = "map2.dat";      //記入済み
  //std::string file1 = "map2_buffer.dat"; //記入済み
  std::string file2 = "node2.dat";       //未記入
  std::string file3 = "xy_all2.dat";     //記入済み
  std::string file4 = "xy_edges2.dat";   //記入済み
  std::string file5 = "Dijkstra2.dat";   //未記入

  int n;
  n = number_of_node(file4);
  Node node[n];

  int x=X, y=Y;
  int map[X][Y];
  input_map(file1, map, x, y);  //map[x][y]にfile1に保存された0,1の地図を代入していく
  int map_buffer[X][Y];
  change_map(map, map_buffer);
  
  Node_in(node, file4);              //nodeの(x, y)を代入
  make_node(file2, map_buffer, node, n);    //node.datに記入, node[].edges_toに代入

  input_edges_cost(node, n);    //edges_costを計算
  search_node(node, n);         //各ノードまでのコストを代入

  make_dijkstra(file5, node, n);

  print_array(node, n);     //いろいろ出力

  gnuplot_spc(file3);          //地図を保存
  gnuplot_spc(file4);          //端点を保存


  clock_t end = clock();
  //演算時間の表示
  std::cout << "Time of calculation:" << (double)(end-start)/CLOCKS_PER_SEC << ".sec\n";
  std::cout << "Please check 'xy_all2.png' & 'xy_edges2.png'\n";
    
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


void change_map(int map[][Y], int map_buffer[][Y]){
  for(int i=1; i<X-1; i++){
    for(int j=1; j<Y-1; j++){
      if(map[i][j] == 1){
        if((map[i][j+1]==1 && map[i-1][j]==1) ||   
           (map[i-1][j]==1 && map[i][j-1]==1) ||
           (map[i][j-1]==1 && map[i+1][j]==1) ||
           (map[i+1][j]==1 && map[i][j+1])){
              map_buffer[i-1][j-1]=1; map_buffer[i-1][j]=1; map_buffer[i-1][j+1]=1;
              map_buffer[i  ][j-1]=1; map_buffer[i  ][j]=1; map_buffer[i  ][j+1]=1;
              map_buffer[i+1][j-1]=1; map_buffer[i+1][j]=1; map_buffer[i+1][j+1]=1;
        }
        else if(map[i][j-1] == 1 && map[i][j+1] == 0){
          map_buffer[i][j-1] = 1;
        }
        else if(map[i][j-1] == 0 && map[i][j+1] == 1){
          map_buffer[i][j+1] = 1;
        }
        else{
          map_buffer[i][j-1] = 1;
          map_buffer[i][j+1] = 1;
        }
      }
    }
  }
  for(int i=1; i<X-1; i++){
    for(int j=1; j<Y-1; j++){
      if(map[j][i] == 1){
        if((map[j][i+1]==1 && map[j-1][i]==1) ||   
           (map[j-1][i]==1 && map[j][i-1]==1) ||
           (map[j][i-1]==1 && map[j+1][i]==1) ||
           (map[i+1][j]==1 && map[i][j+1])){
              map_buffer[j-1][i-1]=1; map_buffer[j-1][i]=1; map_buffer[j-1][i+1]=1;
              map_buffer[j  ][i-1]=1; map_buffer[j  ][i]=1; map_buffer[j  ][i+1]=1;
              map_buffer[j+1][i-1]=1; map_buffer[j+1][i]=1; map_buffer[j+1][i+1]=1;
        }
        else if(map[j-1][i] == 1 && map[j+1][i] == 0){
          map_buffer[j-1][i] = 1;
        }
        else if(map[j-1][i] == 0 && map[j+1][i] == 1){
          map_buffer[j+1][i] = 1;
        }
        else{
          map_buffer[j-1][i] = 1;
          map_buffer[j+1][i] = 1;
        }
      }
    }
  }
  for(int i=0; i<X; i++){
    map_buffer[i][0] = 1;
    map_buffer[0][i] = 1;
    map_buffer[100][i] = 1;
    map_buffer[i][100] = 1;
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
  int cnt = 0;    //sになったらすべて探索済み
  int n_max;
  int i = 0;
  int j = 0;
  int n = node[i].edges_to.size();
  node[i].cost = 0;       //startまでのcostは0
  node[i].path = 0;
  node[i].flag = true;    //探索済み
  node[i].done = true;    //確定ノード
  cnt += 1;
  while(j<n){
	  if(node[node[i].edges_to[j]].flag == false){
	    node[node[i].edges_to[j]].cost = node[i].edges_cost[j];
      node[node[i].edges_to[j]].flag = true;
      node[node[i].edges_to[j]].path = i;
	    j += 1;
	  }
  }
  i = search_confirm_node(node, s, &cnt);   //確定ノードを探す
  node[i].done = true;                   //node iを確定ノードにする
  cnt += 1;
  while(cnt < s){   //goalが確定ノードになるまで繰り返す
    j = 0;
    n = node[i].edges_to.size();	//node[i].edges_toの要素数
    while(j<n){
      if(node[node[i].edges_to[j]].flag == false){
        node[node[i].edges_to[j]].cost = node[i].cost + node[i].edges_cost[j];
        node[node[i].edges_to[j]].flag = true;
        node[node[i].edges_to[j]].path = i;
      }
      else if(node[i].cost + node[i].edges_cost[j] < node[node[i].edges_to[j]].cost){
        node[node[i].edges_to[j]].cost = node[i].cost + node[i].edges_cost[j];
        node[node[i].edges_to[j]].path = i;
      }
      j += 1;
    }
    i = search_confirm_node(node, s, &cnt);   //確定ノードを探す
    if(i != -1){
      node[i].done = true;                   //node iを確定ノードにする
      cnt += 1;
    }
  }
}

int search_confirm_node(Node node[], int s, int *cnt){
  int i = MAX_COST, j;
  int min = MAX_COST;
  for(j=0; j<s; j++){
    if((node[j].flag == true) && (node[j].done == false)){
      if(node[j].cost < min){
        min = node[j].cost;
        i = j;
      }
    }
  }
  if(i == MAX_COST){
    (*cnt) = s;
    return -1;
  }
  return i;
}

void print_array(Node node[], int s){
  //各ノードの座標を表示
 /* std::cout << "[coodinate of nodes]\n";
  for(int i=0; i<s; i++){
    std::cout << i << "(" << node[i].x << "," << node[i].y << ")\n";
  }
  std::cout << '\n';
*/
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
    std::cout  << "node:" << i << "(" << node[i].x << "," << node[i].y << ") ";
    std::cout<< " cost:" << node[i].cost << '\n';
  }
  std::cout << '\n';

  //各ノードに入ってくるノード
  std::cout << "[path]\n";
  for(int i=0; i<s; i++){
    std::cout << i << "-" << node[i].path << '\n';
  }
  std::cout << '\n';

  //最短経路を表示
  std::cout << "[Dijkstra rote]\n";
  int dij[s];
  int i_d = 0;
  int k = s-1;
  dij[i_d] = k;
  i_d += 1;
  while(k != 0){
    dij[i_d] = node[k].path;
    i_d += 1;
    k = node[k].path;
  }
  for(int i=i_d-1; i>=0; i--){
    std::cout << dij[i] << " ";
  }
  std::cout << '\n' << '\n';
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
    fprintf(gp, "set style l 2 lt 1 lc 3 lw 1 pt 5 ps 1\n");
    fprintf(gp, "set ticscale 0\nset xtics 10\nset ytics 10\n");
    fprintf(gp, "set xrange[0:100]\nset yrange[0:100]\n");
    fprintf(gp, "unset key\n");
    if(ofile == "xy_all2.dat"){
      fprintf(gp, "set terminal png\n");
      fprintf(gp, "plot 'xy_all2.dat' linestyle 1\n");
      fprintf(gp, "set output 'xy_all2.png'\n");
      fprintf(gp, "replot 'Dijkstra2.dat' with lp linestyle 2\n");
    }
    else if(ofile == "xy_edges2.dat"){
      fprintf(gp, "set terminal png\n");
      fprintf(gp, "set output 'xy_edges2.png'\n");
      fprintf(gp, "plot 'xy_edges2.dat' linestyle 1\n");
    }
    //system("pause"); fprintf(gp, "exit\n");
    return _pclose(gp);
}

bool check_wall(Node node[], int map[][Y], int i_s, int i_g){
  int s_x = node[i_s].x;
  int s_y = node[i_s].y;
  int g_x = node[i_g].x;
  int g_y = node[i_g].y;

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
      if(flag == 0){
        if(map[s_x+1][s_y+j_y]!=0 && map[s_x-1][s_y+j_y]!=0)  flag = 3;
      }
      if(flag == 3){
        return false;
      }
    }
    return true;
  }
  else if(flag_d == 2){//y座標が等しい
    int i;
    for(i=1; i<=std::abs(d_x); i++){
      i_x = i * sign_x;
      if(flag == 0){
        if(map[s_x+i_x][s_y+1]!=0 && map[s_x+i_x][s_y-1]!=0)  flag = 3;
      }
      if(flag == 3){
        return false;
      }
    }
    return true;
  }
  else{
    if(std::abs(slope2) <= std::abs(slope1)){
      int i = 1, j = 1;
      int flag_ij;
      for(i=1; i<std::abs(d_x); i++){
        int cnt = 0;
        i_x = i * sign_x;
        while(cnt != std::abs(slope1)){
          flag_ij = 1;
          j_y = j * sign_y;
          if(flag_ij ==1 && flag == 0){
            if(map[s_x+i_x][s_y+j_y] != 0){ flag_ij = 0; flag = 3;}
            else if(map[s_x+i_x][s_y+j_y] == 0){ flag_ij = 0; flag = 1;}
          }
          if(flag_ij == 1 && flag == 1){
            if(map[s_x+i_x][s_y+j_y] != 0){ flag_ij = 0; flag = 2;}
          }
          if(flag_ij == 1 && flag == 2){
            flag = 3;
          }
          if(flag == 3){
            return false;
          }
          cnt += 1;
          j += 1;
        }
      }
      j_y = j * sign_y;
      i_x = i * sign_x;
      //ここまでくれば現在地のx座標は目的地のx座標と一致する(はず)
      while((s_y+j_y) != g_y){
        flag_ij = 1;
        flag = 0;
        if(flag == 0){
          if(map[s_x+i_x+1][s_y+j_y] != 0 && map[s_x+i_x-1][s_y+j_y] != 0){flag = 3;}
        }
        if(flag == 3){
          return false;
        }
        j += 1;
        j_y = j * sign_y;
      }
      return true;
    }
    else{
      int i = 1, j = 1;
      int flag_ij;
      for(j=1; j<std::abs(d_y); j++){
        int cnt = 0;
        j_y = j * sign_y;
        while(cnt != std::abs(slope2)){
          flag_ij = 1;
          i_x = i * sign_x;
          if(flag_ij == 1 && flag == 0){
            if(map[s_x+i_x][s_y+j_y] != 0){ flag_ij = 0; flag = 3;}
            else if(map[s_x+i_x][s_y+j_y] == 0){ flag_ij = 0; flag = 1;}
          }
          if(flag_ij == 1 && flag == 1){
            if(map[s_x+i_x][s_y+j_y] != 0){ flag_ij = 0; flag = 2;}
          }
          if(flag_ij == 1 && flag == 2){
            flag = 3;
          }
          if(flag == 3){
            return false;
          }
          cnt += 1;
          i += 1;
        }
      }
      i_x = i * sign_x;
      j_y = j * sign_y;
      //ここまでくれば現在地のy座標は目的地のy座標と一致する(はず)
      while((s_x+i_x) != g_x){
        flag_ij = 1;
        flag = 0;
        if(flag == 0){
          if(map[s_x+i_x][s_y+j_y+1] != 0 && map[s_x+i_x][s_y+j_y-1] != 0){flag = 3;}
        }
        if(flag == 3){
          return false;
        }
        i += 1;
        i_x = i * sign_x;
      }
    return true;
    }
  }
}

void make_dijkstra(std::string file, Node node[], int n){
  std::ofstream ofp(file.c_str());
  int i = n-1;
  while(i != 0){
    ofp << node[i].x << " " << node[i].y << '\n';
    i = node[i].path;
  }
  ofp << node[i].x << " " << node[i].y << '\n';
  ofp.close();
}