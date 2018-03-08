#include <iostream>

using namespace std;

void main()
{
	for(int i = 1; i <= 5; i++)
	{			
		for(int j = 0; j < i; j++)
			printf("%d ",j+1);
		printf("\n");
	}
	system("pause");
}