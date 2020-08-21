#pragma once


class mine_grid
{
public:
	// to-do: 分离创建雷区和布雷
	mine_grid(int width, int height, int mine_num);
	mine_grid(const mine_grid&) = delete;
	mine_grid(mine_grid&& rrv) noexcept;
	~mine_grid();
	void free();
public:
	int solve();			// 自动扫雷程序
	int random_exploit();	// 随机打开一格
	// 扫描，找到某格，它的周围未打开的格数与自身数字(非0)相等，并且标记这些确知雷格
	// 返回 1 为找到，0 为未找到
	int find_unopened_equal_num();
	// 扫描，找到某格，它的周围被标记的格数与自身数字相等，并且打开那些确定非雷格
	// 返回 1 为找到，0 为未找到
	int find_flaged_equal_num();
	int get_num(int x, int y);			// 查看某格数字
	int surrounding_unopened_num(int x,int y);	// 某格周围未打开的格数
	int surrounding_flaged_num(int x, int y);	// 某格周围已标记的格数
public:
	int dig(int x, int y);		// 打开一格
	int flag(int x, int y);		// 标记某格为雷
	int deflag(int x, int y);	// 取消标记
	int shovel(int x, int y);	// 扫一片区域，打开一格后，若周围8格无雷，则都打开
	int isopen(int x, int y);	// 检查某一格是否被打开
	int isflag(int x, int y);	// 检查某一格是否被标记
	int isBOOM();				// 检查是否触雷（打开雷
	int iswin();				// 不是雷的区块都被打开即胜利
	auto size();				// 雷区的长宽
	int m_num();				// 雷的数量
	int display();				// 显示
	int displayBOOM();			// 显示全部地雷
private:
	int surrounding_mine_num(int x, int y);	// 一格周围有几个雷（透视版
	int relative_position(int x, int y, int x_off, int y_off,int &rx,int&ry);	// 给出当前位置相对位移的绝对坐标，
private:
	int* mine_area = nullptr;
	int* num_area = nullptr;
	int* open_area = nullptr;
	int* flag_area = nullptr;
	int width = 0;
	int height = 0;
	int mine_num = 0;
};