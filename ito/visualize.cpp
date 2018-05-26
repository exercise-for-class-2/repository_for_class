
# include <Siv3D.hpp>
# include <iostream>
# include <sstream>
# include <fstream>

#define X 101
#define Y 101

void input_map(std::string file, int map[][Y], int x, int y);

void Main()
{
	Window::SetStyle(WindowStyle::Sizeable);
	int x = X, y = Y;
	std::string file1 = "map.dat";
	int map[X][Y];
	input_map(file1, map, x, y);

	while (System::Update())
	{
		Graphics3D::FreeCamera();
		for (int i = 0; i < X; i++)
		{
			for (int j = 0; j < Y; j++)
			{
				if (map[i][j] == 1)
				{
					Box(Vec3(i, 0, j), 1).draw();
				}
			}
		}
	}
}

void input_map(std::string file, int map[][Y], int x, int y) {
	std::string str;
	int tmp;
	std::ifstream ifile(file.c_str());
	for (int i = 0; i<x; i++) {
		std::getline(ifile, str);
		std::istringstream iss(str);
		int j = 0;
		while (iss >> tmp) {
			map[i][j] = tmp;
			j++;
		}
	}
}