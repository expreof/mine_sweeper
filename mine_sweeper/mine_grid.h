#pragma once


class mine_grid
{
public:
	// to-do: ���봴�������Ͳ���
	mine_grid(int width, int height, int mine_num);
	mine_grid(const mine_grid&) = delete;
	mine_grid(mine_grid&& rrv) noexcept;
	~mine_grid();
	void free();
public:
	int solve();			// �Զ�ɨ�׳���
	int random_exploit();	// �����һ��
	// ɨ�裬�ҵ�ĳ��������Χδ�򿪵ĸ�������������(��0)��ȣ����ұ����Щȷ֪�׸�
	// ���� 1 Ϊ�ҵ���0 Ϊδ�ҵ�
	int find_unopened_equal_num();
	// ɨ�裬�ҵ�ĳ��������Χ����ǵĸ���������������ȣ����Ҵ���Щȷ�����׸�
	// ���� 1 Ϊ�ҵ���0 Ϊδ�ҵ�
	int find_flaged_equal_num();
	int get_num(int x, int y);			// �鿴ĳ������
	int surrounding_unopened_num(int x,int y);	// ĳ����Χδ�򿪵ĸ���
	int surrounding_flaged_num(int x, int y);	// ĳ����Χ�ѱ�ǵĸ���
public:
	int dig(int x, int y);		// ��һ��
	int flag(int x, int y);		// ���ĳ��Ϊ��
	int deflag(int x, int y);	// ȡ�����
	int shovel(int x, int y);	// ɨһƬ���򣬴�һ�������Χ8�����ף��򶼴�
	int isopen(int x, int y);	// ���ĳһ���Ƿ񱻴�
	int isflag(int x, int y);	// ���ĳһ���Ƿ񱻱��
	int isBOOM();				// ����Ƿ��ף�����
	int iswin();				// �����׵����鶼���򿪼�ʤ��
	auto size();				// �����ĳ���
	int m_num();				// �׵�����
	int display();				// ��ʾ
	int displayBOOM();			// ��ʾȫ������
private:
	int surrounding_mine_num(int x, int y);	// һ����Χ�м����ף�͸�Ӱ�
	int relative_position(int x, int y, int x_off, int y_off,int &rx,int&ry);	// ������ǰλ�����λ�Ƶľ������꣬
private:
	int* mine_area = nullptr;
	int* num_area = nullptr;
	int* open_area = nullptr;
	int* flag_area = nullptr;
	int width = 0;
	int height = 0;
	int mine_num = 0;
};