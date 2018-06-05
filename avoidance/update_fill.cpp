#include <iostream>

void update_fil(int ***nmap){       //nmap: 障害物情報込みのマップ
    Drone d;
    for(int i=-2;i<3;i++){
        for(int j=-2;j<3;j++){
            if(nmap[/*今いる階層の値*/][x+i][y+i]!=0){
                fil[i+2][j+2] = true;
            }else{
                fil[i+2][j+2] = false;
            }
        }
    }
}

//end!