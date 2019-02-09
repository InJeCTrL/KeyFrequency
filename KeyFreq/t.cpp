#include<windows.h>
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<algorithm>
#define ShowNum 10//每次显示的频数个数
using namespace std;
typedef map< int, map<string, int> > MapType;//键表类型
typedef map< string, int > InfoType;//键映射类型
typedef vector< pair< string, int > > tVec;//排序用vector
MapType KeyMap;//(虚拟键码，(键名，频数))
MapType::iterator KeyMapIt;//键表迭代器
InfoType::iterator KeyIt;//键迭代器

int InitKeyMap(void)
{//初始化键表
	KeyMap[8]["Backspace"] = 0; KeyMap[9]["Tab"] = 0; KeyMap[12]["Clear"] = 0; KeyMap[13]["Enter"] = 0;
	KeyMap[16]["Shift"] = 0; KeyMap[17]["Ctrl"] = 0; KeyMap[18]["Alt"] = 0; KeyMap[19]["Pause"] = 0;
	KeyMap[20]["Caps Lock"] = 0; KeyMap[27]["Esc"] = 0; KeyMap[32]["Space"] = 0; KeyMap[33]["Page Up"] = 0;
	KeyMap[34]["Page Down"] = 0; KeyMap[35]["End"] = 0; KeyMap[36]["Home"] = 0; KeyMap[37]["Left"] = 0;
	KeyMap[38]["Up"] = 0; KeyMap[39]["Right"] = 0; KeyMap[40]["Down"] = 0; KeyMap[41]["Select"] = 0;
	KeyMap[42]["Print"] = 0; KeyMap[43]["Execute"] = 0; KeyMap[44]["Snapshot"] = 0; KeyMap[45]["Insert"] = 0;
	KeyMap[46]["Delete"] = 0; KeyMap[47]["Help"] = 0;
	for (int i = 48; i < 58; i++)
		KeyMap[i][to_string(i - 48)] = 0;//0~9数字键赋值
	for (int i = 65; i < 91; i++)
	{
		string ts;
		ts = (char)i;
		KeyMap[i][ts] = 0;//字母键赋值
	}
	KeyMap[91]["Left Win"] = 0; KeyMap[92]["Right Win"] = 0; KeyMap[93]["APPS"] = 0;
	for (int i = 96; i < 106; i++)
		KeyMap[i]["小键盘 " + to_string(i - 96)] = 0;//0~9小键盘数字键赋值
	KeyMap[106]["小键盘 *"] = 0; KeyMap[107]["小键盘 +"] = 0; KeyMap[108]["小键盘 Enter"] = 0; KeyMap[109]["小键盘 -"] = 0;
	KeyMap[110]["小键盘 ."] = 0; KeyMap[111]["小键盘 /"] = 0;
	for (int i = 112; i < 124; i++)
		KeyMap[i]["F" + to_string(i - 111)] = 0;//功能键赋值
	KeyMap[144]["Num Lock"] = 0; KeyMap[145]["Scroll"] = 0; KeyMap[160]["Left Shift"] = 0; KeyMap[161]["Right Shift"] = 0;
	KeyMap[162]["Left Control"] = 0; KeyMap[163]["Right Control"] = 0; KeyMap[164]["Left Menu"] = 0; KeyMap[165]["Right Menu"] = 0;
	KeyMap[173]["静音"] = 0; KeyMap[174]["音量-"] = 0; KeyMap[175]["音量+"] = 0; KeyMap[186]["; :"] = 0;
	KeyMap[187]["= +"] = 0; KeyMap[189]["- _"] = 0; KeyMap[191]["/ ?"] = 0; KeyMap[192]["` ~"] = 0;
	KeyMap[219]["[ {"] = 0; KeyMap[220]["\\ |"] = 0; KeyMap[221]["] }"] = 0; KeyMap[222]["' \""] = 0;
	KeyMap[188]["< ,"] = 0; KeyMap[190]["> ."] = 0; KeyMap[226]["\\ |"] = 0;

	return 0;
}
bool LessCmp(const pair<string, int> &p1, const pair<string, int> &p2)
{//从大到小
	return p1.second > p2.second;
}
int ShowFreq(int num = ShowNum)
{//显示键位频数，默认显示前5个最高的按键
	MapType::iterator tKeyMapIt;//临时键表迭代器
	tVec tKeys;//键表倒入vector

	for (MapType::iterator tKeyMapIt = KeyMap.begin(); tKeyMapIt != KeyMap.end(); tKeyMapIt++)
	{//键表导入临时键信息表以排序
		tKeys.push_back(*tKeyMapIt->second.begin());
	}
	sort(tKeys.begin(), tKeys.end(), LessCmp);//非顺序排序
	int MaxFreq = tKeys.begin()->second;//最大键位频数
	system("cls");
	for (tVec::iterator tKeysIt = tKeys.begin(); num > 0 && tKeysIt != tKeys.end(); num--, tKeysIt++)
	{//输出键频表
		int t = tKeysIt->second * 20 / MaxFreq;
		cout.width(15);
		cout.setf(ios::right);
		cout << tKeysIt->first << "\t" << tKeysIt->second << "\t";
		while (t--)
			cout << "█";
		cout << endl;
	}

	return 0;
}
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{//按键回调函数
	switch (wParam)
	{
	case WM_KEYUP:case WM_SYSKEYUP://按键抬起时
		DWORD vkC = ((PKBDLLHOOKSTRUCT)lParam)->vkCode;//虚拟键码
		KeyMapIt = KeyMap.find(vkC);//VKcode查找键表
		if (KeyMapIt != KeyMap.end())
		{//找到已命名键
			KeyIt = KeyMapIt->second.begin();
			KeyIt->second++;
		}
		else
			KeyMap[vkC]["UnNamed Key(vkCode)" + to_string(vkC)] = 1;//写入键表
		ShowFreq();//显示键频
		break;
	}

	return 0;
}

int main()
{
	MSG msg;
	HHOOK retk = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)&LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

	InitKeyMap();//初始化键表

	if (retk != NULL)
		cout << "键频记录开始正常工作！" << endl;
	else
	{
		cout << "核心挂钩失败！" << endl;
		return 1;
	}
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
