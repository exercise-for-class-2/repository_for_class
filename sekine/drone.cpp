#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <stdio.h>
#include <time.h>
//#include <unistd.h>

#define GNPLT "C:/PROGRA~1/gnuplot/bin/gnuplot.exe" 
#define STRLN 100
#define X 101       //mapの横幅
#define Y 101       //mapの縦幅
#define Z 1
#define S_X 5       //startのx座標
#define S_Y 5       //startのy座標
#define S_Z 0       //startのz座標
#define G_X 95      //goalのx座標
#define G_Y 95      //goalのy座標
#define G_Z 0       //goalのy座標
#define MAX_COST 10000
#define MAX_NODE 10000

struct Node{                         //このノードから伸びるエッジの情報
    std::vector<int> edges_to;       //各エッジの接続先のノード番号
    std::vector<double> edges_cost;  //各エッジのコスト
    double x;                        //x座標
    double y;                        //y座標
    int path = -1;                   //このノードに入ってくるノード
    double cost = MAX_COST;          //このノードへの現時点で判明している最小コスト
    bool flag = false;               //探索済みか否か
    bool done = false;               //確定ノードか否か
};

struct Drone{
    int x=S_X, y=S_Y, z=S_Z;    //ドローンの現在地の座標
    bool fil[5][5];             //レーザーレンジファインダーより入手した情報を格納 
    bool flag;
    void avoidance();           //緊急回避用プログラム
    void extract();             //地図から端点を抽出する関数
    void Dijkstra();            //Dijkstraによる経路計算をする関数
    void get_map();             //マップ情報の取得
    void update_fil();          //filを更新し、ドローン周辺の障害物情報を格納

    int shortest_route[MAX_NODE];   //最短経路
    int map[Z][X][Y];               //マップ情報を格納
    int map_buffer[Z][X][Y];        //仮想壁考慮のマップ
    int nextnode;                   //次に向かうノード
    int start = -1;                 //start node
    int goal = -1;                  //goal node
};


//すべてのプログラムを起動する
void start_drone();
//x*x + y*yを返す
double calc_distance(double x, double y);
//node間の距離をnode[]に代入
void input_edges_cost(Node node[], int n);
//各ノードの座標を代入 file = xy_edges.dat
void Node_in(struct Node node[], std::string file);
//各ノードまでのコストを計算
void search_node(Node node[], int n, int start);
//探索済みノードから確定ノードを探す
int  search_confirm_node(Node node[], int n, int *cnt);
//色々std::coutしてくれます
void print_array(Node node[], int n);
//node.datの各ノードとその向こう先を書き込む関数 file = node2.dat
void make_node(std::string file, int map[][Y], Node node[], int n);
//file = "xy_edges.dat"から, ノードの個数を返す
int number_of_node(std::string file);
//gnuplotで描画し画像をpngファイルに保存
//int gnuplot_spc(std::string ofile1, std::string ofile2);
int gnuplot_spc(char *file1, char *file2);
//ノード間の壁の判定
bool check_wall(Node node[], int map[][Y], int i_s, int i_g);
//check_wall()内で使う関数
bool check_wall_last(int map[][Y], int s_x, int s_y, int g_x, int g_y);
//dijkstra経路を図示するためのファイル書き込み
void make_dijkstra(std::string file, Node node[], int start, int goal, int route[]);
//startとgoalのノードを保存
void set_goal(Node node[], int n, int start, int *goal);
//startを決める. 一番初めはgoalも決める
void set_start(Node node[], int n, int *strat, int *goal);

//端点を判定する関数. 端点ならばtrueを返す
bool check(int k, int i,int j,int map[][X][Y]);
//gnuplotで地図を描画するためのfileを作成する
void output_dat(char *file_all, char *file_edges, int map[][X][Y], int k);
//仮想壁を作る
void change_map(int map[][X][Y], int map_buffer[][X][Y]);
//map_bufferを見るため
void output_map(int map_buffer[][X][Y]);
//map_bufferの初期化
void initialize_map_buffer(int map_buffer[][X][Y]);

//端点から端点までドローンの現在地を更新するごとにavoidanceを呼び出しつつ進む
void dronego(Drone d);
//mapから必要な点をファイルへ書き込む
void make_dat(Drone d);

/*--------------------main文---------------------------*/
int main(){
    start_drone();
    return 0;
}
/*-----------------------------------------------------*/

/*-------------------start_drone()------------------------*/
void start_drone(){
    Drone d;
    //Drone d;
    d.get_map();
    make_dat(d);
    std::cout << "[OK] Complete get_map()\n";
    d.Dijkstra();
    /*while( d.x!=G_X || d.y!=G_Y || d.z!=G_Z ){
        d.Dijkstra();
        dronego(d);
    }*/
    std::cout << "[OK] Drone arrived at the goal\n";
}
/*--------------------------------------------------------*/


/*-------------------------------------Drone::get_map()関連の関数---------------------------------------------*/
void Drone::get_map(){
    std::string file0_map   = "map0.dat";
    std::string str;
    int tmp;


    //map1, 2, 3...と複数ある場合の効率的な入れ方が思いつかない
    for(int k=0; k<Z; k++){
        std::ifstream ifile(file0_map.c_str());
        for(int i=0; i<X; i++){
            std::getline(ifile, str);
            std::istringstream iss(str);
            int j = 0;
            while(iss >> tmp){
                map[k][i][j] = tmp;
                j++;
            }
        }
        map[k][S_X][S_Y] = 1;
        map[k][G_X][G_Y] = 1;
        ifile.close();
    }
}
/*-------------------------------------------------------------------*/


/*---------------------------make_dat()関連の関数-----------------------------------------*/
void make_dat(Drone d){
    initialize_map_buffer(d.map_buffer);
    change_map(d.map, d.map_buffer);
    output_map(d.map_buffer);
    char file0_all[STRLN]   = "all0.dat";
    char file0_edges[STRLN] = "edges0.dat";
    for(int k=0; k<Z; k++){
        if(k==0){
            output_dat(file0_all, file0_edges, d.map_buffer, k);      
        }
    }
}

void change_map(int map[][X][Y], int map_buffer[][X][Y]){
    for(int k=0; k<Z; k++){
        for(int i=1; i<X-1; i++){
            for(int j=1; j<Y-1; j++){
                if((i==S_X && j==S_Y) || (i==G_X && j==G_Y)){
                    map_buffer[k][i][j] = 0;
                }
                else{
                    if(map[k][i][j] != 0){
                        if((map[k][i][j+1]!=0 && map[k][i-1][j]!=0) ||   
                        (map[k][i-1][j]!=0 && map[k][i][j-1]!=0) ||
                        (map[k][i][j-1]!=0 && map[k][i+1][j]!=0) ||
                        (map[k][i+1][j]!=0 && map[k][i][j+1]!=0)){
                            map_buffer[k][i-1][j-1]=1; map_buffer[k][i-1][j]=1; map_buffer[k][i-1][j+1]=1;
                            map_buffer[k][i  ][j-1]=1; map_buffer[k][i  ][j]=1; map_buffer[k][i  ][j+1]=1;
                            map_buffer[k][i+1][j-1]=1; map_buffer[k][i+1][j]=1; map_buffer[k][i+1][j+1]=1;
                        }
                        else if(map[k][i][j-1] != 0 && map[k][i][j+1] == 0){
                            map_buffer[k][i][j-1] = 1;
                        }
                        else if(map[k][i][j-1] == 0 && map[k][i][j+1] != 0){
                            map_buffer[k][i][j+1] = 1;
                        }
                        else{
                            map_buffer[k][i][j-1] = 1;
                            map_buffer[k][i][j+1] = 1;
                        }
                    }
                }
            }
        }
        for(int i=1; i<X-1; i++){
            for(int j=1; j<Y-1; j++){
                if((i==S_X && j==S_Y) || (i==G_X && j==G_Y)){
                    map_buffer[k][i][j] = 0;
                }
                else{
                    if(map[k][j][i] != 0){
                        if((map[k][j][i+1]!=0 && map[k][j-1][i]!=0) ||   
                        (map[k][j-1][i]!=0 && map[k][j][i-1]!=0) ||
                        (map[k][j][i-1]!=0 && map[k][j+1][i]!=0) ||
                        (map[k][i+1][j]!=0 && map[k][i][j+1]!=0)){
                            map_buffer[k][j-1][i-1]=1; map_buffer[k][j-1][i]=1; map_buffer[k][j-1][i+1]=1;
                            map_buffer[k][j  ][i-1]=1; map_buffer[k][j  ][i]=1; map_buffer[k][j  ][i+1]=1;
                            map_buffer[k][j+1][i-1]=1; map_buffer[k][j+1][i]=1; map_buffer[k][j+1][i+1]=1;
                        }
                        else if(map[k][j-1][i] != 0 && map[k][j+1][i] == 0){
                            map_buffer[k][j-1][i] = 1;
                        }
                        else if(map[k][j-1][i] == 0 && map[k][j+1][i] != 0){
                            map_buffer[k][j+1][i] = 1;
                        }
                        else{
                            map_buffer[k][j-1][i] = 1;
                            map_buffer[k][j+1][i] = 1;
                        }
                    }
                }
            }
        }
        for(int i=0; i<X; i++){
            map_buffer[k][i][0] = 1;
            map_buffer[k][0][i] = 1;
            map_buffer[k][100][i] = 1;
            map_buffer[k][i][100] = 1;
        }
    }
}

void initialize_map_buffer(int map_buffer[][X][Y]){
    for(int k=0; k<Z; k++){
        for(int i=0; i<X; i++){
            for(int j=0; j<Y; j++){
                map_buffer[k][i][j] = 0;
            }
        }
    }
}

void output_map(int map_buffer[][X][Y]){
    std::string ofile = "map0_buffer.dat";
    std::ofstream ofp(ofile.c_str());
    for(int i=0; i<X; i++){
        for(int j=0; j<Y; j++){
            ofp << map_buffer[0][i][j] << " ";
        }
        ofp << '\n';
    }
    ofp.close();
}

void output_dat(char *file_all, char *file_edges, int map[][X][Y], int k){
    //file_all
    FILE *fp;
    fp = fopen(file_all, "w");
    if(fp == NULL){
        printf("ERROR\n");
        exit(1);
    }
    fprintf(fp, "%d %d\n", S_X, S_Y);
    for(int i=0; i<X; i++){
        for(int j=0; j<Y; j++){
            if(map[k][i][j] != 0){
                fprintf(fp, "%d %d\n", i, j);
            }
        }
    }
    fprintf(fp, "%d %d\n", G_X, G_Y);
    fclose(fp);

    //file_edges
    fp = fopen(file_edges, "w");
    if(fp == NULL){
        printf("ERROR\n");
        exit(1);
    }
    fprintf(fp, "%d %d\n", S_X, S_Y);
    for(int i=0; i<X; i++){
        for(int j=0; j<Y; j++){
            if(check(k, i, j, map)){
                fprintf(fp, "%d %d\n", i, j);
            }
        }
    }
    fprintf(fp, "%d %d\n", G_X, G_Y);
    fclose(fp);    
}

bool check(int k,int i,int j,int map[][X][Y]){
    if(i==0 || i==X-1|| j==0 || j==Y-1){
        return false;
    }
    if(map[k][i-1][j]!=0 && map[k][i+1][j]!=0){
        return false;
    }
    if(map[k][i][j-1]!=0 && map[k][i][j+1]!=0){
        return false;
    }
    if(i!=0 || i!=X-1 || j!=0 || j!=Y-1){
        if( (map[k][i][j+1]==0 && map[k][i-1][j]==0 && map[k][i][j-1]!=0 && map[k][i+1][j]!=0) ||  
            (map[k][i][j+1]!=0 && map[k][i-1][j]==0 && map[k][i][j-1]==0 && map[k][i+1][j]!=0) ||   
            (map[k][i][j+1]!=0 && map[k][i-1][j]!=0 && map[k][i][j-1]==0 && map[k][i+1][j]==0) ||  
            (map[k][i][j+1]==0 && map[k][i-1][j]!=0 && map[k][i][j-1]!=0 && map[k][i+1][j]==0)){ 
                return true;
        }
    }      
    if(map[k][i][j]!=0){
        return true;
    }
    return false;
}
/*----------------------------------------------------------------------------------------------------------*/


/*-----------------------------------Drone::Dijkstra()--------------------------------------------------------*/
void Drone::Dijkstra(){
    if(z == S_Z){//各階層ごとに同じ処理をしたい
        std::string file0_map = "map0.dat";
        std::string file0_all = "all0.dat";
        std::string file0_edges = "edges0.dat";
        std::string file0_node = "node0.dat";
        std::string file0_dijkstra = "dijkstra0.dat";
        char f0_all[STRLN] = "all0";
        char f0_edges[STRLN] = "edges0";
        char f0_dijkstra[STRLN] = "dijkstra0";

        initialize_map_buffer(map_buffer);
        change_map(map, map_buffer);
        output_map(map_buffer);

        //output_map(map_buffer);
        int n = number_of_node(file0_edges);            //node(端点)の総数
        Node node[n];
        Node_in(node, file0_edges);                     //端点をノードとしてnode[]に保存
        make_node(file0_node, map_buffer[0], node, n);  //壁を判定してノードが向かうことのできるノードを保存していく
        input_edges_cost(node, n);                      //node間の距離を保存
        set_start(node, n, &start, &goal);              //startを決める. 一番初めはゴールも決める
        search_node(node, n, start);                    //スタートから到達可能なすべてのノードへの最小コストと最短経路を保存
        set_goal(node, n, start, &goal);                /*現在いる階層でのスタートノードとゴールノードを決める. 
                                                        現在の階層から最終的なゴールに到達できない場合のゴールノードの選び方は未定*/
        make_dijkstra(file0_dijkstra, node, start, goal, shortest_route);   //startからgoalまでの最短経路を保存
        print_array(node, n);
        gnuplot_spc(f0_all, f0_dijkstra);         //ドローンの軌跡をpngに保存
        gnuplot_spc(f0_edges, f0_dijkstra);       //ドローンの軌跡をpngに保存

    }
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

void Node_in(struct Node node[], std::string file){
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

double calc_distance(double x, double y){
    return std::sqrt(x*x + y*y);
}

void input_edges_cost(Node node[], int n){
    for(int i=0; i<n; i++){
        int n_edges_to = node[i].edges_to.size();
        for(int j=0; j<n_edges_to; j++){
            node[i].edges_cost.push_back(calc_distance((node[i].x - node[node[i].edges_to[j]].x), (node[i].y - node[node[i].edges_to[j]].y)));   
        }
    }
}

void set_start(Node node[], int n, int *start, int *goal){
    if((*start) == -1 && (*goal) == -1){
        *start = 0;
        *goal = n-1;
    }
    else{
        *start = *goal;
        *goal = -1;
    }
}

void search_node(Node node[], int n, int start){
    int cnt = 0;            //sになったらすべて探索済み
    int n_max;
    int i = start;
    int j = 0;
    int n_edges_to = node[i].edges_to.size();
    node[i].cost = 0;       //startまでのcostは0
    node[i].path = start;
    node[i].flag = true;    //探索済み
    node[i].done = true;    //確定ノード
    cnt += 1;
    while(j<n_edges_to){
	    if(node[node[i].edges_to[j]].flag == false){
	        node[node[i].edges_to[j]].cost = node[i].edges_cost[j];
            node[node[i].edges_to[j]].flag = true;
            node[node[i].edges_to[j]].path = i;
	        j += 1;
	     }
    }
    i = search_confirm_node(node, n, &cnt);     //確定ノードを探す
    node[i].done = true;                        //node iを確定ノードにする
    cnt += 1;
    while(cnt < n){//到達可能なすべてのノードが確定ノードになるまで繰り返す
        j = 0;
        n_edges_to = node[i].edges_to.size();	        //node[i].edges_toの要素数
        while(j<n_edges_to){
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
    i = search_confirm_node(node, n, &cnt);     //確定ノードを探す
    if(i != -1){
        node[i].done = true;                    //node iを確定ノードにする
        cnt += 1;
        }
    }
}

int search_confirm_node(Node node[], int n, int *cnt){
    int i = MAX_COST, j;
    int min = MAX_COST;
    for(j=0; j<n; j++){
        if((node[j].flag == true) && (node[j].done == false)){
            if(node[j].cost < min){
                min = node[j].cost;
                i = j;
            }
        }
    }
    if(i == MAX_COST){
        (*cnt) = n;
        return -1;
    }
    return i;
}

void set_goal(Node node[], int n, int start, int *goal){
    if(node[*goal].path != -1){//goalに到達可能なら
        *goal = n-1;           //goalは最終的なゴール
    }
    else{//現在の階層のstartからでは到達不可能なら
    /*次に向かう階層に行くことのできる地点を探してそこをgoalとする*/
    /*その決め方は未定. 今は形式的にgoal = n-1としておく*/
    *goal = n-1;
    }
}

void make_dijkstra(std::string file, Node node[], int start, int goal, int route[]){
    std::ofstream ofp(file.c_str());
    int i = goal;  
    int j = 0;
    int tmp[MAX_NODE];
    while(i != start){//goalからstartに戻るまで start node は今は0で固定
        tmp[j] = i;
        j++;
        ofp << node[i].x << " " << node[i].y << '\n';
        i = node[i].path;
    }
    tmp[j] = i;
    j++;
    ofp << node[i].x << " " << node[i].y << '\n';
    ofp.close();

    int k = j;
    for(int j=0; j<k; j++){
        route[j] = tmp[k-j-1];
    }
    std::cout << '\n';
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

/*-------------------------------------------------------------------------------------------------*/


int gnuplot_spc(char *file1, char *file2){
    /*char file1[STRLN]="all0", file2[STRLN]="dijkstra0";
    int i = 0;
    while(ofile1[i] != '.'){
      file1[i] = ofile1[i];
      i++;
    }
    i = 0;
    while(ofile2[i] != '.'){
      file2[i] = ofile2[i];
      i++;
    }*/
    printf("%s %s\n", file1, file2);
    FILE *gp; if((gp = _popen(GNPLT, "w")) == NULL) { printf("ERR\n"); exit(1); }
    fprintf(gp, "set size square\nset colorsequence classic\n");
    fprintf(gp, "set style l 1 lt 1 lc 1 lw 1 pt 5 ps 1\n");
    fprintf(gp, "set style l 2 lt 1 lc 3 lw 1 pt 5 ps 1\n");
    fprintf(gp, "set ticscale 0\nset xtics 10\nset ytics 10\n");
    fprintf(gp, "set xrange[0:100]\nset yrange[0:100]\n");
    fprintf(gp, "set terminal png\n");
    //fprintf(gp, "set output '%s.png'\n", file1);
    fprintf(gp, "plot '%s.dat' linestyle 1\n", file1);
    fprintf(gp, "set output '%s.png'\n", file1);
    fprintf(gp, "replot '%s.dat' with lp linestyle 2\n", file2);
    //system("pause"); 
    fprintf(gp, "exit\n");
    return _pclose(gp);
}

void print_array(Node node[], int n){
  //各ノード間の距離を表示
  int label[n];
  for(int i=0; i<n; i++){
      label[i] = 0;
  }
  std::cout << "[distance between nodes]\n";
  for(int i=0; i<n; i++){
    int n_edges = node[i].edges_to.size();
    for(int j=0; j<n_edges; j++){
      if(label[node[i].edges_to[j]] == 0){
        std::cout << i << "-" << node[i].edges_to[j] << " : " << node[i].edges_cost[j] << '\n';
      }
    }
    label[i] = 1;
  }
  std::cout << '\n';

  //各ノードまでのコストを表示
  std::cout << "[cost of nodes]\n";
  for(int i=0; i<n; i++){
    std::cout  << "node:" << i << "(" << node[i].x << "," << node[i].y << ") ";
    std::cout<< " cost:" << node[i].cost << '\n';
  }
  std::cout << '\n';

  //各ノードに入ってくるノード
  std::cout << "[path]\n";
  for(int i=0; i<n; i++){
    std::cout << i << "-" << node[i].path << '\n';
  }
  std::cout << '\n';

  //最短経路を表示
  std::cout << "[Dijkstra rote]\n";
  int dij[n];
  int i_d = 0;
  int k = n-1;
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