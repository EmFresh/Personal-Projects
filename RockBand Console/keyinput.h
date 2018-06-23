#pragma once
#include <Windows.h>
#include <unordered_map>
#include <vector>
#include <string>

/*----User defined classes----*/
#define amount (wait2 == 0 ? 250 : 25)

class keyinput
{
private:
	std::unordered_map<int, bool>enter;
	std::vector<char>
		numShiftKeys {')','!','@','#','$','%','^','&','*','('},
		symbalKeys {';','/','`','[','\\',']','\'','=',',','-','.'},
		symbalShiftKeys {':','?','~','{','|','}','\"','+','<','_','>'},
		operators {'*','+','-','.','/'};
	short count, count2, wait, wait2;
	std::string typing;
public:

	void clearType()
	{
		typing = "";
	}

	const char* type()
	{
		//OutputDebugStringA((std::to_string(count) + "\n").c_str());

		if(count2++ == wait2)
		{
			count2 = 0;

			//nums & numpad nums
			for(short a = 0x30; a - 0x30 < 10; a++)
				if(press(a) || (press(a + 48) && GetKeyState(VK_NUMLOCK)))
					if(!press(VK_SHIFT))
					{
						wait2 = amount;
						return (typing += a).c_str();
					} else
					{
						wait2 = amount;
						return (typing += numShiftKeys[a - 0x30]).c_str();
					}

			//numpad operators
					for(short a = 0x6A; a - 0x6A < 2; a++)
						if(press(a))
							if(GetKeyState(VK_NUMLOCK))
							{
								wait2 = amount;
								return (typing += operators[a - 0x6A]).c_str();
							}

					for(short a = 0x6D; a - 0x6D < 3; a++)
						if(press(a))
							if(GetKeyState(VK_NUMLOCK))
							{
								wait2 = amount;
								return (typing += operators[a - 0x6D + 2]).c_str();
							}

					//letters
					for(short a = 0x41; a - 0x41 < 26; a++)
						if(press(a))
						{
							if(press(VK_SHIFT) || GetKeyState(VK_CAPITAL))
							{
								wait2 = amount;
								return (typing += a).c_str();
							} else
							{
								wait2 = amount;
								return (typing += a + 32).c_str();
							}
						}

					//OEM (dual use buttons)		
					if(press(0xBA))
					{
						if(press(VK_SHIFT))
						{
							wait2 = amount;
							return (typing += symbalShiftKeys[0]).c_str();
						} else
						{
							wait2 = amount;
							return (typing += symbalKeys[0]).c_str();
						}
					}

					for(short a = 0xBF; a - 0xBF < 2; a++)
						if(press(a))
						{
							if(press(VK_SHIFT))
							{
								wait2 = amount;
								return (typing += symbalShiftKeys[a - 0xBF + 1]).c_str();
							} else
							{
								wait2 = amount;
								return (typing += symbalKeys[a - 0xBF + 1]).c_str();
							}
						}

					for(short a = 0xDB; a - 0xDB < 4; a++)
						if(press(a))
						{
							if(press(VK_SHIFT))
							{
								wait2 = amount;
								return (typing += symbalShiftKeys[a - 0xDB + 3]).c_str();
							} else
							{
								wait2 = amount;
								return (typing += symbalKeys[a - 0xDB + 3]).c_str();
							}
						}

					for(short a = 0xBB; a - 0xBB < 4; a++)
						if(press(a))
						{
							if(press(VK_SHIFT))
							{
								wait2 = amount;
								return (typing += symbalShiftKeys[a - 0XBB + 7]).c_str();
							} else
							{
								wait2 = amount;
								return (typing += symbalKeys[a - 0XBB + 7]).c_str();
							}
						}

					wait2 = 0;
		}

		if(count++ == wait)
		{
			count = 0;
			if(!typing.empty() && press(VK_BACK))
			{
				wait = (wait == 0 ? 200 : 25);
				return (typing = typing.substr(0, typing.size() - 1)).c_str();
			} else if(stroke(VK_SPACE))
				return (typing += ' ').c_str();
			wait = 0;
		}

		return typing.c_str();
	}

	/*
	 bool stroke(int key);
	 * key - The key which is pressed. You can either
	 use VK_KEYS (i.e. VK_RIGHT) or characters
	 (Note: characters must be in uppercase)

	 to be checked if key is pressed and then released
	*/
	bool stroke(int key)
	{
		enter.insert({key, false});

		if(GetAsyncKeyState(key))
			enter[key] = true;

		else if(enter[key] && !GetAsyncKeyState(key))
		{
			enter[key] = false;
			return true;
		}
		return false;
	}

	/*
	bool press(int key)
	* key - The key which is pressed. You can either
	use VK_KEYS (i.e. VK_RIGHT) or characters
	(Note: characters must be in uppercase) to be
	checked if key is pressed
	*/
	bool press(int key)
	{
		return GetAsyncKeyState(key);
	}

	/*
	bool release(int key)
	* key - The key which is pressed. You can either
	use VK_KEYS (i.e. VK_RIGHT) or characters
	(Note: characters must be in uppercase) to be
	checked if key is released
	*/
	bool release(int key)
	{
		return !GetAsyncKeyState(key);
	}
};
