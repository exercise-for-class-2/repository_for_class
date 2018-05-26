
# include <Siv3D.hpp>
# include <iostream>
# include <fstream>
# include <sstream>

#define X 101  
#define Y 101

struct Point3 {
	int x = 0;
	int y = 0;
	int z = 0;
};

void input_map(std::string file, int map[][Y], int x, int y);
void Main()
{

	Window::SetStyle(WindowStyle::Sizeable);
	int x = X, y = Y;
	std::string file1 = "map2.dat";
	int map[X][Y];
	input_map(file1, map, x, y);
	const Font font(30);
	struct Point3 pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;


	while (System::Update())
	{
		Graphics3D::FreeCamera();
		if (Input::KeyL.pressed)
		{
			++pos.x;
		}
		if (Input::KeyJ.pressed)
		{
			--pos.x;
		}
		if (Input::KeyI.pressed)
		{
			++pos.z;
		}
		if (Input::KeyK.pressed)
		{
			--pos.z;
		}

		Sphere(pos.x, pos.y, pos.z, 1).draw(Palette::Lawngreen);
		for (int i = 0; i < X; i++)
		{
			for (int j = 0; j < Y; j++)
			{
				if (map[i][j] != 0)
				{
					Box(1).asMesh()
						.translated(i, 0, j)
						.draw();

				}

			}
		}
	}
}

void input_map(std::string file, int map[][Y], int x, int y) {
	std::string str;
	int tmp;
	std::ifstream ifile(file.c_str());
	for (int i = 0; i<y; i++) {
		std::getline(ifile, str);
		std::istringstream iss(str);
		int j = 0;
		while (iss >> tmp) {
			map[j][i] = tmp;
			j++;
		}
	}
}