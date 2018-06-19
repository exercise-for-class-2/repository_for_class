#include <Siv3D.hpp>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <cstdlib>

#define STRLN 100       //char型配列のサイズ
#define X 101           //mapの横幅
#define Y 101           //mapの縦幅
#define Z 2             //階層数
#define MAX_COST 10000
#define MAX_NODE 10000
#define MAX 10000
#define MAX_Z 100       //階層移動可能な回数

int S_X = 5, S_Y = 5, S_Z = 0;  //startの座標
int G_X = 85, G_Y = 85;         //goalの座標

								//初期化のための, テンプレートを用いた関数
template<typename A, size_t N, typename T>
void FILL(A(&array)[N], const T &val) {
	std::fill((T*)array, (T*)(array + N), val);
}

struct Node {                        //このノードから伸びるエッジの情報
	std::vector<int> edges_to;       //各エッジの接続先のノード番号
	std::vector<double> edges_cost;  //各エッジのコスト
	int x;                           //x座標
	int y;                           //y座標
	int path = -1;                   //このノードに入ってくるノード
	double cost = MAX_COST;          //このノードへの現時点で判明している最小コスト
	bool flag = false;               //探索済みか否か
	bool done = false;               //確定ノードか否か
};

struct Drone {
	int x = S_X, y = S_Y, z = S_Z;    //ドローンの現在地の座標
	bool fil[5][5];             //レーザーレンジファインダーより入手した情報を格納 
	bool flag;
	std::vector<int> shortest_route[MAX_Z]; //最短経路
	int i = 0;
	int map[Z][X][Y];           //マップ情報を格納
	int map_buffer[Z][X][Y];    //仮想壁考慮のマップ
	int nextnode = -1;          //次に向かうノード
	int start[100];             //start node
	int goal[100];              //goal node

	void avoidance();           //緊急回避用プログラム
	void Dijkstra();            //Dijkstraによる経路計算をする関数
	void get_map();             //マップ情報の取得
	void update_fil();          //filを更新し、ドローン周辺の障害物情報を格納
};

std::vector<struct Point> route[MAX_Z]; //droneの軌跡
Node node[MAX_Z][MAX_NODE];				//node
Drone d;								//drone
int map_flag[Z][X][Y];					//過去にgoalに設定されたか否か
std::vector<int> map_flag_i[Z];			//過去にgoalに設定されたノードを保存


void start_drone();                                                         //すべてのプログラムを起動する
double calc_distance(double x, double y);                                   //x*x + y*yを返す
void input_edges_cost(int n);                                               //node間の距離をnode[]に代入
void Node_in(std::string file);                                             //各ノードの座標を代入 file = xy_edges0x.dat
void search_node(int n, int start);                                         //各ノードまでのコストを計算
int  search_confirm_node(int n, int *cnt);                                  //各ノードまでのコストを計算
void make_node(std::string file, int map[][Y], int n);                      //node0x.datの各ノードとその向こう先を書き込む関数
int number_of_node(std::string file);                                       //file = "xy_edges0x.dat"から, ノードの個数を返す
bool check_wall(int map[][Y], int i_s, int i_g);                            //ノード間の壁の判定
bool check_wall_last(int map[][Y], int s_x, int s_y, int g_x, int g_y);     //check_wall()内で使う関数
void make_dijkstra(int start, int goal);									//dijkstra経路を図示するためのファイル書き込み
void set_goal(int n, int *goal);											//goalのノードを保存
void set_start(int n, int *strat, int *goal);                               //startを決める. 一番初めはgoalも決める
bool check(int k, int i, int j, int map[][X][Y]);                           //端点を判定する関数. 端点ならばtrueを返す
void output_dat(char *file_all, char *file_edges, int map[][X][Y], int k);  //gnuplotで地図を描画するためのfileを作成する
void change_map(int map[][X][Y], int map_buffer[][X][Y]);                   //仮想壁を作る
void output_map(int map_buffer[][X][Y]);                                    //map_bufferを見るため
void initialize_map_buffer(int map_buffer[][X][Y]);                         //map_bufferの初期化
void dronego();                                                             //端点から端点までドローンの現在地を更新するごとにavoidanceを呼び出しつつ進む
void make_dat(int i);                                                       //mapから必要な点をファイルへ書き込む
bool chk_wall(int map[][Y], int i, int j);                                   //d.avoidance()内で用いる壁の判定
void input_map(std::string file, int map[][Y]);								//map情報をmap[X][Y]に格納
int set_next(int *z);                                                       //次の階層を決める

																			/*-------------------------------------main文-----------------------------------------*/
void Main() {
	start_drone();
	Window::SetStyle(WindowStyle::Sizeable);
	Camera camera;
	int map[Z][X][Y];
	for (int k = 0; k < Z; k++) {
		std::stringstream file_map;
		file_map << "map" << std::setw(2) << std::setfill('0') << k << ".dat";
		input_map(file_map.str(), map[k]);
	}
	int i_d = 0;
	int k_d = 0;
	const Font font(30);
	while (System::Update()) {
		for (int k = 0; k < Z; k++) {
			for (int i = 0; i < X; i++) {
				for (int j = 0; j < Y; j++) {
					if (map[k][i][j] != 0) {
						if (k == 0) { Box(Vec3(i, k * 10, j), 1).draw(Palette::Cyan); }
						else { Box(Vec3(i, k * 10, j), 1).draw(Palette::Red); }
					}
				}
			}
		}
		if ((int)(route[k_d].size()) == 0) {
			i_d = 0;
			k_d++;
		}
		else {
			camera.pos = (Vec3(route[k_d][i_d].x + 1, 50, route[k_d][i_d].y + 1));
			camera.lookat = Vec3(route[k_d][i_d].x, 0, route[k_d][i_d].y);
			Graphics3D::SetCamera(camera);
			if (k_d%Z == 0) { Box(Vec3(route[k_d][i_d].x, (k_d%Z) * 10, route[k_d][i_d].y), 1).draw(Palette::Cyan); }
			else { Box(Vec3(route[k_d][i_d].x, (k_d%Z) * 10, route[k_d][i_d].y), 1).draw(Palette::Red); }
			Box(Vec3(G_X, 0, G_Y), 3).draw(Palette::Aqua);
			//font(L"({},{})"_fmt, route[k_d][i_d][0], route[k_d][i_d][1]).draw();
			font(L"(").draw();  font(route[k_d][i_d].x).draw(31,0);
			font(L", ").draw(80,0); font(route[k_d][i_d].y).draw(102,0);
			font(L")\n").draw(160,0);
			if (i_d == (int)(route[k_d].size()) - 1) {
				i_d = 0;
				k_d++;
				if (k_d == d.i) {
					k_d = 0;
				}
			}
			else {
				i_d++;
			}
			System::Sleep(0.05s);
		}
	}
}
/*-----------------------------------------------------------------------------------*/

/*-------------------start_drone()------------------------*/
void start_drone() {
	//初期化
	FILL(d.start, -1);
	FILL(d.goal, -1);
	FILL(map_flag, 0);
	FILL(map_flag_i, 0);

	d.get_map();
	d.Dijkstra();
	d.i = 0;
	d.x = S_X;
	d.y = S_Y;
	d.z = S_Z;

	while (d.x != G_X || d.y != G_Y) {
		int i = 1;                  //d.shortest_routeのi番目のnode
		d.nextnode = d.shortest_route[d.i][i];
		while (d.nextnode != -1) {  //goalに着くまで繰り返す(goalに着いたらnextnode=-1にする)
			dronego();
			if (d.nextnode != d.goal[d.i]) {
				i++;
				d.nextnode = d.shortest_route[d.i][i];
			}
			else {
				d.nextnode = -1;
			}
		}
		if (Z - 1 == d.z) {
			d.z = 0;
		}
		else {
			d.z += 1;
		}
		d.i++;
		if (d.i == MAX_Z) { break; }
	}
}
/*--------------------------------------------------------*/


/*-------------------------------------Drone::get_map()関連の関数---------------------------------------------*/
void Drone::get_map() {

	for (int k = 0; k<Z; k++) {
		std::string str;
		int tmp;
		std::stringstream file_map;
		file_map << "map" << std::setw(2) << std::setfill('0') << k << ".dat";
		std::ifstream ifile(file_map.str().c_str());
		for (int ii = 0; ii<X; ii++) {
			std::getline(ifile, str);
			std::istringstream iss(str);
			int j = 0;
			while (iss >> tmp) {
				map[k][ii][j] = tmp;
				j++;
			}
		}
		map[k][S_X][S_Y] = 1;
		map[k][G_X][G_Y] = 1;
		ifile.close();
	}
}
/*-----------------------------------------------------------------------------------------------------------*/


/*---------------------------make_dat()関連の関数-----------------------------------------*/
void make_dat(int i) {
	initialize_map_buffer(d.map_buffer);
	change_map(d.map, d.map_buffer);
	output_map(d.map_buffer);
	char file_all[STRLN];
	char file_edges[STRLN];
	sprintf_s(file_all, "all%02d.dat", i);
	sprintf_s(file_edges, "edges%02d.dat", i);
	output_dat(file_all, file_edges, d.map_buffer, i);
}

void change_map(int map[][X][Y], int map_buffer[][X][Y]) {
	for (int k = 0; k<Z; k++) {
		for (int i = 1; i<X - 1; i++) {
			for (int j = 1; j<Y - 1; j++) {
				if ((i == S_X && j == S_Y) || (i == G_X && j == G_Y)) {
					map_buffer[k][i][j] = 0;
				}
				else {
					if (map[k][i][j] != 0) {
						if ((map[k][i][j + 1] != 0 && map[k][i - 1][j] != 0) ||
							(map[k][i - 1][j] != 0 && map[k][i][j - 1] != 0) ||
							(map[k][i][j - 1] != 0 && map[k][i + 1][j] != 0) ||
							(map[k][i + 1][j] != 0 && map[k][i][j + 1] != 0)) {
							map_buffer[k][i - 1][j - 1] = 1; map_buffer[k][i - 1][j] = 1; map_buffer[k][i - 1][j + 1] = 1;
							map_buffer[k][i][j - 1] = 1; map_buffer[k][i][j] = 1; map_buffer[k][i][j + 1] = 1;
							map_buffer[k][i + 1][j - 1] = 1; map_buffer[k][i + 1][j] = 1; map_buffer[k][i + 1][j + 1] = 1;
						}
						else if (map[k][i][j - 1] != 0 && map[k][i][j + 1] == 0) {
							map_buffer[k][i][j - 1] = 1;
						}
						else if (map[k][i][j - 1] == 0 && map[k][i][j + 1] != 0) {
							map_buffer[k][i][j + 1] = 1;
						}
						else {
							map_buffer[k][i][j - 1] = 1;
							map_buffer[k][i][j + 1] = 1;
						}
					}
				}
			}
		}
		for (int i = 1; i<X - 1; i++) {
			for (int j = 1; j<Y - 1; j++) {
				if ((i == S_X && j == S_Y) || (i == G_X && j == G_Y)) {
					map_buffer[k][i][j] = 0;
				}
				else {
					if (map[k][j][i] != 0) {
						if ((map[k][j][i + 1] != 0 && map[k][j - 1][i] != 0) ||
							(map[k][j - 1][i] != 0 && map[k][j][i - 1] != 0) ||
							(map[k][j][i - 1] != 0 && map[k][j + 1][i] != 0) ||
							(map[k][i + 1][j] != 0 && map[k][i][j + 1] != 0)) {
							map_buffer[k][j - 1][i - 1] = 1; map_buffer[k][j - 1][i] = 1; map_buffer[k][j - 1][i + 1] = 1;
							map_buffer[k][j][i - 1] = 1; map_buffer[k][j][i] = 1; map_buffer[k][j][i + 1] = 1;
							map_buffer[k][j + 1][i - 1] = 1; map_buffer[k][j + 1][i] = 1; map_buffer[k][j + 1][i + 1] = 1;
						}
						else if (map[k][j - 1][i] != 0 && map[k][j + 1][i] == 0) {
							map_buffer[k][j - 1][i] = 1;
						}
						else if (map[k][j - 1][i] == 0 && map[k][j + 1][i] != 0) {
							map_buffer[k][j + 1][i] = 1;
						}
						else {
							map_buffer[k][j - 1][i] = 1;
							map_buffer[k][j + 1][i] = 1;
						}
					}
				}
			}
		}
		for (int i = 0; i<X; i++) {
			map_buffer[k][i][0] = 1;
			map_buffer[k][0][i] = 1;
			map_buffer[k][100][i] = 1;
			map_buffer[k][i][100] = 1;
		}
	}
}

void initialize_map_buffer(int map_buffer[][X][Y]) {
	for (int k = 0; k<Z; k++) {
		for (int i = 0; i<X; i++) {
			for (int j = 0; j<Y; j++) {
				map_buffer[k][i][j] = 0;
			}
		}
	}
}

void output_map(int map_buffer[][X][Y]) {
	for (int k = 0; k<Z; k++) {
		std::stringstream file_map_buffer;
		file_map_buffer << "map_buffer" << std::setw(2) << std::setfill('0') << k << ".dat";
		std::ofstream ofp(file_map_buffer.str().c_str());
		for (int i = 0; i<X; i++) {
			for (int j = 0; j<Y; j++) {
				ofp << map_buffer[k][i][j] << " ";
			}
			ofp << '\n';
		}
		ofp.close();
	}
}

void output_dat(char *file_all, char *file_edges, int map[][X][Y], int k) {
	FILE *fp;
	fopen_s(&fp, file_all, "w");
	if (fp == NULL) {
		printf("ERROR\n");
		std::exit(1);
	}
	fprintf(fp, "%d %d\n", d.x, d.y);
	for (int i = 0; i<X; i++) {
		for (int j = 0; j<Y; j++) {
			if (map[k][i][j] != 0) {
				fprintf(fp, "%d %d\n", i, j);
			}
		}
	}
	fprintf(fp, "%d %d\n", G_X, G_Y);
	fclose(fp);

	//file_edges
	fopen_s(&fp, file_edges, "w");
	if (fp == NULL) {
		printf("ERROR\n");
		std::exit(1);
	}
	fprintf(fp, "%d %d\n", d.x, d.y);
	for (int i = 0; i<X; i++) {
		for (int j = 0; j<Y; j++) {
			if (check(k, i, j, map)) {
				fprintf(fp, "%d %d\n", i, j);
			}
		}
	}
	fprintf(fp, "%d %d\n", G_X, G_Y);
	fclose(fp);
}

bool check(int k, int i, int j, int map[][X][Y]) {
	if (i == 0 || i == X - 1 || j == 0 || j == Y - 1) {
		return false;
	}
	if (map[k][i - 1][j] != 0 && map[k][i + 1][j] != 0) {
		return false;
	}
	if (map[k][i][j - 1] != 0 && map[k][i][j + 1] != 0) {
		return false;
	}
	if (i != 0 || i != X - 1 || j != 0 || j != Y - 1) {
		if ((map[k][i][j + 1] == 0 && map[k][i - 1][j] == 0 && map[k][i][j - 1] != 0 && map[k][i + 1][j] != 0) ||
			(map[k][i][j + 1] != 0 && map[k][i - 1][j] == 0 && map[k][i][j - 1] == 0 && map[k][i + 1][j] != 0) ||
			(map[k][i][j + 1] != 0 && map[k][i - 1][j] != 0 && map[k][i][j - 1] == 0 && map[k][i + 1][j] == 0) ||
			(map[k][i][j + 1] == 0 && map[k][i - 1][j] != 0 && map[k][i][j - 1] != 0 && map[k][i + 1][j] == 0)) {
			return true;
		}
	}
	if (map[k][i][j] != 0) {
		return true;
	}
	return false;
}
/*----------------------------------------------------------------------------------------------------------*/


/*-----------------------------------Drone::Dijkstra()--------------------------------------------------------*/
void Drone::Dijkstra() {
	//アドレスの都合上再度行う処理. あまり気にしないで
	initialize_map_buffer(map_buffer);
	change_map(map, map_buffer);
	output_map(map_buffer);
	int z_now = 0;
	while (x != G_X || y != G_Y) {
		make_dat(z_now);
		std::stringstream file_map, file_all, file_edges, file_node, file_dijkstra;
		char gnufile_all[STRLN], gnufile_edges[STRLN], gnufile_dijkstra[STRLN];
		//fileの名前を自動生成
		std::cout << "z:" << z << '\n';
		file_map << "map" << std::setw(2) << std::setfill('0') << z_now << ".dat";
		file_all << "all" << std::setw(2) << std::setfill('0') << z_now << ".dat";
		file_edges << "edges" << std::setw(2) << std::setfill('0') << z_now << ".dat";
		file_node << "node" << std::setw(2) << std::setfill('0') << z_now << ".dat";
		file_dijkstra << "dijkstra" << std::setw(2) << std::setfill('0') << z_now << ".dat";
		sprintf_s(gnufile_all, "all%02d", z_now);
		sprintf_s(gnufile_edges, "edges%02d", z_now);
		sprintf_s(gnufile_dijkstra, "dijkstra%02d", z_now);

		//ここからDijkstra法スタート
		int n = number_of_node(file_edges.str());               //node(端点)の総数
		Node_in(file_edges.str());                              //端点をノードとしてnode[]に保存
		make_node(file_node.str(), map_buffer[z_now], n);       //壁を判定してノードが向かうことのできるノードを保存していく
		input_edges_cost(n);                                    //node間の距離を保存
		set_start(n, &start[i], &goal[i]);                      //startを決める. 一番初めはゴールも決める
		search_node(n, start[i]);                               //スタートから到達可能なすべてのノードへの最小コストと最短経路を保存
		set_goal(n, &goal[i]);									//現在いる階層でのスタートノードとゴールノードを決める.
		make_dijkstra(start[i], goal[i]);						//startからgoalまでの最短経路を保存
		x = node[i][goal[i]].x;                                 //d.xを更新
		y = node[i][goal[i]].y;                                 //d.yを更新
		i++;                                                    //d.iをインクリメント
		z_now = set_next(&z);                                   //z_nowを次の階層(z)に更新
	}
}

int number_of_node(std::string file) {
	int cnt = 0;
	int tmp;
	std::string str;
	std::ifstream ifile(file.c_str());
	while (std::getline(ifile, str)) {
		std::istringstream iss(str);
		while (iss >> tmp) {
			cnt += 1;
		}
	}
	return (cnt / 2);
}

void Node_in(std::string file) {
	int x, y;
	std::ifstream ifile(file.c_str());
	std::string str;
	int i = 0;
	while (std::getline(ifile, str)) {
		std::istringstream iss(str);
		iss >> x >> y;
		node[d.i][i].x = x;
		node[d.i][i].y = y;
		i += 1;
	}
	ifile.close();
}

void make_node(std::string file, int map[][Y], int n) {
	//node.datにノードを書き込む
	std::ofstream ofp(file.c_str());
	for (int i = 0; i<n; i++) {
		for (int j = 0; j<n; j++) {
			if (i != j) {
				if (check_wall(map, i, j)) {
					ofp << i << " " << j << '\n';
				}
			}
		}
	}
	ofp.close();

	//node.datを読み込んでnode[].edges_to[]に代入していく
	int node_num, edges_to;
	std::ifstream ifp(file.c_str());
	std::string str;
	while (std::getline(ifp, str)) {
		std::istringstream iss(str);
		iss >> node_num >> edges_to;
		node[d.i][node_num].edges_to.push_back(edges_to);
	}
	ifp.close();
}

double calc_distance(double x, double y) {
	return std::sqrt(x*x + y * y);
}

void input_edges_cost(int n) {
	for (int i = 0; i<n; i++) {
		int n_edges_to = node[d.i][i].edges_to.size();
		for (int j = 0; j<n_edges_to; j++) {
			node[d.i][i].edges_cost.push_back(calc_distance((node[d.i][i].x - node[d.i][node[d.i][i].edges_to[j]].x), (node[d.i][i].y - node[d.i][node[d.i][i].edges_to[j]].y)));
		}
	}
}

void set_start(int n, int *start, int *goal) {
	if ((*start) == -1 && (*goal) == -1) {
		*start = 0;
		*goal = n - 1;
	}
	else {
		*start = 0;
		*goal = n - 1;
	}
}

void search_node(int n, int start) {
	int cnt = 0;            //sになったらすべて探索済み
	int i = start;
	int j = 0;
	int n_edges_to = node[d.i][i].edges_to.size();
	node[d.i][i].cost = 0;       //startまでのcostは0
	node[d.i][i].path = start;
	node[d.i][i].flag = true;    //探索済み
	node[d.i][i].done = true;    //確定ノード
	cnt += 1;
	while (j<n_edges_to) {
		if (node[d.i][node[d.i][i].edges_to[j]].flag == false) {
			node[d.i][node[d.i][i].edges_to[j]].cost = node[d.i][i].edges_cost[j];
			node[d.i][node[d.i][i].edges_to[j]].flag = true;
			node[d.i][node[d.i][i].edges_to[j]].path = i;
			j += 1;
		}
	}
	i = search_confirm_node(n, &cnt);     //確定ノードを探す
	node[d.i][i].done = true;                        //node iを確定ノードにする
	cnt += 1;
	while (cnt < n) {//到達可能なすべてのノードが確定ノードになるまで繰り返す
		j = 0;
		n_edges_to = node[d.i][i].edges_to.size();	        //node[i].edges_toの要素数
		while (j<n_edges_to) {
			if (node[d.i][node[d.i][i].edges_to[j]].flag == false) {
				node[d.i][node[d.i][i].edges_to[j]].cost = node[d.i][i].cost + node[d.i][i].edges_cost[j];
				node[d.i][node[d.i][i].edges_to[j]].flag = true;
				node[d.i][node[d.i][i].edges_to[j]].path = i;
			}
			else if (node[d.i][i].cost + node[d.i][i].edges_cost[j] < node[d.i][node[d.i][i].edges_to[j]].cost) {
				node[d.i][node[d.i][i].edges_to[j]].cost = node[d.i][i].cost + node[d.i][i].edges_cost[j];
				node[d.i][node[d.i][i].edges_to[j]].path = i;
			}
			j += 1;
		}
		i = search_confirm_node(n, &cnt);     //確定ノードを探す
		if (i != -1) {
			node[d.i][i].done = true;                    //node iを確定ノードにする
			cnt += 1;
		}
	}
}

int search_confirm_node(int n, int *cnt) {
	int i = MAX_COST, j;
	double min = MAX_COST;
	for (j = 0; j<n; j++) {
		if ((node[d.i][j].flag == true) && (node[d.i][j].done == false)) {
			if (node[d.i][j].cost < min) {
				min = node[d.i][j].cost;
				i = j;
			}
		}
	}
	if (i == MAX_COST) {
		(*cnt) = n;
		return -1;
	}
	return i;
}

void set_goal(int n, int *goal) {
	if (node[d.i][*goal].path != -1) {//goalに到達可能なら
		*goal = n - 1;           //goalは最終的なゴール
	}
	else {//現在の階層のstartからでは到達不可能なら
		  //次に向かう階層に行くことのできる地点を探してそこをgoalとする
		int len = map_flag_i[d.z].size();
		for (int j = 0; j<len; j++) {
			map_flag[d.z][node[d.i][(*goal) - map_flag_i[d.z][j]].x][node[d.i][(*goal) - map_flag_i[d.z][j]].y] = 1;
		}
		int i = 1;
		while ((node[d.i][(*goal) - i].path == -1) || map_flag[d.z][node[d.i][(*goal) - i].x][node[d.i][(*goal) - i].y] == 1) {
			i++;
		}
		*goal = n - 1 - i;
		map_flag_i[d.z].push_back(i);
	}
}

void make_dijkstra(int start, int goal) {
	int i = goal;
	int j = 0;
	int tmp[MAX_NODE];
	while (i != start) {//goalからstartに戻るまで
		tmp[j] = i;
		j++;
		i = node[d.i][i].path;
	}
	tmp[j] = i;
	j++;

	for (int k = 0; k<j; k++) {
		d.shortest_route[d.i].push_back(tmp[j - k - 1]);
	}
}

bool check_wall(int map[][Y], int i_s, int i_g) {
	int s_x = node[d.i][i_s].x;
	int s_y = node[d.i][i_s].y;
	int g_x = node[d.i][i_g].x;
	int g_y = node[d.i][i_g].y;

	bool k = check_wall_last(map, s_x, s_y, g_x, g_y);
	return k;
}

bool check_wall_last(int map[][Y], int s_x, int s_y, int g_x, int g_y) {
	int d_x = g_x - s_x;       //端点同士のx座標の差difference_x
	int d_y = g_y - s_y;       //端点同士のy座標の差difference_y
	int slope1, slope2;
	int flag_d = 0;
	if (d_x == 0)  flag_d = 1;
	else          slope1 = d_y / d_x;  //2点間の傾きd_y / d_x
	if (d_y == 0)  flag_d = 2;
	else          slope2 = d_x / d_y;  //2点間の傾きd_x / d_y

	int sign_x, sign_y;
	if ((d_x == 0) || (d_y == 0)) {
		if (d_x == 0) {
			if (d_y>0) sign_y = 1;
			else      sign_y = -1;
		}
		if (d_y == 0) {
			if (d_x>0) sign_x = 1;
			else      sign_x = -1;
		}
	}
	else {
		if ((d_x>0) && (d_y>0)) { sign_x = 1; sign_y = 1; }
		if ((d_x<0) && (d_y>0)) { sign_x = -1; sign_y = 1; }
		if ((d_x<0) && (d_y<0)) { sign_x = -1; sign_y = -1; }
		if ((d_x>0) && (d_y<0)) { sign_x = 1; sign_y = -1; }
	}

	int flag = 0;
	int i_x, j_y;
	if (flag_d == 1) {//x座標が等しい
		int j;
		for (j = 1; j <= std::abs(d_y); j++) {
			j_y = j * sign_y;
			if (flag == 0) {
				if (map[s_x + 1][s_y + j_y] != 0 && map[s_x - 1][s_y + j_y] != 0)  flag = 3;
			}
			if (flag == 3) {
				return false;
			}
		}
		return true;
	}
	else if (flag_d == 2) {//y座標が等しい
		int i;
		for (i = 1; i <= std::abs(d_x); i++) {
			i_x = i * sign_x;
			if (flag == 0) {
				if (map[s_x + i_x][s_y + 1] != 0 && map[s_x + i_x][s_y - 1] != 0)  flag = 3;
			}
			if (flag == 3) {
				return false;
			}
		}
		return true;
	}
	else {
		if (std::abs(slope2) <= std::abs(slope1)) {
			int i = 1, j = 1;
			int flag_ij;
			for (i = 1; i<std::abs(d_x); i++) {
				int cnt = 0;
				i_x = i * sign_x;
				while (cnt != std::abs(slope1)) {
					flag_ij = 1;
					j_y = j * sign_y;
					if (flag_ij == 1 && flag == 0) {
						if (map[s_x + i_x][s_y + j_y] != 0) { flag_ij = 0; flag = 3; }
						else if (map[s_x + i_x][s_y + j_y] == 0) { flag_ij = 0; flag = 1; }
					}
					if (flag_ij == 1 && flag == 1) {
						if (map[s_x + i_x][s_y + j_y] != 0) { flag_ij = 0; flag = 2; }
					}
					if (flag_ij == 1 && flag == 2) {
						flag = 3;
					}
					if (flag == 3) {
						return false;
					}
					cnt += 1;
					j += 1;
				}
			}
			j_y = j * sign_y;
			i_x = i * sign_x;
			//ここまでくれば現在地のx座標は目的地のx座標と一致する(はず)
			while ((s_y + j_y) != g_y) {
				flag_ij = 1;
				flag = 0;
				if (flag == 0) {
					if (map[s_x + i_x + 1][s_y + j_y] != 0 && map[s_x + i_x - 1][s_y + j_y] != 0) { flag = 3; }
				}
				if (flag == 3) {
					return false;
				}
				j += 1;
				j_y = j * sign_y;
			}
			return true;
		}
		else {
			int i = 1, j = 1;
			int flag_ij;
			for (j = 1; j<std::abs(d_y); j++) {
				int cnt = 0;
				j_y = j * sign_y;
				while (cnt != std::abs(slope2)) {
					flag_ij = 1;
					i_x = i * sign_x;
					if (flag_ij == 1 && flag == 0) {
						if (map[s_x + i_x][s_y + j_y] != 0) { flag_ij = 0; flag = 3; }
						else if (map[s_x + i_x][s_y + j_y] == 0) { flag_ij = 0; flag = 1; }
					}
					if (flag_ij == 1 && flag == 1) {
						if (map[s_x + i_x][s_y + j_y] != 0) { flag_ij = 0; flag = 2; }
					}
					if (flag_ij == 1 && flag == 2) {
						flag = 3;
					}
					if (flag == 3) {
						return false;
					}
					cnt += 1;
					i += 1;
				}
			}
			i_x = i * sign_x;
			j_y = j * sign_y;
			//ここまでくれば現在地のy座標は目的地のy座標と一致する(はず)
			while ((s_x + i_x) != g_x) {
				flag_ij = 1;
				flag = 0;
				if (flag == 0) {
					if (map[s_x + i_x][s_y + j_y + 1] != 0 && map[s_x + i_x][s_y + j_y - 1] != 0) { flag = 3; }
				}
				if (flag == 3) {
					return false;
				}
				i += 1;
				i_x = i * sign_x;
			}
			return true;
		}
	}
}

int set_next(int *z) {
	if (Z - 1 == *z) {
		*z = 0;
	}
	else {
		*z += 1;
	}
	int i = *z;
	return i;
}
/*-------------------------------------------------------------------------------------------------*/


/*----------------------dronego()---------------------------------------------------------------------------*/
void dronego() {        //端点から端点までドローンの現在地を更新するごとにavoidanceを呼び出しつつ進む

	int movex, movey;   //現在地から端点に進むために移動しなきゃいけないx座標の数とy座標の数
	movex = node[d.i][d.nextnode].x - d.x;  // movexを算出
	movey = node[d.i][d.nextnode].y - d.y;  // moveyを算出
	int *move = new int[std::abs(movex) + std::abs(movey) + 1];   //端点から端点に進むためにx座標とy座標をいくつずつ,どの順番で増減させるのかを格納.yが1増加するとき(方向で表すと前)は1,yが1減少(後ろ)が2,xが1増加(右)が3,xが1減少(左)が4として対応

																  /*
																  moveの例:右に1,前に1づつ座標(0,0)から(5,5)まで進みたいとき
																  move[0]=3,move[1]=1,move[2]=3,move[3]=1,move[4]=3,move[5]=1,move[6]=3,move[7]=1,move[8]=3,move[9]=1;
																  */

																  // ↓moveの格納方法案↓
	int ycount = 0, count = 0;  //ycount...yが増減した回数をカウント。  count...xまたはyが増減した回数。最終的にmovex+moveyになるはず
	double slope;               //今いる端点の座標から次向かう端点の座標への傾き
	if (movex == 0) {
		for (int y = 1; y <= std::fabs(movey); y++) {
			if (movey > 0) {
				move[count] = 1;
				count++;
			}
			else if (movey < 0) {
				move[count] = 2;
				count++;
			}
		}
	}
	else {
		slope = (double)(movey) / (double)(movex);          //slopeを算出
		for (int x = 1; x <= std::fabs(movex); x++) {       //x座標を一回づつ増減、その度にy座標も増減させるか判定
															//x座標を増減
			if (movex > 0) {
				move[count] = 3;
				count++;
			}
			else if (movex < 0) {
				move[count] = 4;
				count++;
			}
			//y座標を増減
			while (std::fabs(slope)*x >= ycount) {  //各xに対してslope*xが1を超えたタイミングでyを1回増減、2を超えたタイミングでyをもう1回増減させたい。3を超えたらさらにもう一回yを...(略)
				if (movey > 0) {
					move[count] = 1;
					count++;
				}
				else if (movey < 0) {
					move[count] = 2;
					count++;
				}
				ycount++;  //yが増減した回数を増やす
			}
		}
	}

	//ドローンの位置を更新していく
	for (int i = 0; i<std::abs(movex) + std::abs(movey); i++) {

		d.avoidance();      //進もうとしてる座標が障害物でふさがってたら障害物回避。障害物回避が起こった場合D.flag==1になってる

		if (!d.flag) {      //障害物回避が起こらなかった場合
			if (move[i] == 1) {
				d.y += 1;   //前に1進む
			}
			else if (move[i] == 2) {
				d.y += -1;  //後ろに1進む
			}
			else if (move[i] == 3) {
				d.x += 1;   //右に1進む
			}
			else if (move[i] == 4) {
				d.x += -1;  //左に1進む
			}
			struct Point p = { d.x, d.y };
			route[d.i].push_back(p);
		}
		else if (d.flag) {  //障害物回避が起こった場合
			break;
		}
	}

	delete[]move;

	if (d.flag == true) {   //障害物回避が起こった場合
		d.flag = false;     //再帰内では障害物回避は起きてないから0に戻す
		dronego();          //再帰を使って回避後の地点から目的地の端点に進む
	}
}
/*----------------------------------------------------------------------------------------------------------*/


/*-------------------------Drone::avoidance()----------------------------------------------------------------*/
bool chk_wall(int map[][Y], int i, int j) {
	if (map[i][j] == 0) {
		return false;
	}
	else {
		return true;
	}
}

void Drone::avoidance() {
	flag = true;
	bool right = false, left = false;
	if (fil[2][0] && fil[0][0] && fil[4][0]) {        //前方三つのセンサーが反応
		if (fil[4][2]) {                              //右に壁判定且つ右に壁はマップに存在しないとき
			while (true) {
				if (chk_wall(map[z], x + 1, y)) {       //前方の障害物がなくなるまで左に進む
					if (!fil[0][3] && fil[0][4]) {
						while (fil[4][3]) {
							y += 1;
						}
						return;
					}
					x -= 1;
					update_fil();
				}
				else {
					left = true;
					break;
				}
			}
			if (left) {
				while (true) {
					if (chk_wall(map[z], x - 1, y)) {
						if (!fil[0][1] && fil[0][0]) {
							while (fil[0][3]) {
								y += 1;
							}
							return;
						}
						x += 1;
						update_fil();
					}
					else {
						right = true;
						break;
					}
				}
			}
		}
		else if (fil[0][2]) {                        //左に壁判定且つ左の壁がマップに存在しないとき
			while (true) {
				if (chk_wall(map[z], x - 1, y)) {       //前方の障害物がなくなるまで右に進む
					if (!fil[0][1] && fil[0][0]) {
						while (fil[0][3]) {
							y += 1;
						}
						return;
					}
					x += 1;
					update_fil();
				}
				else {
					right = true;
					break;
				}
			}
			if (right) {
				while (true) {
					if (chk_wall(map[z], x + 1, y)) {    //前方の障害物がなくなるまで左に進む
						if (!fil[0][3] && fil[0][4]) {
							while (fil[4][3]) {
								y += 1;
							}
							return;
						}
						x -= 1;
						update_fil();
					}
					else {
						left = true;
						break;
					}
				}
			}
		}
	}
	else if (fil[2][0]) {                        //前方に障害物あり
		if (!fil[0][0] && fil[0][4]) {            //左斜め前に障害物なしかつ右斜め前に障害物あり
			while (true) {
				if (chk_wall(map[z], x + 1, y + 1)) { //右斜め前に障害物がなくなるまで左斜め前に進む
					if (!fil[0][4]) {
						return;
					}
					x -= 1;
					y += 1;
					update_fil();
				}
			}
		}
		else if (!fil[0][4] && fil[0][0]) {       //右斜め前に障害物なしかつ左斜め前に障害物あり
			while (true) {
				if (chk_wall(map[z], x - 1, y + 1)) {  //左前方に障害物がなくなるまで
					if (!fil[0][0]) {
						return;
					}
					x += 1;
					y += 1;
					update_fil();
				}
			}
		}
	}
	else {
		flag = false;
	}
	if (left && right) {
		z += 1;               //階層の変更
		Dijkstra();
	}
	//後ろに下がる処理は後日実装予定
	// else if(fil[2][0] && fil[0][2]){
	//     while(true){
	//         if(chk_wall(D, D.x-1,D.y)){
	//             if(fil[0][0])
	//             D.y -= 1;
	//             update_fil();
	//         }
	//     }
	// }

}

void Drone::update_fil() {       //nmap: 障害物情報込みのマップ
	for (int ii = -2; ii<3; ii++) {
		for (int j = -2; j<3; j++) {
			if (map[x + ii][y + ii] != 0) {
				fil[ii + 2][j + 2] = true;
			}
			else {
				fil[ii + 2][j + 2] = false;
			}
		}
	}
}
/*-----------------------------------------------------------------------------------------------------------*/


/*-----------------------------------------その他-----------------------------------------------------------*/
void input_map(std::string file, int map[][Y]) {
	std::string str;
	int tmp;
	std::ifstream ifile(file.c_str());
	for (int i = 0; i<X; i++) {
		std::getline(ifile, str);
		std::istringstream iss(str);
		int j = 0;
		while (iss >> tmp) {
			map[i][j] = tmp;
			j++;
		}
	}
}

/*---------------------------------------------------------------------------------------------------------*/