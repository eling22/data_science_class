// hw2_vs.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "fpgrowth.h"

using namespace std;

int main(void)
{
	//redirect stdin to 1.in file
	fstream input_file;
	input_file.open("test.in", fstream::in);
	//input_file.open("sample2.in", fstream::in);
	vector<vector<int>> data;

	string line;
	int line_num = 0;
	while (getline(input_file, line)) {
		line_num++;
		vector<int> arr;
		istringstream iss(line);
		int num;
		while (iss >> num) {
			arr.push_back(num);
			char con;
			iss >> con;
		}
		data.push_back(arr);
	}

	FPGrowth fpgr(data,line_num,0.1);
	//fpgr.ShowOriginData();
	//fpgr.SetVerticalTree();
	//fpgr.ShowTree();
	//fpgr.Show_htable_map();
	//fpgr.ShowOneNumFrequence();
	//fpgr.ShowTree();

	cout << "support min num:" << line_num*0.1 << endl;
	while (1) {
		cout << "enter num:";
		int num;
		cin >> num;
		if (num == -1) { break; }
		fpgr.Show(num);
	}

	return 0;
}



// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的秘訣: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案

//pc
//C:\Users\eling\Desktop\資料科學\data_science_class\assignment_2\hw2_vs\hw2_vs

//mingw32-make -f makefile