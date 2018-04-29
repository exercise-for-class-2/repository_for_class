#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

bool check(int i,int j,int x,int y,int map[][10]){
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


int main(){
//x,yの値をコマンドプロンプト上で入力せずにgraph.datの中身を見て自動的にx,yに代入する
    std::string str1;
    int tmp1;
    int cnt_line = 0;
    int x, y;
    std::string file1 = "graph.dat";
    std::ifstream ifile1(file1.c_str());
    while(std::getline(ifile1,str1)){
        int cnt_row = 0;
        std::istringstream iss1(str1);
        while(iss1 >> tmp1){
            cnt_row += 1;
        }
        x = cnt_row;
        cnt_line += 1;
    }
    y = cnt_line;               

    ifile1.close();
    //x,yにgraph.datの列数,行数が代入されたよ

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

    std::string ofile1 = "output1.dat";       //出力するデータをファイルに書き込む
    std::ofstream ofp1(ofile1.c_str());
    for(int i=0;i<y;i++){
        for(int j=0;j<x;j++){
            if(map[j][i]==1){
                ofp1 << j << " " << i << "\n";
            }
        }
    }
    ofp1.close();

    ifile.close();
    std::string ofile2 = "output2.dat";       //出力するデータをファイルに書き込む
    std::ofstream ofp2(ofile2.c_str());
    for(int i=0;i<y;i++){
        for(int j=0;j<x;j++){
            if(check(i,j,x,y,map)){
                ofp2 << j << " " << i << "\n";
            }
        }
    }
    ofp2.close();

    fputs("set terminal png\n",fp);                     
    fputs("set output 'path.png'\n",fp);
    fputs("set style line 1 pointtype 7\n",fp);
    fputs("plot 'output1.dat' linestyle 1\n",fp);

    fputs("set output 'endpoints.png'\n",fp);
    fputs("set style line 1 pointtype 7\n",fp);
    fputs("plot 'output2.dat' linestyle 1\n",fp);

    fflush(fp);
    pclose(fp);

    std::cout << "Plese check png_files(path.png & enspoints.png)\n";
    std::cout << "created by this program\n";

    return 0;

}