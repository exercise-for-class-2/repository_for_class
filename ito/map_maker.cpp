# include <Siv3D.hpp> //Siv3Dのライブラリを呼び出すのに使う。
# include <HamFramework.hpp> //シーンを操作するのに使う。

#define MAX 2000 //mapの最大行数と最大列数を定義する。

void make_map(int map[MAX][MAX],int cnum, int x, int y,String name); //mapを作る関数

struct CommonData //シーンに共通なデータの定義する。参照する際は、"m_data-> 変数"として呼び出す。
{
	int x = 1, y = 1; //x = 列数 y = 行数　として定義する。
	const Font font{ 20 }; //シーンで使うFontを定義する。
	int map[MAX][MAX]; //mapの値を格納する二次元配列
	int cnum = 2; //mapの属性値数を格納する
	String name = L"Please_Input_FileName"; //出力ファイル名を定義する。
};
using MyApp = SceneManager<String, CommonData>; //MyAppとしてシーン管理を定義する。	
												//String型でシーンの名前を管理し、共通データにはCommonDataを用いる。

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
		m_data->font(L"決定").drawAt(button.center, Palette::Black); //buttonの中央にBlackで文字を表示する。
		m_data->font(L"地図の属性数を入力してください").draw(0,0,Palette::Black); //共通データのfontに従って文字を表示する。
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
			m_data->font(L"決定").drawAt(button.center,Palette::Black); //buttonの中央にBlackで文字を表示する。
			m_data->font(L"地図の横幅を入力してください").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
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
		m_data->font(L"決定").drawAt(button.center, Palette::Black); //buttonの中央にBlackで文字を表示する。
		m_data->font(L"地図の縦幅を入力してください").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
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
			m_data->font(L"はい").drawAt(Yes.center, Palette::Black); //Yesの中央にBlackで文字を表示する。
			m_data->font(L"いいえ").drawAt(No.center, Palette::Black); //Noの中央にBlackで文字を表示する。
			m_data->font(L"入力は正しいですか？").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
			m_data->font(L"属性数",m_data->cnum,L" 横幅：", m_data->x, L" 縦幅：", m_data->y).draw(0, 30,Palette::Black); //共通データのfontに従って文字を表示する。
			if (Yes.leftClicked) //Yesが左クリックされた場合
			{
				changeScene(L"MAKE_MAP");
			}
			else if (No.leftClicked) //Noが左クリックされた場合
			{
				changeScene(L"INPUT_C"); //シーン"INPUT_CHECK"に移動する。
			}
		}
};

class MAKE_MAP : public MyApp::Scene //mapを作るシーン
{
	public:
		int x_base = 0; //xの基座標を定義する。
		int y_base = 0; //yの基座標を定義する。
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
			for (int i = 0; i < m_data->x; i++) //共通データのxまでiを回す。
			{
				for (int j = 0; j < m_data->y; j++) //共通データのyまでjを回す。
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

					if (rect.mouseOver)
					{
						over_i = i;
						over_j = j;
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
			m_data->font(L"完成").drawAt(rect2.center, Palette::Black); //rect2の中央にBlackで文字を表示する。
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
			m_data->font(L"はい").drawAt(Yes.center, Palette::Black); //Yesの中央にBlackで文字を表示する。
			m_data->font(L"いいえ").drawAt(No.center, Palette::Black); //Noの中央にBlackで文字を表示する。
			m_data->font(L"地図は完成しましたか？").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
			m_data->font(L"FILE名:", m_data->name).draw(0, 30,Palette::Black); //共通データのfontに従って文字を表示する。
			Input::GetCharsHelper(m_data->name); //textに文字入力を受け取る。
			if (Yes.leftClicked) //Yesが左クリックされた場合
			{
				make_map(m_data->map,m_data->cnum, m_data->x, m_data->y, m_data->name); //作ったマップをdatファイルとして出力する。
				changeScene(L"FINAL"); //シーン"FINAL"に移動する。
			}
			else if (No.leftClicked) //Noが左クリックされた場合
			{
				changeScene(L"MAKE_MAP"); //シーン"INPUT_CHECK"に移動する。
			}
		}
};

class FINAL : public MyApp::Scene //もう一度マップを作るか確認するシーン
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
		m_data->font(L"はい").drawAt(Yes.center, Palette::Black); //Yesの中央にBlackで文字を表示する。
		m_data->font(L"いいえ").drawAt(No.center, Palette::Black); //Noの中央にBlackで文字を表示する。
		m_data->font(L"別のマップを作りますか？").draw(0, 0, Palette::Black); //共通データのfontに従って文字を表示する。
		if (Yes.leftClicked) //Yesが左クリックされた場合
		{
			for (int i = 0; i < m_data->x; i++)
			{
				for (int j = 0; j < m_data->y; j++)
				{
					m_data->map[i][j] = 0; //mapの初期化を行う。
				}
			}
			m_data->name = L"Please_Input_FileName"; //ファイル名を初期化する。
			changeScene(L"INPUT_C"); //シーン"INPUT_C"に移動する。
		}
		else if (No.leftClicked) //Noが左クリックされた場合
		{
			System::Exit(); //システムを終了する。
		}
	}
};

void Main()
{
	Graphics::SetBackground(Palette::White); //背景を白にする
	MyApp manager; //シーン全体を管理する。
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
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			file.write(map[i][j]%cnum, L" "); //" "区切りで一行ずつ書き込む。
		}
		file.writeln(); //一行ごとに改行をする。
	}
}