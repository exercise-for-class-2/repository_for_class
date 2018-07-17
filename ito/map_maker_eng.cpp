# include <Siv3D.hpp> //Siv3Dのライブラリを呼び出すのに使う。
# include <HamFramework.hpp> //シーンを操作するのに使う。
# include <iostream>
# include <sstream>
# include <fstream>
#define MAX 2000 //mapの最大行数と最大列数を定義する。

void make_map(int map[MAX][MAX],int cnum, int x, int y,String name); //mapを作る関数

struct CommonData //シーンに共通なデータの定義する。参照する際は、"m_data-> 変数"として呼び出す。
{
	int x = 1, y = 1; //x = 列数 y = 行数として定義する。
	const Font font{ 20 }; //シーンで使うFontを定義する。
	int map[MAX][MAX]; //mapの値を格納する二次元配列
	int cnum = 2; //mapの属性値数を格納する
	String name = L"Please_Input_FileName"; //出力ファイル名を定義する。
};
using MyApp = SceneManager<String, CommonData>; //MyAppとしてシーン管理を定義する。	
												//String型でシーンの名前を管理し、共通データにはCommonDataを用いる。
class TOP_MENU : public MyApp::Scene //トップメニュー画面
{
public:

	RoundRect new_map{ 100, 80, 200, 50,4 }; //新規作成に使う丸角長方形を定義する。
	RoundRect read_map{ 100, 160, 250, 50,4 }; //マップを読み込むに使う丸角長方形を定義する。
	void init()
	{
		Graphics::SetBackground(Palette::White); //背景を白にする
	}
	void update() //シーンの間は、この中をループする。
	{
		const Color color_new(new_map.mouseOver ? 220 : 255); //マウスがnew_mapの上にある時の色を定義する。
		const Color color_read(read_map.mouseOver ? 220 : 255); //マウスがread_mapの上にある時の色を定義する。
		m_data->font(L"MENU").draw(150, 20, Palette::Black); 
		new_map.draw(color_new);
		read_map.draw(color_read);
		m_data->font(L"NEW MAP").drawAt(new_map.center, Palette::Black); 
		m_data->font(L"READ MAP").drawAt(read_map.center, Palette::Black);
		if (new_map.leftClicked)
		{
			changeScene(L"INPUT_C"); //シーン"INPUT_X"に移動する。
		}
		else if(read_map.leftClicked)
		{
			changeScene(L"INPUT_C_READ");//シーン"READ_MAP"に移動する。
		}

	}

};

class INPUT_C_READ : public MyApp::Scene //色数を入力するシーン
{
public:

	String text; //入力に使うSting型を定義する。
	RoundRect button{ 200, 60, 80, 50,4 }; //決定ボタンに使う丸角長方形を定義する。
	void init()
	{
		Graphics::SetBackground(Palette::White); //背景を白にする
	}
	void update() //シーンの間は、この中をループする。
	{
		const Color color(button.mouseOver ? 220 : 255); //マウスがbuttonの上にある時の色を定義する。
		button.draw(color); //buttonを色＝colorで表示する。
		m_data->font(L"OK").drawAt(button.center, Palette::Black); //buttonの中央にBlackで文字を表示する。
		m_data->font(L"INPUT THE NUMBER OF ATTRIBUTES").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
		m_data->font(m_data->cnum).draw(0, 30, Palette::Black); //共通データのfontに従って文字を表示する。
		Input::GetCharsHelper(text); //textに文字入力を受け取る。
		if (button.leftClicked) //左クリックをした場合
		{
			m_data->cnum = Parse<int>(text); //textをint型に変換する。
			changeScene(L"INPUT_X_READ"); //シーン"INPUT_X_READ"に移動する。
		}
		else
		{
			m_data->cnum = Parse<int>(text); //textをint型に変換する。
		}

	}

};

class INPUT_X_READ : public MyApp::Scene //Xを入力するシーン
{
public:

	String text; //入力に使うSting型を定義する。
	RoundRect button{ 200, 60, 80, 50,4 }; //決定ボタンに使う丸角長方形を定義する。
	void init()
	{
		Graphics::SetBackground(Palette::White); //背景を白にする
	}
	void update() //シーンの間は、この中をループする。
	{
		const Color color(button.mouseOver ? 220 : 255); //マウスがbuttonの上にある時の色を定義する。
		button.draw(color); //buttonを色＝colorで表示する。
		m_data->font(L"OK").drawAt(button.center, Palette::Black); //buttonの中央にBlackで文字を表示する。
		m_data->font(L"INPUT THE WIDTH OF THE MAP").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
		m_data->font(m_data->x).draw(0, 30, Palette::Black); //共通データのfontに従って文字を表示する。
		Input::GetCharsHelper(text); //textに文字入力を受け取る。
		if (button.leftClicked) //左クリックをした場合
		{
			m_data->x = Parse<int>(text); //textをint型に変換する。
			changeScene(L"INPUT_Y_READ"); //シーン"INPUT_Y_READ"に移動する。
		}
		else
		{
			m_data->x = Parse<int>(text); //textをint型に変換する。
		}

	}

};

class INPUT_Y_READ : public MyApp::Scene //Yを入力するシーン
{
public:

	String text; //入力に使うSting型を定義する。
	RoundRect button{ 200, 60, 80, 50,4 }; //決定ボタンに使う丸角長方形を定義する。
	void init()
	{
		Graphics::SetBackground(Palette::White); //背景を白にする
	}
	void update() //シーンの間は、この中をループする。
	{
		const Color color(button.mouseOver ? 220 : 255); //マウスがbuttonの上にある時の色を定義する。
		button.draw(color); //buttonを色＝colorで表示する。
		m_data->font(L"OK").drawAt(button.center, Palette::Black); //buttonの中央にBlackで文字を表示する。
		m_data->font(L"INPUT THE HEIGHT OF THE MAP").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
		m_data->font(m_data->y).draw(0, 30, Palette::Black); //共通データのfontに従って文字を表示する。
		Input::GetCharsHelper(text); //textに文字入力を受け取る。
		if (button.leftClicked) //左クリックをした場合
		{
			m_data->y = Parse<int>(text); //textをint型に変換する。
			changeScene(L"READ_MAP"); //シーン"INPUT_CHECK"に移動する。
		}
		else
		{
			m_data->y = Parse<int>(text); //textをint型に変換する。
		}

	}

};
class READ_MAP : public MyApp::Scene //トップメニュー画面
{
public:

	RoundRect button{ 100, 100, 80, 50,4 }; //決定に使う丸角長方形を定義する。
	void init()
	{
		Graphics::SetBackground(Palette::White); //背景を白にする
	}
	void update() //シーンの間は、この中をループする。
	{
		const Color color(button.mouseOver ? 220 : 255); //マウスがnew_mapの上にある時の色を定義する。
		button.draw(color);
		m_data->font(L"OK").drawAt(button.center, Palette::Black);
		m_data->font(L"INPUT NAME OF THE MAP").draw(0, 0, Palette::Black);
		m_data->font(L"MAP NAME：").draw(0, 50, Palette::Black);
		m_data->font(m_data->name).draw(140, 50, Palette::Black); //共通データのfontに従って文字を表示する。
		Input::GetCharsHelper(m_data->name);//textに文字入力を受け取る。
		if (button.leftClicked)
		{
			std::string str;
			int tmp;
			std::ifstream ifile((m_data->name+L".dat").c_str());
			for (int i = 20; i<m_data->x+20; i++) {
				std::getline(ifile, str);
				std::istringstream iss(str);
				int j = 20;
				while (iss >> tmp) {
					m_data->map[i][j] = tmp;
					j++;
				}
			}
			for (int i = 20; i < m_data->x + 20; i++) { //mapの周りを壁で囲う
				for (int j = 20; j < m_data->y + 20; j++) {
					if (i == 20 || i == m_data->x + 20 - 1 || j == 20 || j == m_data->y + 20 - 1)
						m_data->map[i][j] = 1;
				}
			}

			changeScene(L"INPUT_CHECK_READ"); //シーン"INPUT_X_READ"に移動する。
		}

	}

};
class INPUT_CHECK_READ : public MyApp::Scene //入力が正しいか確認するシーン
{
public:
	RoundRect Yes{ 100, 90, 100, 50,4 }; //Yesボタンに使う丸角長方形を定義する。
	RoundRect No{ 230, 90, 100, 50,4 }; //Noボタンに使う丸角長方形を定義する。
	void init()
	{
		Graphics::SetBackground(Palette::White); //背景を白にする
	}
	void update() //シーンの間は、この中をループする。
	{
		const Color color_y(Yes.mouseOver ? 220 : 255); //マウスがYesの上にある時の色を定義する。
		const Color color_n(No.mouseOver ? 220 : 255); //マウスがNoの上にある時の色を定義する。
		Yes.draw(color_y); //Yesを色＝color_yで表示する。
		No.draw(color_n); //Noを色＝color_nで表示する。
		m_data->font(L"YES").drawAt(Yes.center, Palette::Black); //Yesの中央にBlackで文字を表示する。
		m_data->font(L"NO").drawAt(No.center, Palette::Black); //Noの中央にBlackで文字を表示する。
		m_data->font(L"IS THIS INPUT CORRECT?").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
		m_data->font(L"THE NUMBER OF ATTRIBUTES", m_data->cnum, L" WIDTH：", m_data->x, L" HEIGHT：", m_data->y).draw(0, 30, Palette::Black); //共通データのfontに従って文字を表示する。
		if (Yes.leftClicked) //Yesが左クリックされた場合
		{
			changeScene(L"MAKE_MAP");
		}
		else if (No.leftClicked) //Noが左クリックされた場合
		{
			for (int i = 20; i < m_data->x + 20; i++)
			{
				for (int j = 20; j < m_data->y + 20; j++)
				{
					m_data->map[i][j] = 0; //mapの初期化を行う。
				}
			}
			m_data->name = L"Please_Input_FileName"; //ファイル名を初期化する。
			changeScene(L"TOP_MENU"); //シーン"INPUT_CHECK"に移動する。
		}
	}
};
class INPUT_C : public MyApp::Scene //色数を入力するシーン
{
public:

	String text; //入力に使うSting型を定義する。
	RoundRect button{ 200, 60, 80, 50,4 }; //決定ボタンに使う丸角長方形を定義する。
	void init()
	{
		Graphics::SetBackground(Palette::White); //背景を白にする
	}
	void update() //シーンの間は、この中をループする。
	{
		const Color color(button.mouseOver ? 220 : 255); //マウスがbuttonの上にある時の色を定義する。
		button.draw(color); //buttonを色＝colorで表示する。
		m_data->font(L"OK").drawAt(button.center, Palette::Black); //buttonの中央にBlackで文字を表示する。
		m_data->font(L"INPUT THE NUMBER OF ATTRIBUTES").draw(0,0,Palette::Black); //共通データのfontに従って文字を表示する。
		m_data->font(m_data->cnum).draw(0, 30, Palette::Black); //共通データのfontに従って文字を表示する。
		Input::GetCharsHelper(text); //textに文字入力を受け取る。
		if (button.leftClicked) //左クリックをした場合
		{
			m_data->cnum = Parse<int>(text); //textをint型に変換する。
			changeScene(L"INPUT_X"); //シーン"INPUT_X"に移動する。
		}
		else
		{
			m_data->cnum = Parse<int>(text); //textをint型に変換する。
		}

	}

};												

class INPUT_X : public MyApp::Scene //Xを入力するシーン
{
	public:

		String text; //入力に使うSting型を定義する。
		RoundRect button{ 200, 60, 80, 50,4 }; //決定ボタンに使う丸角長方形を定義する。
		void init()
		{
			Graphics::SetBackground(Palette::White); //背景を白にする
		}
		void update() //シーンの間は、この中をループする。
		{
			const Color color(button.mouseOver ? 220 : 255); //マウスがbuttonの上にある時の色を定義する。
			button.draw(color); //buttonを色＝colorで表示する。
			m_data->font(L"OK").drawAt(button.center,Palette::Black); //buttonの中央にBlackで文字を表示する。
			m_data->font(L"INPUT THE WIDTH OF THE MAP").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
			m_data->font(m_data->x).draw(0, 30, Palette::Black); //共通データのfontに従って文字を表示する。
			Input::GetCharsHelper(text); //textに文字入力を受け取る。
			if(button.leftClicked) //左クリックをした場合
			{
				m_data->x = Parse<int>(text); //textをint型に変換する。
				changeScene(L"INPUT_Y"); //シーン"INPUT_Y"に移動する。
			}
			else
			{
				m_data->x = Parse<int>(text); //textをint型に変換する。
			}

		}

};

class INPUT_Y : public MyApp::Scene //Yを入力するシーン
{
public:

	String text; //入力に使うSting型を定義する。
	RoundRect button{ 200, 60, 80, 50,4 }; //決定ボタンに使う丸角長方形を定義する。
	void init()
	{
		Graphics::SetBackground(Palette::White); //背景を白にする
	}
	void update() //シーンの間は、この中をループする。
	{
		const Color color(button.mouseOver ? 220 : 255); //マウスがbuttonの上にある時の色を定義する。
		button.draw(color); //buttonを色＝colorで表示する。
		m_data->font(L"OK").drawAt(button.center, Palette::Black); //buttonの中央にBlackで文字を表示する。
		m_data->font(L"INPUT THE HEIGHT OF THE MAP").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
		m_data->font(m_data->y).draw(0, 30, Palette::Black); //共通データのfontに従って文字を表示する。
		Input::GetCharsHelper(text); //textに文字入力を受け取る。
		if (button.leftClicked) //左クリックをした場合
		{
			m_data->y = Parse<int>(text); //textをint型に変換する。
			changeScene(L"INPUT_CHECK"); //シーン"INPUT_CHECK"に移動する。
		}
		else
		{
			m_data->y = Parse<int>(text); //textをint型に変換する。
		}

	}

};

class INPUT_CHECK : public MyApp::Scene //入力が正しいか確認するシーン
{
	public:
		RoundRect Yes{ 100, 90, 100, 50,4 }; //Yesボタンに使う丸角長方形を定義する。
		RoundRect No{ 230, 90, 100, 50,4 }; //Noボタンに使う丸角長方形を定義する。
		void init()
		{
			Graphics::SetBackground(Palette::White); //背景を白にする
		}
		void update() //シーンの間は、この中をループする。
		{
			const Color color_y(Yes.mouseOver ? 220 : 255); //マウスがYesの上にある時の色を定義する。
			const Color color_n(No.mouseOver ? 220 : 255); //マウスがNoの上にある時の色を定義する。
			Yes.draw(color_y); //Yesを色＝color_yで表示する。
			No.draw(color_n); //Noを色＝color_nで表示する。
			m_data->font(L"OK").drawAt(Yes.center, Palette::Black); //Yesの中央にBlackで文字を表示する。
			m_data->font(L"NO").drawAt(No.center, Palette::Black); //Noの中央にBlackで文字を表示する。
			m_data->font(L"IS THIS INPUT CORRECT?").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
			m_data->font(L"THE NUMBER OF ATTRIBUTES", m_data->cnum, L" WIDTH：", m_data->x, L" HEIGHT：", m_data->y).draw(0, 30, Palette::Black); //共通データのfontに従って文字を表示する。
			if (Yes.leftClicked) //Yesが左クリックされた場合
			{
				for (int i = 20; i < m_data->x+20; i++) { //mapの周りを壁で囲う
					for (int j = 20; j < m_data->y+20; j++) {
						if (i == 20 || i == m_data->x+20 - 1 || j == 20 || j == m_data->y+20 - 1)
							m_data->map[i][j] = 1;
					}
				}
				changeScene(L"MAKE_MAP");
			}
			else if (No.leftClicked) //Noが左クリックされた場合
			{
				changeScene(L"TOP_MENU"); //シーン"INPUT_CHECK"に移動する。
			}
		}
};

class MAKE_MAP : public MyApp::Scene //mapを作るシーン
{

	public:
		int x_base = -40; //xの基座標を定義する。
		int y_base = -40; //yの基座標を定義する。
		int over_i;
		int over_j;
		Font cfont{ 10 }; //マス内の文字のFontを定義する。
		RoundRect rect2{ 539,429,100,50,4 }; //完成ボタンに使う丸角長方形を定義する。
		RoundRect rect3{ 0,429,100,50,4 }; //座標を表示する枠に使う丸角長方形を定義する。
		void init()
		{
			Graphics::SetBackground(Palette::Black); //背景を黒にする
		}
		void update() //シーンの間は、この中をループする。
		{

			for (int i = 20; i < m_data->x+20; i++) //共通データのxまでiを回す。
			{
				for (int j = 20; j < m_data->y+20; j++) //共通データのyまでjを回す。
				{
					Rect rect(i * 20+x_base, j * 20+y_base, 20, 20); //mapを表す長方形を定義する。
					if (rect.leftClicked) // rectが左クリックされた場合
					{
						m_data->map[i][j] += 1; //共通データのmap[i][j]に１を加算する。
					}
					if (rect.rightClicked)
					{
						m_data->map[i][j] += m_data->cnum-1;
					}
					
					if (Input::KeyW.pressed&& rect.leftClicked) {
					for (int k = 0; k < 10; k++) {
					m_data->map[i][j - k]+=1;
					}
					m_data->map[i][j] += m_data->cnum - 1;
					}
					if (Input::KeyS.pressed && rect.leftClicked){
					for (int k = 0; k < 10; k++) {
					m_data->map[i][j + k]+=1;
					}
					m_data->map[i][j] += m_data->cnum - 1;
					}
					if (Input::KeyD.pressed && rect.leftClicked) {
					for (int k = 0; k < 10; k++) {
					m_data->map[i+k][j]+=1;
					}
					m_data->map[i][j] += m_data->cnum - 1;
					}
					if (Input::KeyA.pressed && rect.leftClicked) {
					for (int k = 0; k <10; k++) {
					m_data->map[i-k][j]+=1;
					}
					m_data->map[i][j] += m_data->cnum - 1;
					}
					
					if (Input::KeyB.pressed&& rect.leftClicked) {
					for (int k = -5; k < 5; k++) {
					for (int l = -5; l < 5; l++) {
							m_data->map[i + k][j + l] += 1;
						}
						}
					m_data->map[i][j] += m_data->cnum - 1;
					}
					
					if (rect.mouseOver)
					{
						over_i = i-20;
						over_j = j-20;
					}
					int c = m_data->map[i][j] % m_data->cnum; //共通データのmap[i][j]を属性数で割った余りを定義する。
					const Color color(c != 0 ? HSV(360/c) : HSV(0,0,0)); //クリック回数によって変化する色を定義する。
					rect.draw(color); //rectをcolor色で表示する。
					rect.drawFrame(0, 2, Palette::Green); //rectの枠を緑色で表示する。
					cfont(c).drawAt(rect.center, Palette::White); //rectの中央にWhiteで文字を表示する。

				}
			}
			//以下のif文を使って、矢印キーで画面を操作できるようにする。
			if (Input::KeyRight.pressed) //→が押されている場合
			{
				x_base -= 5;
			}
			if (Input::KeyLeft.pressed) //←が押されている場合
			{
				x_base += 5;
			}
			if (Input::KeyUp.pressed) //↑が押されている場合
			{
				y_base += 5;
			}
			if (Input::KeyDown.pressed) //↓が押されている場合
			{
				y_base -= 5;
			}
			rect3.draw();
			cfont(L"(", over_i, L",", over_j, L")").drawAt(rect3.center, Palette::Black);
			const Color color2(rect2.mouseOver ? 220 : 255); //マウスがrect2の上にある時の色を定義する。
			rect2.draw(color2); //rect2をcolor2色で表示する。
			m_data->font(L"COMPLETE").drawAt(rect2.center, Palette::Black); //rect2の中央にBlackで文字を表示する。
			if (rect2.leftClicked) //rect2が左クリックされた場合
			{
				changeScene(L"CHECK_MAP"); //シーン"CHECK_MAP"に移動する。
			}
		}
};

class CHECK_MAP : public MyApp::Scene //mapの完成を確認するシーン
{
	public:
		RoundRect Yes{ 100, 90, 100, 50,4 }; //Yesボタンに使う丸角長方形を定義する。
		RoundRect No{ 230, 90, 100, 50,4 }; //Noボタンに使う丸角長方形を定義する。
		void init()
		{
			Graphics::SetBackground(Palette::White); //背景を白にする
		}
		void update()//シーンの間は、この中をループする。
		{
			const Color color_y(Yes.mouseOver ? 220 : 255); //マウスがYesの上にある時の色を定義する。
			const Color color_n(No.mouseOver ? 220 : 255); //マウスがNoの上にある時の色を定義する。
			Yes.draw(color_y); //Yesを色＝color_yで表示する。
			No.draw(color_n); //Noを色＝color_nで表示する。
			m_data->font(L"YES").drawAt(Yes.center, Palette::Black); //Yesの中央にBlackで文字を表示する。
			m_data->font(L"NO").drawAt(No.center, Palette::Black); //Noの中央にBlackで文字を表示する。
			m_data->font(L"DO YOU COMPLETE MAP MAKING?").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
			m_data->font(L"FILE NAME:", m_data->name).draw(0, 30,Palette::Black); //共通データのfontに従って文字を表示する。
			Input::GetCharsHelper(m_data->name); //textに文字入力を受け取る。
			if (Yes.leftClicked) //Yesが左クリックされた場合
			{
				make_map(m_data->map,m_data->cnum, m_data->x, m_data->y, m_data->name); //作ったマップをdatファイルとして出力する。
				changeScene(L"FINAL"); //シーン"FINAL"に移動する。
			}
			else if (No.leftClicked) //Noが左クリックされた場合
			{
				changeScene(L"MAKE_MAP"); //シーン"MAKE_MAP"に移動する。
			}
		}
};

class FINAL : public MyApp::Scene //もう一度マップを作るか確認するシーン
{
public:
	RoundRect r1{ 100, 90, 400, 50,4 }; //Yesボタンに使う丸角長方形を定義する。
	RoundRect r2{ 100, 180, 400, 50,4 }; //Noボタンに使う丸角長方形を定義する。
	RoundRect r3{ 100, 270, 400, 50,4 }; //Yesボタンに使う丸角長方形を定義する。
	void init()
	{
		Graphics::SetBackground(Palette::White); //背景を白にする
	}
	void update()//シーンの間は、この中をループする。
	{
		const Color color_r1(r1.mouseOver ? 220 : 255); //マウスがYesの上にある時の色を定義する。
		const Color color_r2(r2.mouseOver ? 220 : 255); //マウスがNoの上にある時の色を定義する。
		const Color color_r3(r3.mouseOver ? 220 : 255); //マウスがNoの上にある時の色を定義する。
		r1.draw(color_r1); //Yesを色＝color_yで表示する。
		r2.draw(color_r2); //Noを色＝color_nで表示する。
		r3.draw(color_r3);
		m_data->font(L"CREATE ANOTHER MAP").drawAt(r1.center, Palette::Black); //Yesの中央にBlackで文字を表示する。
		m_data->font(L"EDIT SAME MAP").drawAt(r2.center, Palette::Black); //Noの中央にBlackで文字を表示する。
		m_data->font(L"FINISH").drawAt(r3.center, Palette::Black);

		if (r1.leftClicked) //Yesが左クリックされた場合
		{
			for (int i = 20; i < m_data->x+20; i++)
			{
				for (int j = 20; j < m_data->y+20; j++)
				{
					m_data->map[i][j] = 0; //mapの初期化を行う。
				}
			}
			m_data->name = L"Please_Input_FileName"; //ファイル名を初期化する。
			changeScene(L"TOP_MENU"); //シーン"INPUT_C"に移動する。
		}
		else if (r2.leftClicked) {
			changeScene(L"MAKE_MAP"); //シーン"MAKE_MAP"に移動する。
		}
		else if (r3.leftClicked) //Noが左クリックされた場合
		{
			System::Exit(); //システムを終了する。
		}
	}
};

void Main()
{
	Graphics::SetBackground(Palette::White); //背景を白にする
	MyApp manager; //シーン全体を管理する。
	manager.add<TOP_MENU>(L"TOP_MENU");//TOP_MENUをシーンに登録する。
	manager.add<INPUT_C_READ>(L"INPUT_C_READ"); //INPUT_C_READをシーンに登録する。
	manager.add<INPUT_X_READ>(L"INPUT_X_READ"); //INPUT_X_READをシーンに登録する。
	manager.add<INPUT_Y_READ>(L"INPUT_Y_READ"); //INPUT_Y_READをシーンに登録する。
	manager.add<READ_MAP>(L"READ_MAP");//READ_MAPをシーンに登録する。
	manager.add<INPUT_CHECK_READ>(L"INPUT_CHECK_READ");
	manager.add<INPUT_C>(L"INPUT_C"); //INPUT_Cをシーンに登録する。
	manager.add<INPUT_X>(L"INPUT_X"); //INPUT_Xをシーンに登録する。
	manager.add<INPUT_Y>(L"INPUT_Y"); //INPUT_Yをシーンに登録する。
	manager.add<INPUT_CHECK>(L"INPUT_CHECK"); //INPUT_CHECKをシーンに登録する。
	manager.add<MAKE_MAP>(L"MAKE_MAP"); //MAKE_MAPをシーンに登録する。
	manager.add<CHECK_MAP>(L"CHECK_MAP"); //CHECK_MAPをシーンに登録する。
	manager.add<FINAL>(L"FINAL"); //FINALをシーンに登録する。
	Window::SetStyle(WindowStyle::Sizeable); //windowをサイズ変更可能にする。

	
	while (System::Update())
	{

		Graphics3D::FreeCamera();//3D空間のカメラを操作自由にする。
		manager.updateAndDraw(); //登録したシーンを実行する。

	}
}

void make_map(int map[MAX][MAX],int cnum, int x, int y,String name)
{
	TextWriter file(name + L".dat"); //"入力された名前"+".dat"を開く。
	for (int i = 20; i < x+20; i++)
	{
		for (int j = 20; j < y+20; j++)
		{
			file.write(map[i][j]%cnum, L" "); //" "区切りで一行ずつ書き込む。
		}
		file.writeln(); //一行ごとに改行をする。
	}
}
