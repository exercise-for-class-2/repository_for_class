#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>

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
//仮想壁を作る
void change_map(int map[][Y], int map_copy[][Y]);

int main() {

    int x = X, y = Y;
    std::string file1 = "map2.dat";

    int map[X][Y];
    input_map(file1, map, x, y);          //map[x][y]にfile1の内容を代入していく
    int map_copy[X][Y];
    change_map(map, map_copy);

    std::string ofile1 = "xy_all2.dat";   //全体を描画するためのfile
    std::string ofile2 = "xy_edges2.dat"; //端点を描画するためのfile
    output_dat(ofile1, map, x, y);        //障害物を記録
    output_dat(ofile2, map, x, y);        //端点を記録

    std::cout << "Please check 'xy_all2.dat' & 'xy_edges2.dat'\n";
    std::cout << "Please compile 'Dijkstra_5.cpp'\n";

    return 0;
}

bool check(int i,int j,int x,int y,int map[][Y]){
    if(i==0 || i==x-1|| j==0 || j==y-1){
        return false;
    }
    if(map[i-1][j]==1 && map[i+1][j]==1){
        return false;
    }
    if(map[i][j-1]==1 && map[i][j+1]==1){
        return false;
    }
    if(map[i][j]==1){
        return true;
    }
    return false;
}
/*
bool check(int i,int j,int x,int y,int map[][Y]){ 

    bool s=false,v=false,ru=false,lu=false,rd=false,ld=false; 
    int cnt = 0; 

    for(int k=-1;k<2;k++){ 
        for(int l=-1;l<2;l++){ 
            if(map[j+k][i+l]==1){ 
                cnt++; 
            } 
        } 
    } 
 
    if(map[j][i]==0){ 
        return false; 
    } 
 
    if(i==0 || i==y-1|| j==0 || j==x-1){//壁 
        return true; 
    } 
    if(map[j-1][i]==1 && map[j+1][i]==1){//横 
        s = true; 
    } 
    if(map[j][i-1]==1 && map[j][i+1]==1){//縦 
        v = true; 
    } 
 
    if(map[j+1][i+1]==1){ 
        lu = true; 
    } 
 
    if(map[j-1][i+1]==1){ 
        ru = true; 
    } 

    if(map[j-1][i-1]==1){ 
        rd = true; 
    } 
 
    if(map[j+1][i-1]==1){ 
        ld = true; 
    } 
     
    if(s && v){ 
        if(!ru && map[j][i+2]==1 && map[j+2][i]==1){ 
            return true; 
        }else if(!rd && map[j+2][i]==1 && map[j][i-2]==1){ 
            return true;
        }else if(!lu && map[j][i+2]==1 && map[j-2][i]==1){ 
            return true; 
        }else if(!ld && map[j-2][i]==1 && map[j][i-2]==1){ 
            return true; 
        } 
    } 
    if(s || v){ 
        return false; 
    } 
    if(cnt==0){ 
        return false; 
    } 
    if(!s && !v){ 
        return true; 
    } 
     
    return false; 
} 
*/
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
}

void output_dat(std::string ofile, int map[][Y], int x, int y){
    int s_x = S_X;   //startのx座標
    int s_y = S_Y;   //startのy座標
    int g_x = G_X;   //goalのx座標
    int g_y = G_Y;   //goalのy座標
    if(ofile == "xy_all2.dat"){
        std::ofstream ofp(ofile.c_str());
        ofp << s_x << " " << s_y << '\n';
        for(int i=0;i<x;i++){
            for(int j=0;j<y;j++){
                if(map[i][j]==1){
                    ofp << i << " " << j << "\n";
                }
            }
        }
        ofp << g_x << " " << g_y << '\n';
        ofp.close();
    }
    else if(ofile == "xy_edges2.dat"){
        std::ofstream ofp(ofile.c_str());
        ofp << s_x << " " << s_y << '\n';
        for(int i=0;i<x;i++){
            for(int j=0;j<y;j++){
                if(check(i,j,x,y,map)){
                    ofp << i << " " << j << "\n";
                }
            }
        }
        ofp << g_x << " " << g_y << '\n';
        ofp.close();
    }
}

void change_map(int map[][Y], int map_copy[][Y]){
    for(int i=0; i<X; i++){
        for(int j=0; j<Y; j++){
            
        }
    }
}