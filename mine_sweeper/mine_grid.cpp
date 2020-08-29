#include "mine_grid.h"
#include <random>
#include <utility>
#include <iostream>
#include <vector>

mine_grid::mine_grid(int w, int h, int mnum):width(w),height(h),mine_num(mnum)
{
	// 为雷区分配内存
	mine_area = new int[w * h]{};
	num_area = new int[w * h]{};
	open_area = new int[w * h]{};
	flag_area = new int[w * h]{};
}

mine_grid::mine_grid(mine_grid&& rrv) noexcept
{
	if (&rrv != this)
	{
		mine_area = rrv.mine_area;
		num_area = rrv.num_area;
		open_area = rrv.open_area;
		flag_area = rrv.flag_area;
		width = rrv.width;
		height = rrv.height;
		mine_num = rrv.mine_num;
		rrv.free();
	}
}

mine_grid::~mine_grid()
{
	free();
}

void mine_grid::free()
{
	delete[] mine_area;
	mine_area = nullptr;
	delete[] num_area;
	num_area = nullptr;
	delete[] open_area;
	open_area = nullptr;
	delete[] flag_area;
	flag_area = nullptr;
	width = 0;
	height = 0;
	mine_num = 0;
}

int mine_grid::solve()
{
	/*if (find_unopened_equal_num() == 0)
		if (find_flaged_equal_num() == 0)
			random_exploit();*/
	if (find_unopened_equal_num() == 1)
		return 1;
	if (find_flaged_equal_num() == 1)
		return 1;
	
	// 逻辑推理
	// ...

	if (random_exploit() == 1)
		return 1;
	
	return 0;
}

int mine_grid::random_exploit()
{
	std::vector<int> unopened_index;
	unopened_index.reserve(width * height);
	for (int i = 0; i != width * height; ++i)
	{
		if (open_area[i] == 0)
			unopened_index.push_back(i);
	}
	std::random_device rd;
	std::uniform_int_distribution<unsigned> u(0, unopened_index.size());
	std::default_random_engine e1(rd());
	auto index = u(e1);
	auto i = unopened_index[index];
	std::cout << "random exploit: " << (i%width) << ' ' << (i/width) << std::endl;
	return shovel(i % width, i / width);
}

int mine_grid::find_unopened_equal_num()
{
	for (int iy = 0; iy != height; ++iy)
	{
		for (int ix = 0; ix != width; ++ix)
		{
			if (isopen(ix, iy) == 1)
			{
				auto num = get_num(ix, iy);
				if (num != 0 && surrounding_unopened_num(ix, iy) == num)
				{
					int rx, ry;
					for (int oy = -1; oy != 2; ++oy)
						for (int ox = -1; ox != 2; ++ox)
							if (relative_position(ix, iy, ox, oy, rx, ry) == 1)
								if (isopen(rx, ry) == 0&&isflag(rx,ry)==0)
								{
									std::cout << "flag: " << rx << ' ' << ry << std::endl;
									return flag(rx, ry);
								}
				}
			}
		}
	}
	return 0;
}

int mine_grid::find_flaged_equal_num()
{
	for (int iy = 0; iy != height; ++iy)
	{
		for (int ix = 0; ix != width; ++ix)
		{
			if (isopen(ix, iy))
			{
				auto num = get_num(ix, iy);
				if (num != 0 && surrounding_flaged_num(ix, iy) == num)
				{
					int rx, ry;
					for (int oy = -1; oy != 2; ++oy)
						for (int ox = -1; ox != 2; ++ox)
							if (relative_position(ix, iy, ox, oy, rx, ry))
								if (isflag(rx, ry) == 0 && isopen(rx, ry) == 0)
								{
									std::cout << "shovel: " << rx << ' ' << ry << std::endl;
									return shovel(rx, ry);
								}
				}
			}
		}
	}
	return 0;
}

int mine_grid::get_num(int x, int y)
{
	return num_area[x + y * width];
}

int mine_grid::surrounding_unopened_num(int x, int y)
{
	int rx, ry;
	int num = 0;
	for (int oy = -1; oy != 2; ++oy)
	{
		for (int ox = -1; ox != 2; ++ox)
		{
			if (relative_position(x, y, ox, oy, rx, ry) == 1)
				if (isopen(rx, ry) == 0)
					num++;
		}
	}
	return num;
}

int mine_grid::surrounding_flaged_num(int x, int y)
{
	int rx, ry;
	int num = 0;
	for (int oy = -1; oy != 2; ++oy)
	{
		for (int ox = -1; ox != 2; ++ox)
		{
			if (relative_position(x, y, ox, oy, rx, ry) == 1)
				if (isflag(rx, ry) == 1)
					num++;
		}
	}
	return num;
}

int mine_grid::arrange_mine()
{
	for (int i = 0; i != m_num(); ++i)
		mine_area[i] = 1;	// 1 代表有雷
	// 打乱
	std::random_device r;
	std::uniform_int_distribution<unsigned> u;
	std::default_random_engine e1(r());
	using std::swap;
	for (unsigned i = width * height - 1; i > 0; --i)
	{
		u.param(std::uniform_int_distribution<unsigned>::param_type{ 0,i });
		auto index = u(e1);
		swap(mine_area[i], mine_area[index]);
	}
	// 遍历雷区，标上某格周围的雷数
	for (int y = 0; y != height; ++y)
	{
		for (int x = 0; x != width; ++x)
		{
			if (mine_area[x + y * width] == 1)
				num_area[x + y * width] = 0;	//当前格是雷直接跳过(还是标个0吧
			else
				num_area[x + y * width] = surrounding_mine_num(x, y);
		}
	}
	return 1;
}

int mine_grid::dig(int x, int y)
{
	deflag(x, y);
	open_area[x + y * width] = 1;	// 1代表被打开
	return 1;	// 成功
}

int mine_grid::flag(int x, int y)
{
	if (isopen(x,y) == 0)
	{
		flag_area[x + y * width] = 1;	// 1代表被标记
		return 1;
	}
	return 0;
}

int mine_grid::deflag(int x, int y)
{
	if (flag_area[x + y * width] == 1)
	{
		flag_area[x + y * width] = 0;
		return 1;
	}
	return 0;
}

int mine_grid::shovel(int x, int y)
{
	int rx, ry;
	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		dig(x, y);
		if (surrounding_mine_num(x, y) == 0)
			for (int iy = -1; iy != 2; ++iy)
				for (int ix = -1; ix != 2; ++ix)
					if (relative_position(x, y, ix, iy, rx, ry))
						if (isopen(rx, ry) != 1)
							shovel(rx, ry);
		return 1;
	}
	return 0;
}

int mine_grid::isopen(int x, int y)
{
	return open_area[x + y * width];
}

int mine_grid::isflag(int x, int y)
{
	return flag_area[x + y * width];
}

int mine_grid::isBOOM()
{
	for (int i = 0; i != width * height; ++i)
	{
		if (open_area[i] == 1 && mine_area[i] == 1)
			return 1;	// 踩雷了
	}
	return 0;	// safe
}

int mine_grid::iswin()
{
	for (int i = 0; i != width * height; ++i)
	{
		// 存在无雷的地方未打开
		if (open_area[i] == 0 && mine_area[i] == 0)
			return 0;	
	}
	return 1;
}

auto mine_grid::size()
{
	return std::pair{ width,height };
}

int mine_grid::m_num()
{
	return mine_num;
}

int mine_grid::display()
{
	for (int iy = 0; iy != height; ++iy)
	{
		for (int ix = 0; ix != width; ++ix)
		{
			if (open_area[ix + iy * width] == 0)
				std::cout << "█";
			else
				std::cout << num_area[ix + iy * width]/*<<' '*/;
		}
		std::cout << std::endl;
	}
	std::cout << "boom: " << m_num() << std::endl;
	return 1;
}

int mine_grid::displayBOOM()
{
	for (int iy = 0; iy != height; ++iy)
	{
		for (int ix = 0; ix != width; ++ix)
		{
			if (mine_area[ix + iy * width] == 1)
				std::cout << "*";
			else
				std::cout << num_area[ix + iy * width];
		}
		std::cout << std::endl;
	}
	std::cout << "boom: " << m_num() << std::endl;
	return 1;
}

int mine_grid::surrounding_mine_num(int x, int y)
{
	int num = 0;
	int rx = 0, ry = 0;
	for (int iy=-1; iy != 2; ++iy)
	{
		for (int ix=-1; ix != 2; ++ix)
		{
			if (relative_position(x, y, ix, iy, rx, ry) == 1)
				if (mine_area[rx + ry * width] == 1)
					num++;
		}
	}
	return num;
}

int mine_grid::relative_position(int x, int y, int x_off, int y_off,int &rx,int &ry)
{
	rx = x + x_off;
	ry = y + y_off;
	if (rx < 0 || rx >= width || ry < 0 || ry >= height)
		return 0;	//不在范围内，失败
	return 1;	//成功
}
