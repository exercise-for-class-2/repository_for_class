#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#define MAX 9999


struct Node{//�m�[�h���߂�\����
    std::vector<int> edge_to;//���̃m�[�h�̐ڑ���
    std::vector<int> edge_cost;//���̃m�[�h�ɂ������ӂ̃R�X�g
    int edge_from=0;//���̃m�[�h�̍ŒZ�ڑ���
    int cost_min=MAX;   //���̃m�[�h�ւ̍ŏ��R�X�g
    bool decided=false;//�ŒZ�o�H���m�肵�Ă��邩�ǂ���
    bool used=false;//���̃m�[�h���g�p��������
    double x=0;//�m�[�h��x���W
    double y=0;//�m�[�h��y���W

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

int Node_in(struct Node *node){//�m�[�h�ɒl����͂���֐�
    int node_num,node_to,node_cost,node_x,node_y;
    std::string name;
    std::cout<<"dat�t�@�C���̖��O�����:";
    std::cin>>name;//�m�[�h�Ɋւ���f�[�^���K�肷��dat���w�肷��
    std::ifstream file(name);//�w�肵��dat�t�@�C�����J��
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

void Node_update_min(struct Node *node,int node_num_max){//���m��m�[�h�̍ŒZ�������X�V����
    Node_print(node,node_num_max);    
    int i=1;
    while(node[i].used){
        std::cout<<"i="<<i<<'\n';
        i++;
    }
    std::cout<<"����"<<'\n';
};

int main(void){
    struct Node node[20];//�_�̔z��
    int node_num_max;
    node[1].cost_min=0;//�n�_�̃R�X�g��0�Ƃ���
    node_num_max=Node_in(node);
    Node_print(node,node_num_max);
    Node_update_min(node,node_num_max);
    return 0;
}