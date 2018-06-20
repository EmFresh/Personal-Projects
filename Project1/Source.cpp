#include <vector>
#include <iostream>

using namespace std;

void main()
{
	vector<string> choice {"popeys","Swiss","bulk barn","east side"};
	srand(time());
	printf("%s", choice[rand() % 4].c_str())
		;
	system("pause");
}