#include<iostream>

struct drone{
    int x,y;        //�h���[���̌��ݒn�̍��W
    map[][];        //�}�b�v�����i�[
    fil[5][5];      //���[�U�[�����W�t�@�C���_�[�����肵�������i�[ 
    int nextx,nexty    //���ɐi�ޗ\��̍��W(���ݒn����݂Ă����i�ނ��B�Ƃ肠����nextx��nexty�̂ǂ��炩��1,-1������Ƒz��)
    private:
        Node[]          //�ʂ�m�[�h���i�[ 
        extract()       //�n�}����[�_�𒊏o����֐�
        Dijkstra()      //Dijkstra�ɂ��o�H���r������֐�
        avoidance()  //�ً}���p�v���O����
}

void filin(drone drone){//fil�ɏ����i�[�B0�͎��R��ԁA1�͏�Q��
	
	fil[2][2]=0;//���ݒn�̂��ߎ��R��ԁB
	
}

//�o�H��ɏ�Q��������Ή��̌o�H�쐬����֐��B
void avoidance(std::vector<int> d,drone drone){ //d...�n�}��ł̍ŒZ�o�H  drone...drone�\���́u�h���[���v
	
	int flag=0;//��Q������ň�ꂽ���Ƃ܂��o�H��ɖ߂��Ă�����1�ɂȂ�
	while(flag==0);{
		if(drone.nextx!=0){
			drone.nextx=0;
			drone.nexty=1;
		}else if(drone.nexty!=0){
			drone.nextx=1;
			drone.nexty=0;
		}
		
		drone.x = drone.nextx;
		drone.y = drone.nexty;
		
		//if(drone.x == �o�H�� && drone.y == �o�H��){ //���̔���@�ǂ����悤
			flag=1;
		}
	}
	
}
//���݂̍��W���ӂ�T��o�H��ɏ�Q���������avoidance���Ăяo���Bdrone������W�i�ނ��Ƃɂ�����g���z��
void dronego(std::vector<int> d,drone drone){ //d...�n�}��ł̍ŒZ�o�H  drone...drone�\���́u�h���[���v
	
	filin(drone); //����̏󋵂��擾
	
	if(drone.fil[2+drone.nextx][2+drone.nexty]==1){//�i�����Ƃ��Ă���W����Q���łӂ������Ă���
		
		avoidance(d,drone);//��Q�����
		
	}else{
		drone.x = drone.nextx;
		drone.y = drone.nexty;
		//drone.nextx = ??  nextx��nexty���ǂ�����Č��߂�̂��킩��Ȃ�
		//drone.nexty = ??
	}

}