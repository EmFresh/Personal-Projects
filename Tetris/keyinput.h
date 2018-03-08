#pragma once
#include <Windows.h>
#include <unordered_map>

/*----User defined classes----*/
class keyinput
{
public:
	std::unordered_map<int, bool>enter;
	//bool enter = false;
	bool stroke(int key)
	{
		enter.insert(std::pair<int, bool>(key, false));

		if (GetAsyncKeyState(key))
			enter[key] = true;
		else if (enter[key] && !GetAsyncKeyState(key))
		{
			enter[key] = false;
			return true;
		}
		return false;
	}

	bool press(int key)
	{
		return GetAsyncKeyState(key);
	}
	bool release(int key)
	{
		return !GetAsyncKeyState(key);
	}
};