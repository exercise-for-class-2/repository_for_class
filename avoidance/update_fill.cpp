#include <iostream>

void update_fil(int **nmap, bool dflag){       //nmap: 障害物情報込みのマップ
    Drone d;
    if(!dflag){
        for(int i=-2;i<3;i++){
            for(int j=-2;j<3;j++){
                if(nmap[/*今いる階層の値*/][x+i][y+i]!=0){
                    fil[i+2][j+2] = true;
                }else{
                    fil[i+2][j+2] = false;
                }
            }
        }
    }else{
        for(int i=0;i<5;i++){
            for(int j=0;j<5j++){
                if(nmap[x+i-2][y+i-2]!=0){
                    fil[4-i][4-j] = true;
                }else{
                    fil[4-i][4-j] = false;
                }
            }
        }
    }
}

//end!