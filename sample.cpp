#include <iostream>
#include <sstream>
#include <fstream>

#define X 100   //地図の横の大きさ make_map.cppで計算されたxの値をあらかじめ定義しておく
#define Y 100   //地図の縦の大きさ make_map.cppで計算されたyの値をあらかじめ定義しておく

void input_map(std::string file, int map[][Y], int x, int y);

int main() {

    int x = X, y = Y;
    std::string file1 = "map2.dat";

    int map[X][Y];
    input_map(file1, map, x, y);
    /*  
    for(int i=0;i<X;i++){
        for(int j=0;j<50;j++){
            std::cout << map[i][j] << " ";
        }
        std::cout << "\n";
    }*/
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
            std::cout<<tmp<<"\n";
            j++;
        }
    }
    ifile.close();
}