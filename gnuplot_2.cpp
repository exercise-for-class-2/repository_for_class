#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

int main(){

//x,yの値をコマンドプロンプト上で入力せずにgraph.datの中身を見て自動的にx,yに代入する
    std::string str1;
    int tmp1;
    int cnt_line = 0;    //graph.datの行数, つまりyの値を数えるための引数
    int x, y;
    std::string file1 = "graph.dat";         //参照するファイル
    std::ifstream ifile1(file1.c_str());
    while(std::getline(ifile1,str1)){
        int cnt_row = 0;    //graph.datの列数, つまりxの値を数えるための引数
        std::istringstream iss1(str1);
        while(iss1 >> tmp1){    //数字を読み込むごとに
            cnt_row += 1;       //cnt_rowを1増やす
        }
        x = cnt_row;            //xが確定(何回も同じ値を代入してしまうけどそこはご愛敬)
        cnt_line += 1;          //graph.datの1行の読み込みが終わるとcnt_lineを1増やす
    }
    y = cnt_line;               //yが確定

    ifile1.close();
    //x,yにgraph.datの列数,行数が代入されたよ

    FILE *fp = popen("gnuplot","w");        //gnuplotを開く
    if(fp==NULL){
        return -1;
    }
    std::string str2;
    int map[x][y];                          //グラフの状態を格納する配列
    int tmp2;
    std::string file2 = "graph.dat";         //参照するファイル
    std::ifstream ifile2(file2.c_str());
    for(int i=0;i<y;i++){                   //配列に格納する
        std::getline(ifile2,str2);
        std::istringstream iss2(str2);
        int j=0;
        while(iss2 >> tmp2){
            map[j][i] = tmp2;
            j++;
        }
    }

    ifile2.close();
    std::string ofile = "output.dat";       //出力するデータをファイルに書き込む
    std::ofstream ofp(ofile.c_str());
    for(int i=0;i<y;i++){
        for(int j=0;j<x;j++){
            if(map[j][i]==1){
                ofp << j << " " << i << "\n";
            }
        }
    }
    ofp.close();

    fputs("set terminal png\n",fp);                     
    fputs("set output 'path.png'\n",fp);                //出力した画像をpath.pngに保存する
    fputs("set style line 1 pointtype 7\n",fp);
    fputs("plot 'output.dat' linestyle 1\n",fp);        //gnuplot上の操作

    fflush(fp);
    std::cin.get();
    pclose(fp);

    return 0;
}