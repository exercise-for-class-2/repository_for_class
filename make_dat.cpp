#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <time.h>

#define GNPLT "C:/PROGRA~1/gnuplot/bin/gnuplot.exe"
#define X 101   //地図の横の大きさ make_map.cppで計算されたxの値をあらかじめ定義しておく
#define Y 101   //地図の縦の大きさ make_map.cppで計算されたyの値をあらかじめ定義しておく
#define S_X 5   //startのx座標
#define S_Y 5   //startのy座標
#define G_X 95  //goalのx座標
#define G_Y 95  //goalのy座標

//端点を判定する関数. 端点ならばtrueを返す
bool check(int i,int j,int x,int y,int map[][Y]);
//map[X][Y]に0,1地図情報を保存する
void input_map(std::string file, int map[][Y], int x, int y);
//gnuplotで地図を描画するためのfileを作成する
void output_dat(std::string ofile, int map[][Y], int x, int y);

int main() {

    clock_t start = clock();
    int x = X, y = Y;
    std::string file1 = "map2.dat";

    int map[x][y];
    input_map(file1, map, x, y);          //map[x][y]にfile1の内容を代入していく

    std::string ofile1 = "xy_all2.dat";   //全体を描画するためのfile
    std::string ofile2 = "xy_edges2.dat"; //端点を描画するためのfile
    output_dat(ofile1, map, x, y);        //障害物を記録
    output_dat(ofile2, map, x, y);        //端点を記録

    clock_t end = clock();
    //演算時間の表示
    std::cout << "Time of calculation:" << (double)(end-start)/CLOCKS_PER_SEC << ".sec\n";
    
    return 0;
}


bool check(int i,int j,int x,int y,int map[][Y]){
    if(i==0 || i==y-1|| j==0 || j==x-1){
        return false;
    }
    if(map[j-1][i]==1 && map[j+1][i]==1){
        return false;
    }
    if(map[j][i-1]==1 && map[j][i+1]==1){
        return false;
    }
    if(map[j][i]==1){
        return true;
    }
    return false;
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

void output_dat(std::string ofile, int map[][Y], int x, int y){
    int s_x = S_X;   //startのx座標
    int s_y = S_Y;   //startのy座標
    int g_x = G_X;   //goalのx座標
    int g_y = G_Y;   //goalのy座標
    if(ofile == "xy_all2.dat"){
        std::ofstream ofp(ofile.c_str());
        ofp << s_x << " " << s_y << '\n';
        ofp << g_x << " " << g_y << '\n';
        for(int i=0;i<y;i++){
            for(int j=0;j<x;j++){
                if(map[j][i]==1){
                    ofp << j << " " << i << "\n";
                }
            }
        }
        ofp.close();
    }
    else if(ofile == "xy_edges2.dat"){
        std::ofstream ofp(ofile.c_str());
        ofp << s_x << " " << s_y << '\n';
        for(int i=0;i<y;i++){
            for(int j=0;j<x;j++){
                if(check(i,j,x,y,map)){
                    ofp << j << " " << i << "\n";
                }
            }
        }
        ofp << g_x << " " << g_y << '\n';
        ofp.close();
    }
}