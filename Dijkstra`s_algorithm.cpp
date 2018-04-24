#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#define MAX 9999


struct Node{//ノードを定める構造体
    std::vector<int> edge_to;//このノードの接続先
    std::vector<int> edge_cost;//このノードにかかわる辺のコスト
    int edge_from=0;//このノードの最短接続元
    int cost_min=MAX;   //このノードへの最小コスト
    bool decided=false;//最短経路が確定しているかどうか
    bool used=false;//このノードが使用中か判定
    double x=0;//ノードのx座標
    double y=0;//ノードのy座標

};

void Node_print(struct Node *node,int node_num_max){
    for(int i=1;i<=node_num_max;i++){
    std::cout<<"node["<<i<<"]\n";
    for(int j=0;j<node[i].edge_to.size();j++){
        std::cout<<"edge_to["<<j<<"]="<<node[i].edge_to[0]<<'\n';
    }
    for(int j=0;j<node[i].edge_to.size();j++){
        std::cout<<"edge_cost["<<j<<"]="<<node[i].edge_cost[0]<<'\n';
    }
    std::cout<<"edge_from="<<node[i].edge_from<<'\n';
    std::cout<<"cost_min="<<node[i].cost_min<<'\n';
    std::cout<<"x="<<node[i].x<<'\n';
    std::cout<<"y="<<node[i].y<<'\n';
    std::cout<<"decided is ";
    if(node[i].decided==true){std::cout<<"True\n";}
    else{std::cout<<"False\n";}
    std::cout<<"used is ";
    if(node[i].used==true){std::cout<<"True\n";}
    else{std::cout<<"False\n";}
    }
};

int Node_in(struct Node *node){//ノードに値を入力する関数
    int node_num,node_to,node_cost,node_x,node_y;
    std::string name;
    std::cout<<"datファイルの名前を入力:";
    std::cin>>name;//ノードに関するデータを規定するdatを指定する
    std::ifstream file(name);//指定したdatファイルを開く
    std::string str;
    while(getline(file,str)){
        node_num=std::stoi(&str[0]);
        node_to=std::stoi(&str[2]);
        node_cost=std::stoi(&str[4]);
        node_x=std::stoi(&str[6]);
        node_y=std::stoi(&str[8]);
        node[node_num].used=true;
        node[node_num].edge_to.push_back(node_to);
        node[node_num].edge_cost.push_back(node_cost);
        node[node_num].x=node_x;
        node[node_num].y=node_y;
        } 
    file.close();
    return node_num;
};

void Node_update_min(struct Node *node,int node_num_max){//未確定ノードの最短距離を更新する
    Node_print(node,node_num_max);    
    int i=1;
    while(node[i].used){
        std::cout<<"i="<<i<<'\n';
        i++;
    }
    std::cout<<"完了"<<'\n';
};

int main(void){
    struct Node node[20];//点の配列
    int node_num_max;
    node[1].cost_min=0;//始点のコストを0とする
    node_num_max=Node_in(node);
    Node_print(node,node_num_max);
    Node_update_min(node,node_num_max);
    return 0;
}