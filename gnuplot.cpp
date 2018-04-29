#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


int main(){
    FILE *fp = popen("gnuplot","w");
    if(fp==NULL){
        return -1;
    }
    std::string str;
    int map[30][10];
    int tmp;
    std::string file = "graph.dat";
    std::ifstream ifile(file.c_str());
    for(int i=0;i<10;i++){
        std::getline(ifile,str);
        std::istringstream iss(str);
        int j=0;
        while(iss >> tmp){
            map[j][i] = tmp;
            j++;
        }
    }
    ifile.close();
    std::string ofile = "output.dat";
    std::ofstream ofp(ofile.c_str());
    for(int i=0;i<10;i++){
        for(int j=0;j<30;j++){
            if(map[j][i]==1){
                ofp << j << " " << i << "\n";
            }
        }
    }
    ofp.close();

    fputs("set mouse\n",fp);
    fputs("plot 'output.dat'\n",fp);


    fflush(fp);
    std::cin.get();
    pclose(fp);
    return 0;
}