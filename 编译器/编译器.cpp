#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include <sstream>
#include <fstream>
#include "符号表结构.h"

using namespace std;



int run()
{
	int k;
	while (1)
	{
		system("cls");
		print_begin();
		cin >> k;
		while (!(k == 1 || k == 2 || k == 3 || k == 4 || k == 5))
		{
			cout << "输入为非提供的选项，请重新输入（1 - 5）：";
			cin >> k;
		}
		switch (k)
		{
		case 1:
			scan_run();
			system("pause");
			break;
		case 2:
			Analysis_run();
			system("pause");
			break;
		case 3:
			print_QT();
			system("pause");
			break;
		case 4:
			print_fuhao();
			system("pause");
			break;
		default:
			break;
		}
		if (k == 5)
			break;
	}
	return 1;

}

void print_begin()
{
	cout << "本程序可提供的操作：" << endl;
	cout << "1、词法分析（token序列生成）" << endl;
	cout << "2、语义分析（检验文法合格性）" << endl;
	cout << "3、中间代码生成（四元式的生成）" << endl;
	cout << "4、各符号表内容" << endl;
	cout << "5、退出" << endl;
	cout << "请输入你的操作（1 - 5）：" ;
}

int main()
{
	autoAddType();
	run();
	
	return 0;

}

