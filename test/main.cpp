#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<int> fff(10);
	cout << fff.size() << endl;
	fff.push_back(22);
	cout << fff.size() << endl;
	for (auto& s : fff)
		cout << s << ' ';
	return 0;
}