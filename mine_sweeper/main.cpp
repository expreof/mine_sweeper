#include <iostream>
#include <chrono>
#include "mine_grid.h"

using namespace std;
int main()
{
	mine_grid heaven(10, 10, 10);
	heaven.arrange_mine();	// ±ØĞë²¼ÖÃÀ×Çø
	heaven.display();
	auto tStart = chrono::steady_clock::now();
	while (true)
	{
		heaven.solve();
		//heaven.display();
		if (heaven.iswin())
		{
			cout << "WIN" << endl;
			break;
		}
		if (heaven.isBOOM())
		{
			cout << "LOSE" << endl;
			break;
		}
	}
	auto tEnd = chrono::steady_clock::now();
	heaven.displayBOOM();
	std::chrono::duration<double> diff = tEnd - tStart;
	cout << diff.count() << " s"<< endl;
	return 0;
}