#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>

#define GNPLT "C:/PROGRA~1/gnuplot/bin/gnuplot.exe"  
#define STRLN 100                                    
#define SIZE  100                                    
#define X 101
#define Y 101


int spc_ex1(double x, double y);
int spc_ex2(double x, double y);

void spcmap(std::string file, int (*spc)(double, double));

void count_xy(std::string file, int *x, int *y);

int main() {
    int x=0, y=0;

    std::string file1 = "map1.dat";
    spcmap(file1, spc_ex1);              
    count_xy(file1, &x, &y);            

    std::string file2 = "map2.dat";
    spcmap(file2, spc_ex2);
    count_xy(file2, &x, &y);
    std::cout << "x:" << x << " y:" << y << '\n';  
/*
    std::string file3 = "map3.dat";
    spcmap(file3, spc_ex3);
    count_xy(file3, &x, &y);
    
    std::string file4 = "map4.dat";
    spcmap(file4, spc_ex4);
    count_xy(file4, &x, &y);
*/
return 0;
}


int spc_ex1(double x, double y) {                                                             
    if((x > 0) && (x < 100)) {                                                                
        if(y <   0) return 1;                                                                 
        if(y ==  0) return  (((x <= 20) || (x >= 40))? 1 : 0);                               
        if(y <  50) return  (((x == 20) || (x == 40))? 1 : 0);                                
        if(y == 50) return ((((x >= 20) && (x <= 40)) || ((x >= 60) && (x <= 80))) ? 1 : 0);  
        if(y < 100) return  (((x == 60) || (x == 80))? 1 : 0);                                
        if(y ==100) return  (((x <= 60) || (x >= 80))? 1 : 0);                               
    }                                                                                       
    return 1;                                                                           
} 


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
        if(y == 90) return (( x==10 || x==20 || x>=30) ? 1 : 0);
        if(y < 100) return 0;
        if(y ==100) return 1;
        if(y > 100) return 1;
    }
    return 1;
}

void spcmap(std::string file, int (*spc)(double, double)) {
    int x, y, t;
    std::ofstream ofp(file.c_str());
    for(y = 0; y <= SIZE; y++) {
        for(x = 0; x <= SIZE; x++){
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

