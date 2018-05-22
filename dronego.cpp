struct drone{
    int x,y,z;        //�h���[���̌��ݒn�̍��W
	int flag=0;
    bool fil[5][5];      //���[�U�[�����W�t�@�C���_�[�����肵�������i�[ 
    void avoidance();     //�ً}���p�v���O����
    void extract();       //�n�}����[�_�𒊏o����֐�
    void Dijkstra();      //Dijkstra�ɂ��o�H�v�Z������֐�
    void get_map();     //�}�b�v���̎擾
    void update_fil();  //fil���X�V����
    Node node[];   //�m�[�h
    private:
        int map[][][];        //�}�b�v�����i�[

}

//�[�_����[�_�܂Ńh���[���̌��ݒn���X�V���邲�Ƃ�avoidance���Ăяo���i��
void dronego(drone D,int nextnode){ // D...�h���[��  nextnode...�ڕW�_�ƂȂ�m�[�h�̔ԍ�
	
	int movex,movey;  //���ݒn����[�_�ɐi�ނ��߂Ɉړ����Ȃ��Ⴂ���Ȃ�x���W�̐���y���W�̐�
	movex = D.node[nextnode].x - D.x;  // movex���Z�o
	movey = D.node[nextnode].y - D.y;  // movey���Z�o
	
	double slope;  //������[�_�̍��W���玟�������[�_�̍��W�ւ̌X��
	slope=(double)(movex)/(double)(movey);  //slope���Z�o
	
	int move[movex+movey];  //�[�_����[�_�ɐi�ނ��߂�x���W��y���W����������,�ǂ̏��Ԃő���������̂����i�[.y��1��������Ƃ�(�����ŕ\���ƑO)��1,y��1����(���)��2,x��1����(�E)��3,x��1����(��)��4�Ƃ��đΉ�
	
	/* 
		move�̗�:�E��1,�O��1�Â��W(0,0)����(5,5)�܂Ői�݂����Ƃ�
		move[0]=3,move[1]=1,move[2]=3,move[3]=1,move[4]=3,move[5]=1,move[6]=3,move[7]=1,move[8]=3,move[9]=1;
	*/
	
	// ��move�̊i�[���@�ā�
	int ycount=0,count=0;  //ycount...y�����������񐔂��J�E���g�B  count...x�܂���y�����������񐔁B�ŏI�I��movex+movey�ɂȂ�͂�
	for(int x=1;x<=fabs(movex);x++){  //x���W�����Â����A���̓x��y���W�����������邩����
		//x���W�𑝌�
		if(movex>0){
			move[count]=3;
			count++;
		}else if(movex<0){
			move[count]=4;
			count++;
		}
		//y���W�𑝌�
		while(slope*x>=ycount){  //�ex�ɑ΂���slope*x��1�𒴂����^�C�~���O��y��1�񑝌��A2�𒴂����^�C�~���O��y������1�񑝌����������B3�𒴂����炳��ɂ������y��...(��)
			if(movey>0){
				move[count]=1;
				count++;
			}else if(movey<0){
				move[count]=2;
				count++;
			}
			ycount++;  //y�����������񐔂𑝂₷
		}
	}
	
	//�h���[���̈ʒu���X�V���Ă���
	for(int i=0;i<movex+movey;i++){
		
		avoidance(D);  //�i�����Ƃ��Ă���W����Q���łӂ������Ă����Q������B��Q��������N�������ꍇD.flag==1�ɂȂ��Ă�
		
		if(D.flag==0){  //��Q��������N����Ȃ������ꍇ
			if(move[i]==1){
				D.y+=1;  //�O��1�i��
			}else if(move[i]==2){
				D.y+=-1;  //����1�i��
			}else if(move[i]==3){
				D.x+=1;  //�E��1�i��
			}else if(move[i]==4){
				D.x+=-1;  //����1�i��
			}
		}else if(D.flag==1){  //��Q��������N�������ꍇ
			break;
		}
		
	}
	
	if(D.flag==1){  //��Q��������N�������ꍇ
		D.flag=0;  //�ċA���ł͏�Q������͋N���ĂȂ�����0�ɖ߂�
		dronego(D,nextnode);  //�ċA���g���ĉ����̒n�_����ړI�n�̒[�_�ɐi��
	}
	
}
