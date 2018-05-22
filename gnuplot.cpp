#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>



bool check(int map[][10],int i,int j,int x,int y);

int main(){
    int x,y;
    std::cout << "xのサイズとyのサイズを入力してください(一行で入力してください）\n";
    std::cin >> x >> y;
    FILE *fp = popen("gnuplot","w");        //gnuplotを開く
    if(fp==NULL){
        return -1;
    }
    std::string str;
    int map[x][10];                          //グラフの状態を格納する配列
    int tmp;
    std::string file = "graph.dat";         //参照するファイル
    std::ifstream ifile(file.c_str());
    for(int i=0;i<y;i++){                   //配列に格納する
        std::getline(ifile,str);
        std::istringstream iss(str);
        int j=0;
        while(iss >> tmp){
            map[j][i] = tmp;
            j++;
        }
    }

    ifile.close();
    std::string ofile = "output.dat";       //出力するデータをファイルに書き込む
    std::ofstream ofp(ofile.c_str());
    for(int i=0;i<y;i++){
        for(int j=0;j<x;j++){
            if(check(map,i,j,x,y)){
                ofp << j << " " << i << "\n";
            }
        }
    }
    ofp.close();

    fputs("plot 'output.dat'\n",fp);        //gnuplot上の操作


    fflush(fp);
    std::cin.get();
    std::cin.get();
    pclose(fp);
    return 0;
}

bool check(int map[][10],int i,int j,int x,int y){
    if(i==0 || i==y-1|| j==0 || j==x-1){
        return true;
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