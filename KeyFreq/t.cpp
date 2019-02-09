#include<windows.h>
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<algorithm>
#define ShowNum 10//ÿ����ʾ��Ƶ������
using namespace std;
typedef map< int, map<string, int> > MapType;//��������
typedef map< string, int > InfoType;//��ӳ������
typedef vector< pair< string, int > > tVec;//������vector
MapType KeyMap;//(������룬(������Ƶ��))
MapType::iterator KeyMapIt;//���������
InfoType::iterator KeyIt;//��������

int InitKeyMap(void)
{//��ʼ������
	KeyMap[8]["Backspace"] = 0; KeyMap[9]["Tab"] = 0; KeyMap[12]["Clear"] = 0; KeyMap[13]["Enter"] = 0;
	KeyMap[16]["Shift"] = 0; KeyMap[17]["Ctrl"] = 0; KeyMap[18]["Alt"] = 0; KeyMap[19]["Pause"] = 0;
	KeyMap[20]["Caps Lock"] = 0; KeyMap[27]["Esc"] = 0; KeyMap[32]["Space"] = 0; KeyMap[33]["Page Up"] = 0;
	KeyMap[34]["Page Down"] = 0; KeyMap[35]["End"] = 0; KeyMap[36]["Home"] = 0; KeyMap[37]["Left"] = 0;
	KeyMap[38]["Up"] = 0; KeyMap[39]["Right"] = 0; KeyMap[40]["Down"] = 0; KeyMap[41]["Select"] = 0;
	KeyMap[42]["Print"] = 0; KeyMap[43]["Execute"] = 0; KeyMap[44]["Snapshot"] = 0; KeyMap[45]["Insert"] = 0;
	KeyMap[46]["Delete"] = 0; KeyMap[47]["Help"] = 0;
	for (int i = 48; i < 58; i++)
		KeyMap[i][to_string(i - 48)] = 0;//0~9���ּ���ֵ
	for (int i = 65; i < 91; i++)
	{
		string ts;
		ts = (char)i;
		KeyMap[i][ts] = 0;//��ĸ����ֵ
	}
	KeyMap[91]["Left Win"] = 0; KeyMap[92]["Right Win"] = 0; KeyMap[93]["APPS"] = 0;
	for (int i = 96; i < 106; i++)
		KeyMap[i]["С���� " + to_string(i - 96)] = 0;//0~9С�������ּ���ֵ
	KeyMap[106]["С���� *"] = 0; KeyMap[107]["С���� +"] = 0; KeyMap[108]["С���� Enter"] = 0; KeyMap[109]["С���� -"] = 0;
	KeyMap[110]["С���� ."] = 0; KeyMap[111]["С���� /"] = 0;
	for (int i = 112; i < 124; i++)
		KeyMap[i]["F" + to_string(i - 111)] = 0;//���ܼ���ֵ
	KeyMap[144]["Num Lock"] = 0; KeyMap[145]["Scroll"] = 0; KeyMap[160]["Left Shift"] = 0; KeyMap[161]["Right Shift"] = 0;
	KeyMap[162]["Left Control"] = 0; KeyMap[163]["Right Control"] = 0; KeyMap[164]["Left Menu"] = 0; KeyMap[165]["Right Menu"] = 0;
	KeyMap[173]["����"] = 0; KeyMap[174]["����-"] = 0; KeyMap[175]["����+"] = 0; KeyMap[186]["; :"] = 0;
	KeyMap[187]["= +"] = 0; KeyMap[189]["- _"] = 0; KeyMap[191]["/ ?"] = 0; KeyMap[192]["` ~"] = 0;
	KeyMap[219]["[ {"] = 0; KeyMap[220]["\\ |"] = 0; KeyMap[221]["] }"] = 0; KeyMap[222]["' \""] = 0;
	KeyMap[188]["< ,"] = 0; KeyMap[190]["> ."] = 0; KeyMap[226]["\\ |"] = 0;

	return 0;
}
bool LessCmp(const pair<string, int> &p1, const pair<string, int> &p2)
{//�Ӵ�С
	return p1.second > p2.second;
}
int ShowFreq(int num = ShowNum)
{//��ʾ��λƵ����Ĭ����ʾǰ5����ߵİ���
	MapType::iterator tKeyMapIt;//��ʱ���������
	tVec tKeys;//������vector

	for (MapType::iterator tKeyMapIt = KeyMap.begin(); tKeyMapIt != KeyMap.end(); tKeyMapIt++)
	{//��������ʱ����Ϣ��������
		tKeys.push_back(*tKeyMapIt->second.begin());
	}
	sort(tKeys.begin(), tKeys.end(), LessCmp);//��˳������
	int MaxFreq = tKeys.begin()->second;//����λƵ��
	system("cls");
	for (tVec::iterator tKeysIt = tKeys.begin(); num > 0 && tKeysIt != tKeys.end(); num--, tKeysIt++)
	{//�����Ƶ��
		int t = tKeysIt->second * 20 / MaxFreq;
		cout.width(15);
		cout.setf(ios::right);
		cout << tKeysIt->first << "\t" << tKeysIt->second << "\t";
		while (t--)
			cout << "��";
		cout << endl;
	}

	return 0;
}
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{//�����ص�����
	switch (wParam)
	{
	case WM_KEYUP:case WM_SYSKEYUP://����̧��ʱ
		DWORD vkC = ((PKBDLLHOOKSTRUCT)lParam)->vkCode;//�������
		KeyMapIt = KeyMap.find(vkC);//VKcode���Ҽ���
		if (KeyMapIt != KeyMap.end())
		{//�ҵ���������
			KeyIt = KeyMapIt->second.begin();
			KeyIt->second++;
		}
		else
			KeyMap[vkC]["UnNamed Key(vkCode)" + to_string(vkC)] = 1;//д�����
		ShowFreq();//��ʾ��Ƶ
		break;
	}

	return 0;
}

int main()
{
	MSG msg;
	HHOOK retk = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)&LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

	InitKeyMap();//��ʼ������

	if (retk != NULL)
		cout << "��Ƶ��¼��ʼ����������" << endl;
	else
	{
		cout << "���Ĺҹ�ʧ�ܣ�" << endl;
		return 1;
	}
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
