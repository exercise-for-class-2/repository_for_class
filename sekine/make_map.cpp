#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>

#define GNPLT "C:/PROGRA~1/gnuplot/bin/gnuplot.exe"  // gnuplot.exe のある場所. PROGRA~1 は Program Files を意味する.
#define STRLN 100                                    // 文字列長の上限.
#define SIZE  100                                    // 2次元自由空間の規模.
#define X 101
#define Y 101

//決められた条件の下, 0 or 1を返す
int spc_ex2(double x, double y);
//spc_ex(x, y)を引数としてfileに0 or 1を書き込んでいく
void spcmap(std::string file, int (*spc)(double, double));
//0 or 1で表された地図が保存されているfileにアクセスし, 地図の横と縦の長さをx, yに保存する
void count_xy(std::string file, int *x, int *y);

int main() {
    int x=0, y=0;
/*
    std::string file1 = "map1.dat";
    spcmap(file1, spc_ex1);              //0と1の羅列をfile1に保存
    count_xy(file1, &x, &y);            //縦と横の長さをx, yに代入
*/
    std::string file2 = "map00.dat";
    spcmap(file2, spc_ex2);
    count_xy(file2, &x, &y);
    std::cout << "x:" << x << " y:" << y << '\n';  //make_dat.cppとDijkstra_5.cppで使う地図の縦と横の長さ
/*
    std::string file3 = "map3.dat";
    spcmap(file3, spc_ex3);
    count_xy(file3, &x, &y);
    
    std::string file4 = "map4.dat";
    spcmap(file4, spc_ex4);
    count_xy(file4, &x, &y);
*/
    std::cout << "Please check 'map2.dat'\n";
    std::cout << "Please compile 'make_dat.cpp'\n";
    
return 0;
}

/*
int spc_ex1(double x, double y) {                                                             // 自由空間なら0, 障害物なら1を返戻.
    if((x > 0) && (x < 100)) {                                                                // 自由空間は水平座標0と100の間.
        if(y <   0) return 1;                                                                 // 垂直座標0未満はすべて障害物.
        if(y ==  0) return  (((x <= 20) || (x >= 40))? 1 : 0);                                // 水平座標20と40に障害物があるのは
        if(y <  50) return  (((x == 20) || (x == 40))? 1 : 0);                                // 垂直座標50まで.
        if(y == 50) return ((((x >= 20) && (x <= 40)) || ((x >= 60) && (x <= 80))) ? 1 : 0);  // 垂直座標50からは,
        if(y < 100) return  (((x == 60) || (x == 80))? 1 : 0);                                // 水平座標60と80に障害物があるのは
        if(y ==100) return  (((x <= 60) || (x >= 80))? 1 : 0);                                // 垂直座標100まで.
    }                                                                                         // 水平座標0と100の間以外は
    return 1;                                                                                 // すべて障害物.
}                                                                                             // 関数終了.
*/

int spc_ex2(double x, double y){
    if(x>0 && x<100){
        if(y < 0  ) return 1;
        if(y == 0 ) return (( x<=80 || x>=90 ) ? 1 : 0);
        if(y < 10 ) return (( x==10 || x==70 || x==80 || x==90) ? 1 : 0);
        if(y == 10) return (( x==10 || (x>=20&&x<=30) || (x>=40&&x<=60) || x==70 || x==80 || x==90) ? 1 : 0); 
        if(y < 20 ) return (( x==10 || x==20 || x==30 || x==40 || x==70 || x==80 || x==90) ? 1 : 0);
        if(y == 20) return (( x==10 || x==20 || x==30 || x==40 || (x>=50&&x<=70) || (x>=80&&x<=90)) ? 1 : 0);
        if(y < 30 ) return (( x==10 || x==20 || x==30 || x==40) ? 1 : 0);
        if(y == 30) return (( x==10 || x==20 || x==30 || (x>=40&&x<=80) || x>=90 ) ? 1 : 0);
        if(y < 40 ) return (( x==20 || x==30 || x==40 || x==80 || x==90) ? 1 : 0);
        if(y == 40) return (( x<=20 || x==30 || x==40 || (x>=50&&x<=80) || x>=90) ? 1 : 0);       
        if(y < 50 ) return (( x==40) ? 1 : 0);
        if(y == 50) return (( (x>=10&&x<=50) || (x>=60&&x<=90)) ? 1 : 0);
        if(y < 60 ) return (( x==10 || x==50 || x==60 || x==90) ? 1 : 0);
        if(y == 60) return (( x==10 || (x>=20&&x<=50) || x==60 || (x>=70&&x<=80) || x>=90) ? 1 : 0);
        if(y < 70 ) return (( x==50 || x==70 || x==80) ? 1 :0);
        if(y == 70) return (( x<=40 || (x>=50&&x<=70) || (x>=80&&x<=90)) ? 1 : 0);
        if(y < 80 ) return (( x==10 || x==40 || x==90) ? 1 : 0);
        if(y == 80) return (( x==10 || (x>=20&&x<=30) || x==40 || (x>=50&&x<=90)) ? 1 : 0);
        if(y < 90 ) return (( x==10 || x==20 || x==30 || x==50 || x==90) ? 1 :0);
        if(y == 90) return (( x==10 || x==20 || (x>=30&&x<=90)) ? 1 : 0);
        if(y < 100) return 0;
        if(y ==100) return 1;
        if(y > 100) return 1;
    }
    return 1;
}

void spcmap(std::string file, int (*spc)(double, double)) {
    int x, y, t;
    std::ofstream ofp(file.c_str());
    for(x = 0; x <= SIZE; x++) {
        for(y = 0; y <= SIZE; y++){
           t = spc(x, y);
           ofp << t << " ";
        }
        ofp << '\n';
    }
    ofp.close();
}

void count_xy(std::string file, int *x, int *y){
    std::string str;
    int tmp;
    std::ifstream ifile(file.c_str());
    while(std::getline(ifile, str)){
        std::istringstream iss(str);
        while(iss >> tmp){
            *x += 1;
        }
        *y += 1;
    }
    *x = (*x) / (*y);
    ifile.close();
}

