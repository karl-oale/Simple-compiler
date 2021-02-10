#include <iostream>
#include <stdio.h>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include "符号表结构.h"

using namespace std;


vector<QT_TOKEN> QT_TOKEN_List;



TOKEN now_Token;	//当前的读取token
int flag = 1;			//检查语法分析是否正确的标志，默认为1，分析错误则变为0
int pp = 0;			//now_token读写头


stack<TOKEN> SYN;	//符号栈
stack<TOKEN> SEM;	//对象栈
int t = 1;			//四元式中间变量下标
int now_level = 20;		//现在的层
int now_loca = 0;		//现在的位置

TOKEN TOKEN_t;		//临时token
SYNBL SYNBL_t;		//临时主表项
int T_t;			//临时整型数
int Length_Array;	//数组赋值的下标
int fn = 0;			//函数的地址表增值
int FM[50];		//保存各函数起始终止的四元式下标
int fc = 0;			//统计形参个数
int fs = 0;			//统计实参个数
int fc_t = 0;		//保存形参个数临时变量
//now_Token指向下一个token
void readToken()
{
	//cout << Word_List.at(pp).word << endl;
	if (pp < Word_List.size())
	{
		do
		{
			now_Token = find_token(Word_List.at(pp).word, Word_List.at(pp).End_state);
			pp++;
		} while (now_Token.num == -2 && now_Token.type_id == -2);
	}
	else
	{
		now_Token = { -1,-1 };
	}

}


//程序开始
void Analysis_run()
{
	string sss;
	read_data("input.txt", sss);
	printf_data(sss);
	readToken();
	Source();
	if (flag == 1)
	{
		cout << "语法分析完成，没有错误！" << endl;
	}
	else if (flag == 0)
	{
		cout << "语法分析过程中存在错误！" << endl;
	}
	else
	{
		cout << "错误标志flag结果错误！" << endl;
	}
}

//<源程序>→<开始>
int Source()
{
	
	begin();
	return 1;
}

//<开始>→<数据类型><主函数>|<数据类型><外部定义>
int begin()
{
	if (now_Token.type_id == 0 && (KT[now_Token.num] == "int" || KT[now_Token.num] == "char" || KT[now_Token.num] == "float" || KT[now_Token.num] == "string" || KT[now_Token.num] == "bool" || KT[now_Token.num] == "string" || KT[now_Token.num] == "bool"))
	{
		SYN.push(now_Token);
		readToken();
		if (now_Token.type_id == 0 && KT[now_Token.num] == "main")
		{
			SYN.pop();
			main_function();
		}
		else if (now_Token.type_id == 1)
		{
			fn++;
			now_level = 20 + fn;
			now_loca = 0;
			if (fn == 1)
			{
				FM[0] = 0;
			}
			else
			{
				FM[(2 * fn) - 2] = QT_TOKEN_List.size();
			}
			External();
		}
		else
		{
			cout << "函数名缺少或函数名定义错误！" << endl;
			flag = 0;
		}
	}
	else
	{
		cout << "函数缺少类型符！" << endl;
		flag = 0;
	}
	return 1;
}

//<主函数>→main ( ) {<复合语句>}
int main_function()
{
	if (now_Token.type_id == 0 && KT[now_Token.num] == "main")
	{
		now_level = 20;
		now_loca = 0;
		SEM.push(now_Token);
		QT_TOKEN_List.push_back({ "START",{-1,-1},{-1,-1},now_Token });
		readToken();
		if (now_Token.type_id == 2 && PT[now_Token.num] == "(")
		{
			readToken();
			if (now_Token.type_id == 2 && PT[now_Token.num] == ")")
			{
				readToken();
				if (now_Token.type_id == 2 && PT[now_Token.num] == "{")
				{

					readToken();
					Compound();
					if (now_Token.type_id == 2 && PT[now_Token.num] == "}")
					{
						QT_TOKEN_List.push_back({ "END",{-1,-1},{-1,-1},SEM.top() });
						SEM.pop();
						return 1;
					}
					else
					{
						cout << "主函数定义缺少 } 右花括号！" << endl;
						flag = 0;
					}
				}
				else
				{
					cout << "主函数定义缺少 } 右花括号！" << endl;
					flag = 0;
				}

			}
			else
			{
				cout << "主函数定义中缺少 ） 右括号！" << endl;
				flag = 0;
			}
		}
		else
		{
			cout << "主函数定义中缺少 ( 左括号！" << endl;
			flag = 0;
		}
	}
	return 1;
}

//<外部定义>→<函数头>{<复合语句>}<开始>
int External()
{
	function_header();
	if (now_Token.type_id == 2 && PT[now_Token.num] == "{")
	{

		readToken();
		Compound();
		if (now_Token.type_id == 2 && PT[now_Token.num] == "}")
		{
			QT_TOKEN_List.push_back({ "END",{-1,-1},{-1,-1},SEM.top() });
			SEM.pop();
			FM[(2 * fn) - 1] = QT_TOKEN_List.size() - 1;
			readToken();
			begin();
		}
		else
		{
			cout << "函数定义缺少 } 右花括号！" << endl;
			flag = 0;
		}
	}
	else
	{
		cout << "外部定义存在错误！" << endl;
		flag = 0;
	}
	return 1;
}

//<函数头>→<标识符>(<形参>)
int function_header()
{
	if (now_Token.type_id == 1)
	{
		
		QT_TOKEN_List.push_back({ "START",{-1,-1},{-1,-1},now_Token });
		TOKEN_t = Is_Exist(IT.at(now_Token.num), 20);
		if (TOKEN_t.num == -1)
		{
			//当前标识符进主表			 name		type(类型)	CAT(种类）ADDR(地址)
			SYNBL_List.push_back({ IT.at(now_Token.num), { 8,declar_type(KT[SYN.top().num]) }, "f", {20,-fn} });
			SEM.push({7,static_cast<int>(SYNBL_List.size()-1) });
			PFINFL_List.push_back({ now_level });
			//SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
			SYN.pop();

		}
		else
		{
			SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
			cout << IT.at(now_Token.num) << "  函数重复定义" << endl;
			flag = 0;
		}

		readToken();
		if (now_Token.type_id == 2 && PT[now_Token.num] == "(")
		{
			readToken();
			fc = 0;		//形参个数统计
			Formal();
			if (now_Token.type_id == 2 && PT[now_Token.num] == ")")
			{
				readToken();
				PFINFL_List.at(fn - 1).FN = fc;
				return 1;
			}
			else
			{
				cout << "函数定义中缺少 ） 右括号！" << endl;
				flag = 0;
			}
		}
		else
		{
			cout << "函数定义中缺少 ( 左括号！" << endl;
			flag = 0;
		}
	}

	return 1;
}


//<形参>→<数据类型><标识符>|<数据类型><标识符>,<形参>
int Formal()
{
	if (now_Token.type_id == 0 && (KT[now_Token.num] == "int" || KT[now_Token.num] == "char" || KT[now_Token.num] == "float" || KT[now_Token.num] == "string" || KT[now_Token.num] == "bool"))
	{
		SYN.push(now_Token);
		readToken();
		if (now_Token.type_id == 1)
		{
			fc++;
			TOKEN_t = Is_Exist(IT.at(now_Token.num), now_level);
			if (TOKEN_t.num == -1)
			{
				//当前标识符进主表		   name						type（类型）				CAT(种类）		ADDR（地址）
				SYNBL_List.push_back({ IT.at(now_Token.num), { 8,declar_type(KT[SYN.top().num]) }, "v", {now_level,now_loca} });
				SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
				PFINFL_List.at(fn - 1).parameter[fc - 1] = SEM.top();
				now_loca += Length_type(SEM.top());
			}
			else
			{
				SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
				cout << IT.at(now_Token.num) << "  变量重复定义" << endl;
				flag = 0;
			}
			SYN.pop();
			SEM.pop();
			readToken();
			if (now_Token.type_id == 2 && PT[now_Token.num] == ",")
			{

				readToken();
				Formal();
			}
		}
		else
		{
			cout << "定义的形参为非标识符！" << endl;
			flag = 0;
		}
	}
	else
	{
		cout << "数据类型定义错误！" << endl;
		flag = 0;
	}
	return 1;
}


//<复合语句>→<变量声明><复合语句> | <赋值、函数调用语句><复合语句> | <选择语句><复合语句> | <循环语句><复合语句> | <返回语句><复合语句> | ε
int Compound()
{
	if (now_Token.type_id == 0 && KT[now_Token.num] == "if")
	{
		Select();
		Compound();
	}
	else if (now_Token.type_id == 0 && KT[now_Token.num] == "while")
	{
		Cycle();
		Compound();
	}
	else if (now_Token.type_id == 0 && KT[now_Token.num] == "return")
	{
		Back();
		Compound();
	}
	else if (now_Token.type_id == 0 && (KT[now_Token.num] == "int" || KT[now_Token.num] == "char" || KT[now_Token.num] == "float" || KT[now_Token.num] == "string" || KT[now_Token.num] == "bool"))
	{
		Variable();
		Compound();
	}
	else if (now_Token.type_id == 1)
	{
		Call_and_Assignment();
		Compound();
	}
	return 1;
}



//<变量声明>→<数据类型><标识符>；|<数据类型><标识符>，<标识符列表>；|<数据类型><标识符><数组列表>;
int Variable()
{
	if (now_Token.type_id == 0 && (KT[now_Token.num] == "int" || KT[now_Token.num] == "char" || KT[now_Token.num] == "float" || KT[now_Token.num] == "string" || KT[now_Token.num] == "bool"))
	{
		SYN.push(now_Token);
		readToken();
		if (now_Token.type_id == 1)
		{
			TOKEN_t = Is_Exist(IT.at(now_Token.num), now_level);
			if (TOKEN_t.num == -1)
			{
				//当前标识符进主表		   name						type（类型）				CAT(种类）		ADDR（地址）
				SYNBL_List.push_back({ IT.at(now_Token.num), { 8,declar_type(KT[SYN.top().num]) }, "v", {now_level,now_loca} });
				SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
				now_loca += Length_type(SEM.top());
			}
			else
			{
				SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
				cout << IT.at(now_Token.num) << "  变量重复定义" << endl;
				flag = 0;
			}
			readToken();
			if (now_Token.type_id == 2 && PT[now_Token.num] == ";")
			{
				SEM.pop();
				SYN.pop();
				readToken();
				return 1;
			}
			else if (now_Token.type_id == 2 && PT[now_Token.num] == ",")
			{
				SEM.pop();
				readToken();
				dentifier();
				if (now_Token.type_id == 2 && PT[now_Token.num] == ";")
				{
					SEM.pop();
					SYN.pop();
					readToken();
					return 1;
				}
				else
				{
					cout << "变量声明缺少 ; 分号！" << endl;
					flag = 0;
				}
			}
			else if (now_Token.type_id == 2 && PT[now_Token.num] == "[")
			{
				Array();
				if (now_Token.type_id == 2 && PT[now_Token.num] == ";")
				{
					SEM.pop();
					SYN.pop();
					readToken();
					return 1;
				}
				else
				{
					cout << "变量声明缺少 ; 分号！" << endl;
					flag = 0;
				}
			}
			else
			{
				cout << "变量声明缺少 ; 分号！" << endl;
				flag = 0;
			}

		}

	}
	else
	{
		cout << "变量声明错误！" << endl;
		flag = 0;
	}
	return 1;
}

//<标识符列表>→<标识符> | <标识符>,<标识符列表>
int dentifier()
{
	if (now_Token.type_id == 1)
	{
		TOKEN_t = Is_Exist(IT.at(now_Token.num), now_level);
		if (TOKEN_t.num == -1)
		{
			//当前标识符进主表		   name						type（类型）				CAT(种类）		ADDR（地址）
			SYNBL_List.push_back({ IT.at(now_Token.num), { 8,declar_type(KT[SYN.top().num]) }, "v", {now_level,now_loca} });
			SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
			now_loca += Length_type(SEM.top());
		}
		else
		{
			SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
			cout << IT.at(now_Token.num) << "  变量重复定义" << endl;
			flag = 0;
		}
		readToken();
		if (now_Token.type_id == 2 && PT[now_Token.num] == ",")
		{
			SEM.pop();
			readToken();
			dentifier();
		}
	}
	else
	{
		cout << "标识符定义失败！" << endl;
		flag = 0;
	}
	return 1;
}

//<数组列表>→[<整型常量>] | ，<标识符>[<整型常量>]<数组列表>
int Array()
{
	if (now_Token.type_id == 2 && PT[now_Token.num] == "[")
	{
		readToken();
		if (now_Token.type_id == 3)
		{
			//退回原来认为非数组的长度
			now_loca = now_loca - Length_type(SYNBL_List.back().type);
			SYNBL_t = SYNBL_List.back();

			//数组的长度
			T_t = atoi(IC.at(now_Token.num).c_str()) * (Length_type(SYNBL_t.type));
			//将当前数组数据添加到数组表中
			AINFL_List.push_back({ 0, atoi(IC.at(now_Token.num).c_str()),SYNBL_t.type,T_t });
			//将当前类型数据添加到类型表中
			TYPEL_List.push_back({ 'a',{9,static_cast<int>(AINFL_List.size() - 1)} });

			//将原来认为非数组的主表项的类型指向改为指向数组
			SYNBL_List.at(SYNBL_List.size() - 1).type.num = TYPEL_List.size() - 1;

			//调整地址的偏移地址
			now_loca = now_loca + T_t;

			readToken();
			if (now_Token.type_id == 2 && PT[now_Token.num] == "]")
			{
				readToken();
				if (now_Token.type_id == 2 && PT[now_Token.num] == "=")
				{
					readToken();
					Array_assignment();
				}
				return 1;
			}
			else
			{
				cout << "数组定义缺少 ] 右方括号！" << endl;
				flag = 0;
			}
		}

	}
	else if (now_Token.type_id == 2 && PT[now_Token.num] == ",")
	{
		SEM.pop();
		readToken();
		if (now_Token.type_id == 1)
		{
			TOKEN_t = Is_Exist(IT.at(now_Token.num), now_level);
			if (TOKEN_t.num == -1)
			{
				//当前标识符进主表		   name						type（类型）				CAT(种类）		ADDR（地址）
				SYNBL_List.push_back({ IT.at(now_Token.num), { 8,declar_type(KT[SYN.top().num]) }, "v", {now_level,now_loca} });
				SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });

			}
			else
			{
				SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
				cout << IT.at(now_Token.num) << "  变量重复定义" << endl;
				flag = 0;
			}
			readToken();
			if (now_Token.type_id == 2 && PT[now_Token.num] == "[")
			{
				readToken();
				//Arithmetic_Expression();
				if (now_Token.type_id == 3)
				{
					//退回原来认为非数组的长度
					now_loca = now_loca - Length_type(SYNBL_List.back().type);
					SYNBL_t = SYNBL_List.back();

					//数组的长度
					T_t = atoi(IC.at(now_Token.num).c_str()) * (Length_type(SYNBL_t.type));
					//将当前数组数据添加到数组表中
					AINFL_List.push_back({ 0, atoi(IC.at(now_Token.num).c_str()),SYNBL_t.type,T_t });
					//将当前类型数据添加到类型表中
					TYPEL_List.push_back({ 'a',{9,static_cast<int>(AINFL_List.size() - 1)} });

					//将原来认为非数组的主表项的类型指向改为指向数组
					SYNBL_List.at(SYNBL_List.size() - 1).type.num = TYPEL_List.size() - 1;

					//调整地址的偏移地址
					now_loca = now_loca + T_t;

					readToken();
					if (now_Token.type_id == 2 && PT[now_Token.num] == "]")
					{
						readToken();
						if (now_Token.type_id == 2 && PT[now_Token.num] == "=")
						{
							readToken();
							Array_assignment();
						}
						if (now_Token.type_id == 2 && PT[now_Token.num] == ",")
						{
							SEM.pop();
							Array();
						}
						else
						{
							return 1;
						}
					}
					else
					{
						cout << "数组定义缺少 ] 右方括号！" << endl;
						flag = 0;
					}
				}
				else
				{
					return 1;
					cout << "数组定义[]内非整型常量！" << endl;
					flag = 0;
				}
			}
			else
			{
				cout << "数组定义缺少 [ 左方括号！" << endl;
				flag = 0;
			}
		}
	}
	return 1;
}


//<数组赋值>→{<数字列表>} 
int Array_assignment()
{

	if (now_Token.type_id == 2 && PT[now_Token.num] == "{")
	{
		readToken();
		Length_Array = 0;
		Digital_list();
		if (now_Token.type_id == 2 && PT[now_Token.num] == "}")
		{
			readToken();
			return 1;
		}
	}
	else
	{
		cout << "数组初始化右部不正确！" << endl;
		flag = 0;
	}
	return 1;
}

//<数字列表>→<整型常量>|，<整型常量>|<实型常量>|，<实型常量>
int Digital_list()
{
	if (now_Token.type_id == 3 || now_Token.type_id == 4)
	{
		SEM.push(now_Token);
		action_Array();
		readToken();
		if (now_Token.type_id == 2 && PT[now_Token.num] == ",")
		{
			readToken();
			Digital_list();
		}
	}
	else
	{
		cout << "数组赋值{ }里非常数！" << endl;
		flag = 0;
	}
	return 1;
}



//<选择语句>→if(<表达式>){<复合语句>}【else(<表达式>){<复合语句>}】
int Select()
{
	if (now_Token.type_id == 0 && KT[now_Token.num] == "if")
	{
		readToken();
		if (now_Token.type_id == 2 && PT[now_Token.num] == "(")
		{
			readToken();
			Expression();

			//动作开始
				//生成四元式
			if (Get_Type(SEM.top()) == 'b' || Get_Type(SEM.top()) == 'i' || Get_Type(SEM.top()) == 'r') {
				QT_TOKEN_List.push_back({ "if", SEM.top(), { -1, -1 }, { -1, -1 } });

				//弹出对象栈中保存算术表达式的Token
				SEM.pop();
				//then语句地址的Token入对象栈，为回填做准备
				SEM.push({ -1, static_cast<int>(QT_TOKEN_List.size() - 1) });
			}
			else {
				cout << "if表达式中类型错误" << endl;
				return 1;
			}
			//动作结束


			if (now_Token.type_id == 2 && PT[now_Token.num] == ")")
			{
				readToken();
				if (now_Token.type_id == 2 && PT[now_Token.num] == "{")
				{
					readToken();
					Compound();
					if (now_Token.type_id == 2 && PT[now_Token.num] == "}")
					{
						
						QT_TOKEN_List.at(SEM.top().num).result.num = static_cast<int>(QT_TOKEN_List.size() + 1);
						readToken();
						if (now_Token.type_id == 0 && KT[now_Token.num] == "else")
						{
							//产生四元式
							QT_TOKEN_List.push_back({ "else", { -1, -1 }, { -1, -1 }, { -1, static_cast<int>(QT_TOKEN_List.size() + 2) } });
							//地址回填，根据根据SEM.top().num找到if语句，修改num值
							QT_TOKEN_List.at(SEM.top().num).result.num = static_cast<int>(QT_TOKEN_List.size() + 1);
							//弹出对象栈中保存if语句地址的Token
							SEM.pop();
							//压入地址，等待回填else
							SEM.push({ -1, static_cast<int>(QT_TOKEN_List.size() - 1) });
							
							readToken();
							if (now_Token.type_id == 2 && PT[now_Token.num] == "{")
							{
								readToken();
								Compound();

								//产生四元式
								QT_TOKEN_List.push_back({ "ifend", { -1, -1 }, { -1, -1 }, { -1, -1 } });
								//地址回填，根据SEM.top().num找到else语句，修改num值
								QT_TOKEN_List.at(SEM.top().num).result.num = static_cast<int>(QT_TOKEN_List.size());
								//弹出对象栈中保存then语句地址的Token
								SEM.pop();
								//动作结束

								if (now_Token.type_id == 2 && PT[now_Token.num] == "}")
								{
									readToken();
									return 1;
								}
								else
								{
									cout << "选择语句缺少 } 右花括号！" << endl;
									flag = 0;
								}
							}
							else
							{
								cout << "选择语句缺少 { 左花括号！" << endl;
								flag = 0;
							}
						}
						else
						{
							//没有else
							QT_TOKEN_List.at(SEM.top().num).result.num = static_cast<int>(QT_TOKEN_List.size() + 1);
							SEM.pop();
							QT_TOKEN_List.push_back({ "ifend", { -1, -1 }, { -1, -1 }, { -1, -1 } });
							return 1;
						}
					}
					else
					{
						cout << "选择语句缺少 } 右花括号！" << endl;
						flag = 0;
					}
				}
				else
				{
					cout << "选择语句缺少 { 左花括号！" << endl;
					flag = 0;
				}
			}
		}
		else
		{
			cout << "选择语句缺少 （ 左括号！" << endl;
			flag = 0;
		}
	}
	else
		return 1;
	return 1;
}

//<循环语句>→while(<表达式>){<复合语句>}
int Cycle()
{
	if (now_Token.type_id == 0 && KT[now_Token.num] == "while")
	{
		//产生四元式
		QT_TOKEN_List.push_back({ "while", { -1, -1 }, { -1, -1 }, { -1, -1 } });
		//将while语句的地址入对象栈中，为地址回填做准备
		SEM.push({ -1, static_cast<int>(QT_TOKEN_List.size() - 1) });

		readToken();
		if (now_Token.type_id == 2 && PT[now_Token.num] == "(")
		{
			readToken();
			Expression();
			//产生四元式
			QT_TOKEN_List.push_back({ "do", SEM.top(), { -1, -1 }, { -1, -1 } });
			//弹出保存在对象栈中算术表达式的值
			SEM.pop();
			//将do语句的地址入对象栈中，为地址回填做准备
			SEM.push({ -1, static_cast<int>(QT_TOKEN_List.size() - 1) });

			if (now_Token.type_id == 2 && PT[now_Token.num] == ")")
			{
				readToken();
				if (now_Token.type_id == 2 && PT[now_Token.num] == "{")
				{
					readToken();
					Compound();

					//回填do语句跳转转地址
					QT_TOKEN_List.at(SEM.top().num).result.num = static_cast<int>(QT_TOKEN_List.size() + 2);
					//弹出do语句在对象栈中的Token
					SEM.pop();
					//产生四元式,whend语句跳转地址
					QT_TOKEN_List.push_back({ "whend", { -1, -1 }, { -1, -1 }, { -1, static_cast<int>(SEM.top().num + 1) } });
					//弹出while语句在对象栈中的Token
					SEM.pop();

					if (now_Token.type_id == 2 && PT[now_Token.num] == "}")
					{
						readToken();
						return 1;
					}
					else
					{
						cout << "循环语句缺少 } 右花括号！" << endl;
						flag = 0;
					}
				}
				else
				{
					cout << "循环语句缺少 { 左花括号！" << endl;
					flag = 0;
				}
			}
			else
			{
				cout << "循环语句缺少） 右括号！" << endl;
				flag = 0;
			}
		}
		else
		{
			cout << "循环语句缺少（ 左括号！" << endl;
			flag = 0;
		}
	}
	return 1;
}

//<返回语句>→return<标识符>;|return<整型常量>;
int Back()
{
	if (now_Token.type_id == 0 && KT[now_Token.num] == "return")
	{
		readToken();
		if (now_Token.type_id == 1 || now_Token.type_id == 3)
		{
			
			
			QT_TOKEN_List.push_back({ "ret",{-1,-1},{-1,-1},now_Token });
			
			
			
			readToken();
			if (now_Token.type_id == 2 && PT[now_Token.num] == ";")
			{
				readToken();
				return 1;
			}
			else
			{
				cout << "返回语句缺少 ; 分号！" << endl;
				flag = 0;
			}
		}
		else
		{
			cout << "返回值非标识符或整数常量！" << endl;
			flag = 0;
		}
	}
	return 1;
}

//<赋值、函数调用语句>→ < 标识符 >= <表达式>； | < 标识符 >= <标识符>(<实参>); | <标识符>(<实参>); | <标识符>[<算术表达式>]=
int Call_and_Assignment()
{
	if (now_Token.type_id == 1)
	{
		TOKEN_t = Is_Exist(IT.at(now_Token.num), now_level);
		if (TOKEN_t.num == -1)
		{
			cout << "变量未定义！" << endl;
			flag = 0;
			readToken();
			return 1;
		}
		SEM.push(TOKEN_t);
		if (Is_function(now_Token))
		{
			readToken();
			if (now_Token.type_id == 2 && PT[now_Token.num] == "(")
			{
				
				readToken();
				fs = 0;
				Actual();
				
				
				int u = search_SYNBL(IT.at(SEM.top().num));	//函数名在主表中的偏移量
				int w = -(SYNBL_List.at(u).ADDR.num);		//该函数在的地址层次
				
				if (fs<fc_t)
				{
					cout << SYNBL_List.at(u).name << "   函数实参少于形参！" << endl;
					flag = 0;
					return 1;
				}
				QT_TOKEN_List.push_back({ "call",SEM.top(),{-1,-1},{-1,(FM[2 * (w - 1)]+1)} });
				SEM.pop();	//弹出函数名
				
				if (now_Token.type_id == 2 && PT[now_Token.num] == ")")
				{
					readToken();
					//SEM.pop();
					if (now_Token.type_id == 2 && PT[now_Token.num] == ";")
					{
						readToken();
						return 1;
					}
					else
					{
						cout << "缺少 ; 分号！" << endl;
						flag = 0;
					}
				}
				else
				{
					cout << "缺少 ） 反括号！" << endl;
					flag = 0;
				}
			}
			
		
		}
		else
		{
			readToken();
			if (now_Token.type_id == 2 && PT[now_Token.num] == "=")
			{
				readToken();
				if (Is_function(now_Token))
				{
					SEM.push(now_Token);
					readToken();
					if (now_Token.type_id == 2 && PT[now_Token.num] == "(")
					{
						readToken();
						fs = 0;
						Actual();
						
						int u = search_SYNBL(IT.at(SEM.top().num));	//函数名在主表中的偏移量
						int w = -(SYNBL_List.at(u).ADDR.num);		//该函数在的地址层次
						
						if (fs < fc_t)
						{
							cout << SYNBL_List.at(u).name << "   函数实参少于形参！" << endl;
							flag = 0;
							return 1;
						}
						QT_TOKEN_List.push_back({ "call",SEM.top(),{-1,-1},{-1,FM[2 * (w - 1)]} });
						SEM.pop();	//弹出函数名
						SEM.push(QT_TOKEN_List.at(FM[2 * w - 1]).result);
						if (now_Token.type_id == 2 && PT[now_Token.num] == ")")
						{
							readToken();
							action_Assignment();
							if (now_Token.type_id == 2 && PT[now_Token.num] == ";")
							{
								readToken();
								return 1;
							}
							else
							{
								cout << "缺少 ; 分号！" << endl;
								flag = 0;
							}
						}
						else
						{
							cout << "缺少 ） 反括号！" << endl;
							flag = 0;
						}
					}

				}
				else
				{
					Expression();
					action_Assignment();
					if (now_Token.type_id == 2 && PT[now_Token.num] == ";")
					{
						readToken();
						return 1;
					}
					else
					{
						cout << "赋值或函数调用错误！" << endl;
						flag = 0;
					}
				}

			}
			else if (now_Token.type_id == 2 && PT[now_Token.num] == "[")
			{
				readToken();
				Arithmetic_Expression();
				if (now_Token.type_id == 2 && PT[now_Token.num] == "]")
				{
					readToken();
					if (now_Token.type_id == 2 && PT[now_Token.num] == "=")
					{
						readToken();
						Arithmetic_Expression();
						if (now_Token.type_id == 2 && PT[now_Token.num] == ";")
						{
							readToken();
							return 1;
						}
						else
						{
							cout << "数组赋值时缺少 ; 分号！" << endl;
							flag = 0;
						}
					}
					else
					{
						cout << "数组赋值时缺少 = 等号！" << endl;
						flag = 0;
					}

				}
				else
				{
					cout << "数组赋值时缺少 ] 右方括号！" << endl;
					flag = 0;
				}
			}

		}
		
		
	}
	else
	{
		cout << "赋值或函数调用错误！" << endl;
		flag = 0;
	}
	return 1;
}



//<实参>→<标识符>|<标识符>,<实参>
int Actual()
{
	if (now_Token.type_id == 1)
	{
		fs++;
		SEM.push(now_Token);
		action_F_assignment();
		readToken();
		if (now_Token.type_id == 2 && PT[now_Token.num] == ",")
		{
			readToken();
			Actual();
		}
	}
	else
	{
		cout << "函数实参定义错误！" << endl;
		flag = 0;
	}
	return 1;
}


//<表达式>→<算术表达式>|<逻辑表达式>|<字符常量>|<字符串常量>
int Expression()
{
	if (now_Token.type_id == 5 || now_Token.type_id == 6)
	{
		SEM.push(now_Token);
		readToken();
	}
	else
	{
		Arithmetic_Expression();
		Logic_Expression();
	}


	return 1;

}

//<算数表达式>→ <T> 【W1 <T>】        //e -> t e1			
/*
int Arithmetic_Expression()
{
	T();
	if (now_Token.type_id == 2 && (PT[now_Token.num] == "+" || PT[now_Token.num] == "-"))
	{
		readToken();
		T();
	}
	return 1;
}*/

//e1 -> + t e1 | - t e1

//<T>→<F> 【W2 <F>】				
/*
int T()
{
	F();
	if (now_Token.type_id == 2 && (PT[now_Token.num] == "*" || PT[now_Token.num] == "/"))
	{
		readToken();
		F();
	}
	return 1;
}*/


//<算术表达式> -> <T> <E1>	
	
int Arithmetic_Expression()
{
	T();
	E1();
	return 1;
}

//<E1> -> <W1> <T> <E1>
int E1()
{
	if (now_Token.type_id == 2 && (PT[now_Token.num] == "+" || PT[now_Token.num] == "-"))
	{
		SYN.push(now_Token);
		readToken();
		T();
		action_E1_T1();
		E1();
	}
	return 1;
}

//<T> -> <F> <T1>
int T()
{
	F();
	T1();
	return 1;
}

//<T1> -> <W2> <F> <T1>
int T1()
{
	if (now_Token.type_id == 2 && (PT[now_Token.num] == "*" || PT[now_Token.num] == "/"))
	{
		SYN.push(now_Token);
		readToken();
		F();
		action_E1_T1();
		T1();
	}
	return 1;
}


//<F>→(<算术表达式>)|<标识符> |<整数>|<实数>		
int F()
{
	if (now_Token.type_id == 2 && PT[now_Token.num] == "(")
	{
		readToken();
		Arithmetic_Expression();
		if (now_Token.type_id == 2 && PT[now_Token.num] == ")")
		{
			readToken();
			return 1;
		}
		else
		{
			cout << "算术表达式中缺少 ） 右括号！" << endl;
			flag = 0;
		}
	}
	else if (now_Token.type_id == 1)
	{

		TOKEN_t = Is_Exist(IT.at(now_Token.num), now_level);
		if (TOKEN_t.num != -1)
		{
			//标识符入对象栈
			SEM.push(TOKEN_t);
		}
		else {
			cout << "标识符: " << IT.at(now_Token.num) << " 未定义!" << endl;
			flag = 0;
		}
		readToken();
		return 1;
	}
	else if (now_Token.type_id == 3 || now_Token.type_id == 4)
	{
		//常量入对象栈
		SEM.push(now_Token);
		readToken();
		return 1;
	}
	else if (now_Token.type_id == 2)
	{
		cout << "cuowu" << endl;
		readToken();
		return 1;
	}
	return 1;
}

//逻辑表达式
int Logic_Expression()
{
	if (now_Token.type_id == 2 && (PT[now_Token.num] == "<" ||
		PT[now_Token.num] == "<=" || PT[now_Token.num] == ">" ||
		PT[now_Token.num] == ">=" || PT[now_Token.num] == "==" ||
		PT[now_Token.num] == "!="))
	{
		SYN.push(now_Token);
		readToken();
		Arithmetic_Expression();
		action_Logic();
	}
	return 1;
}

//判断该标识符是否是函数
int Is_function(TOKEN tm)
{
	string s;
	int h;
	if (tm.type_id == 1)
	{
		s = IT.at(tm.num);
		for (int i = 0; i < SYNBL_List.size(); i++)
		{
			if (SYNBL_List.at(i).name == s)
			{
				h = i;
				break;
			}
		}
		if (SYNBL_List.at(h).CAT == "f")
		{
			return 1;
		}
	}
	return 0;
}

//数组赋值的语义动作
int action_Array()
{
	int type_L, type_R;		//
	int kk = -1;

	//新建中间变量用于四元式保存结果
	//char s_t_[50];
	string s_t = "t";
	//sprintf(s_t_, "%d", t);
	s_t += to_string(t);
	t++;

	TOKEN value = SEM.top();	//读取的数字
	SEM.pop();
	TOKEN arr_IT = SEM.top();	//数组的标识符
	SYNBL arr_SYNBL = SYNBL_List.at(arr_IT.num);			//数组标识符的主表项
	TYPEL arr_TYPEL = TYPEL_List.at(arr_SYNBL.type.num);	//数组类型在类型表中的项
	AINFL arr_AINFL = AINFL_List.at(arr_TYPEL.TPOINT.num);	//数组在数组表中的项

	if (arr_TYPEL.TVAL != 'a')
	{
		return 1;
	}

	//数组赋值时没有越界
	if (Length_Array >= arr_AINFL.LOW && Length_Array < arr_AINFL.UP)
	{
		TOKEN kt = { 7,static_cast<int>(SYNBL_List.size()) };		//临时变量
		SYNBL_List.push_back({ s_t,arr_AINFL.CTP,"v",{ now_level,now_loca } });	//中间变量进主表
		now_loca = now_loca + arr_AINFL.CLEN;	//地址偏移量增加

		//取址四元式生成
		QT_TOKEN_List.push_back({ "[]",arr_IT,add_IC(Length_Array),kt });

		type_L = SYNBL_List.at(kt.num).type.num;
		if (value.type_id == 3 || value.type_id == 4)
		{
			if (type_L == 0 && value.type_id == 3)
			{
				kk = 1;
			}
			else if (type_L == 1)
			{
				kk = 1;
			}
		}
		else if (value.type_id == 5)
		{
			if (type_L == 3)
			{
				kk = 1;
			}
		}
		else
		{
			type_R = SYNBL_List.at(value.num).type.num;
			if (type_L == type_R)
			{
				kk = 1;
			}
		}
		if (kk == 1)
		{
			QT_TOKEN_List.push_back({ "=",value,{-1,-1},kt });
			Length_Array++;
			return 1;
		}
	}
	return 1;
}

//赋值语句的语义动作
int action_Assignment()
{
	int kk = -1;
	TOKEN t_r = SEM.top();
	SEM.pop();
	TOKEN t_l = SEM.top();
	SEM.pop();
	int type_l = SYNBL_List.at(t_l.num).type.num; // 类型 i r c s b f
	int type_r;

	if (t_r.type_id == 3 || t_r.type_id == 4)
	{
		if (type_l == 0 && t_r.type_id == 3)// i = r
		{
			kk = 1;
		}
		else if (type_l == 1 && (t_r.type_id == 3 || t_r.type_id == 4))	// r = i  或者  r = r 
		{
			kk = 1;
		}
		else if (type_l == 4)	//b
		{
			if (t_r.type_id == 3)	//i		
			{
				if (IC.at(t_r.num) == "0" || IC.at(t_r.num) == "1")		//b = i (0,1)
				{
					kk = 2;
				}
			}
		}
	}
	else if (t_r.type_id == 5)	//c
	{
		if (type_l == 2)	//c
		{
			kk = 1;
		}
	}
	else if (t_r.type_id == 6)	//s
	{
		if (type_l == 3)	//s
		{
			kk = 1;
		}
	}
	else
	{
		type_r = SYNBL_List.at(t_r.num).type.num;	//类型 i r c s b f
		if (type_r == type_l)
		{
			kk = 1;
		}
	}
	if (kk == 1 || kk == 2)
	{
		QT_TOKEN_List.push_back({ "=",t_r,{-1,-1},t_l });
	}
	else
	{
		cout << SYNBL_List.at(t_l.num).name <<"  赋值语句类型不匹配！" << endl;
		flag = 0;
	}
	return 1;
}

//函数调用参数赋值语义动作
int action_F_assignment()
{
		
	int kk = -1;
	TOKEN t_r = SEM.top();	//变量token
	SEM.pop();
	
	string ss = IT.at(SEM.top().num);
	int ff = search_SYNBL(ss);	//函数名在主表中的位置
	
	if (ff == -1)
	{
		cout << ss << "  函数不存在" << endl;
		flag = 0;
		return 1;
	}
	fc_t = PFINFL_List.at((-(SYNBL_List.at(ff).ADDR.num)-1)).FN;
	if (fs>fc_t)
	{
		cout << ss << "    实参个数大于形参！" << endl;
		flag = 0;
		return 1;
	}

	TOKEN t_l = PFINFL_List.at((-(SYNBL_List.at(ff).ADDR.num)-1)).parameter[fs - 1];	//形参中第（fs）个变量在主表中的记录
	//SEM.pop();
	
	int type_l = SYNBL_List.at(t_l.num).type.num; // 类型 i r c s b f
	int type_r;

	if (t_r.type_id == 3 || t_r.type_id == 4)
	{
		if (type_l == 0 && t_r.type_id == 3)// i  r
		{
			kk = 1;
		}
		else if (type_l == 4)	//b
		{
			if (t_r.type_id == 3)	//i
			{
				if (IC.at(t_r.num) == "0" || IC.at(t_r.num) == "1")
				{
					kk = 2;
				}
			}
		}
	}
	else if (t_r.type_id == 5)	//c
	{
		if (type_l == 2)	//c
		{
			kk = 1;
		}
	}
	else if (t_r.type_id == 6)	//s
	{
		if (type_l == 3)	//s
		{
			kk = 1;
		}
	}
	else
	{
		type_r = SYNBL_List.at(t_r.num).type.num;	//类型 i r c s b f
		if (type_r == type_l)
		{
			kk = 1;
		}
	}
	if (kk == 1 || kk == 2)
	{
		QT_TOKEN_List.push_back({ "=",t_r,{-1,-1},t_l });
	}
	else
	{
		cout << "函数实参形参类型不匹配！" << endl;
		flag = 0;
	}
	return 1;
}

//算术表达式中E1、T1的语义动作
int action_E1_T1()
{
	QT_TOKEN QT_temp;	//临时四元式
	SYNBL SYNBL_temp;	//临时主表项
	TOKEN T_temp;		//临时token

	//新建中间变量用于四元式保存结果
	//char s_t_[50];
	string s_t = "t";
	//sprintf(s_t_, "%d", t);
	s_t += to_string(t);
	t++;

	//输出四元式到临时四元式中
	QT_temp.operate = PT[SYN.top().num];
	SYN.pop();
	QT_temp.res2 = SEM.top();
	SEM.pop();
	QT_temp.res1 = SEM.top();
	SEM.pop();

	T_temp = type_after_two_res_add(QT_temp.res1, QT_temp.res2);
	if (T_temp.num >= 0)
	{
		//临时四元式填入四元式表
		QT_temp.result.type_id = 7;
		QT_temp.result.num = SYNBL_List.size();
		QT_TOKEN_List.push_back(QT_temp);

		//将当前中间变量填入主表
		SYNBL_temp.name = s_t;
		SYNBL_temp.type = T_temp;
		SYNBL_temp.CAT = "v";
		SYNBL_temp.ADDR.type_id = now_level;
		SYNBL_temp.ADDR.num = now_loca;
		SYNBL_List.push_back(SYNBL_temp);

		//地址偏移量增加
		now_loca = now_loca + (T_temp.num + 1) * 4;

		//将结果压入对象栈
		SEM.push(QT_temp.result);
	}
	return 1;
}

//逻辑表达式的语义动作
int action_Logic()
{
	QT_TOKEN QT_temp;	//临时四元式
	SYNBL SYNBL_temp;	//临时主表项
	TOKEN T_temp;		//临时token

	//新建中间变量用于四元式保存结果
	//char s_t_[50];
	string s_t = "t";
	//sprintf(s_t_, "%d", t);
	s_t += to_string(t);
	t++;

	//输出四元式到临时四元式中
	QT_temp.operate = PT[SYN.top().num];
	SYN.pop();
	QT_temp.res2 = SEM.top();
	SEM.pop();
	QT_temp.res1 = SEM.top();
	SEM.pop();
	QT_temp.result = { 7,static_cast<int>(SYNBL_List.size()) };

	//中间变量主表项
	SYNBL_temp = { s_t,{ 8,4 },"v",{ now_level,now_loca } };
	now_loca++;

	//中间变量进主表，四元式进四元式表
	SYNBL_List.push_back(SYNBL_temp);
	QT_TOKEN_List.push_back(QT_temp);

	//带有结果的中间变量进对象栈，弹出符号栈栈顶符
	SEM.push(QT_temp.result);
	//SYN.pop();

	return 0;
}


void autoAddType()
{
	int f;
	//支持的基本类型包括，整形，实型，字符，字符串，波尔型,函数
	char basicType[6] = { 'i', 'r', 'c', 's', 'b','f' };
	for (f = 0; f < 6; f++)
	{
		TYPEL_List.push_back({ basicType[f], { -1, -1 } });
	}
}

//添加整型常量
TOKEN add_IC(int t)
{
	TOKEN t1 = { 3,static_cast<int>(IC.size()) };
	string m = to_string(t);
	int k = search_IC(m);
	if (k == -1)
	{
		k = IC.size();
		IC.push_back(m);
	}
	t1 = { 3,k };
	return t1;
}

//返回两个操作数相加后的类型，即保存结果的类型
TOKEN type_after_two_res_add(TOKEN a, TOKEN b)
{
	int k = -1;
	TOKEN H;
	switch (a.type_id)
	{
	case 3:
		if (b.type_id == 3)
		{
			k = 0;
		}
		else if (b.type_id == 4)
		{
			k = 1;
		}
		else if (b.type_id == 7)
		{
			if (SYNBL_List.at(b.num).type.num == 0)
			{
				k = 0;
			}
			else
			{
				k = 1;
			}
		}
		break;
	case 4:
		k = 1;
		break;
	case 7:
		if (SYNBL_List.at(a.num).type.num == 0)
		{
			if (b.type_id == 3)
			{
				k = 0;
			}
			else if (b.type_id == 4)
			{
				k = 1;
			}
			else if (b.type_id == 7)
			{
				if (SYNBL_List.at(b.num).type.num == 0)
				{
					k = 0;
				}
				else
				{
					k = 1;
				}
			}
		}
		else
		{
			k = 1;
		}
	default:
		k = 0;
		break;
	}
	if (k == -1)
	{
		H = { -1,-1 };
	}
	else
	{
		H = { 8,k };
	}
	return H;
}

//寻找主表中是否存在 M 标识符
int search_SYNBL(string M)
{
	for (int i = 0; i < SYNBL_List.size(); i++)
	{
		if (M == SYNBL_List.at(i).name)
		{
			return i;
		}
	}
	return -1;
}

//返回数据类型
char Get_Type(TOKEN tok1)
{
	if (tok1.type_id == 7)
	{
		//从主表中查找到该Token的type再从TapArray表中查到类型
		return TYPEL_List.at(SYNBL_List.at(tok1.num).type.num).TVAL;
	}
	else
	{
		switch (tok1.type_id)
		{
		case 3: return 'i';
		case 4: return 'r';
		case 5: return 'c';
		case 6: return 's';
		default: return ' ';
		}
	}
}


//判断当前标识符是否已经定义
TOKEN Is_Exist(string s, int t)
{
	TOKEN tok;
	for (int i = 0; i < SYNBL_List.size(); i++)
	{
		if (SYNBL_List.at(i).name == s && SYNBL_List.at(i).ADDR.type_id == t)
		{
			tok = { 7,i };
			return tok;
		}
	}
	tok = { -1,-1 };
	return tok;
}

//返回类型的下标，0整型，1实型，2字符，3字符串，4布尔
int declar_type(string m)
{
	if (m == "int")
		return 0;
	else if (m == "float")
		return 1;
	else if (m == "char")
		return 2;
	else if (m == "string")
		return 3;
	else if (m == "bool")
		return 4;
}

//返回类型的长度
int Length_type(TOKEN k)
{
	if (k.type_id == 8)
	{
		switch (k.num)
		{
		case 0:
			return 4;
			break;
		case 1:
			return 8;
			break;
		case 2:
			return 1;
			break;
		case 4:
			return 1;
			break;
		default:
			break;
		}
		if (TYPEL_List.at(k.num).TVAL == 'a')
		{
			return AINFL_List.at(TYPEL_List.at(k.num).TPOINT.num).CLEN;
		}
	}
	else if (k.type_id == 7)
	{
		return Length_type(SYNBL_List.at(k.num).type);
	}
	return 0;
}






/**************   打印输出部分     *********/

string token_to_string(TOKEN q)
{
	string s = "";
	ostringstream out;
	switch (q.type_id)
	{
	case 0:
		s += KT[q.num];
		break;
	case 1:
		s += IT[q.num];
		break;
	case 2:
		s += PT[q.num];
		break;
	case 3:
		s += IC[q.num];
		break;
	case 4:
		s += FC[q.num];
		break;
	case 5:
		s += CT[q.num];
		break;
	case 6:
		s += ST[q.num];
		break;
	case 7:
		if (q.num < SYNBL_List.size())
		{
			s += SYNBL_List.at(q.num).name;
		}
		break;
	case 8:
		s += TYPEL_List.at(q.num).TVAL;
		break;
	case -1:
		if (q.num != -1)
		{
			out << q.num;
			s += out.str();
		}
		else
		{
			s += "_";
		}
		break;
	default:
		break;
	}
	return s;
}

void QT_token_to_string()
{
	QT_TOKEN QT_tok;
	QT QT_str;
	for (int i = 0; i < QT_TOKEN_List.size(); i++)
	{
		QT_tok = QT_TOKEN_List.at(i);
		QT_str.operate = QT_tok.operate;
		QT_str.res1 = token_to_string(QT_tok.res1);
		QT_str.res2 = token_to_string(QT_tok.res2);
		QT_str.result = token_to_string(QT_tok.result);
		QT_List.push_back(QT_str);
	}
}


void print_QT()
{
	string sss1;
	read_data("input.txt", sss1);
	printf_data(sss1);
	ofstream outfile6("SYNBL_List.txt", ios::trunc);
	outfile6.close();
	cout << printf_QT_List() << endl << endl;
	

}

//打印四元式并写入到文件中
string printf_QT_List()
{
	ostringstream printOut;
	QT_token_to_string();
	QT pl;
	printOut << "\n\t\t未进行优化的四元式" << endl;
	printOut << "序号\t操作符" << "\t\t操作数1" << "\t\t操作数2" << "\t\t结果数" << endl;
	for (int i = 0; i < QT_List.size(); i++)
	{
		pl = QT_List.at(i);
		printOut << "" << i + 1 << ":\t(" << pl.operate << "\t\t" << pl.res1 << "\t\t" << pl.res2 << "\t\t" << pl.result << "\t)" << endl;
	}
	//写入到文件中
	//string path = "SYNBL_List.txt";
	ofstream fout("QT_List.txt");
	if (fout) {
		fout << printOut.str() << endl;
		fout.close();
	}
	return printOut.str();
}





void print_fuhao()
{

	clear_file_syn();
	cout << "\n\n各个符号表展示：\n\n";
	cout << printf_SYNBL_List() << endl << endl;
	cout << Print_TYPEL() << endl << endl;
	cout << PrintAinfl() << endl << endl;
	cout << Print_PFINFL() << endl << endl;
}


void clear_file_syn()
{
	ofstream outfile("SYNBL_List.txt", ios::trunc);
	outfile.close();
	ofstream outfile1("TYPEL_List.txt", ios::trunc);
	outfile1.close();
	ofstream outfile2("AINFL_List.txt", ios::trunc);
	outfile2.close();
	ofstream outfile3("PFINFL_List.txt", ios::trunc);
	outfile3.close();
}



string printf_SYNBL_List()
{
	ostringstream printOut;
	int a;
	SYNBL i;
	printOut << "\t\t主表\t\t" << endl;
	printOut << "序号\t名字\t类型\t种类\t地址" << endl;
	for (a = 0; a < SYNBL_List.size(); a++) {
		i = SYNBL_List.at(a);
		printOut << a << "\t";
		printOut << i.name;
		printOut << "\t(" << i.type.type_id << " , " << i.type.num << ")\t";
		printOut <<"  "<< i.CAT;
		printOut << "\t(" << i.ADDR.type_id << " , " << i.ADDR.num << ")" << endl;
	}
	//写入到文件中
	//string path = "SYNBL_List.txt";
	ofstream fout("SYNBL_List.txt");
	if (fout) {
		fout << printOut.str() << endl;
		fout.close();
	}
	return printOut.str();
}

string Print_TYPEL() {
	ostringstream printOut;
	int a;
	TYPEL tap;
	printOut << "\t类型表\t\t" << endl;
	printOut << "序号\t类型\t指向数据结构指针" << endl;
	for (a = 0; a < TYPEL_List.size(); a++) {
		tap = TYPEL_List.at(a);
		printOut << a << "\t";
		printOut << tap.TVAL;
		printOut << "\t( " << tap.TPOINT.type_id << " , " << tap.TPOINT.num << " )" << endl;
	}
	//写入到文件中
	ofstream fout("TYPEL_List.txt");
	if (fout) {
		fout << printOut.str() << endl;
		fout.close();
	}
	return printOut.str();
}

//将数组表写到文件中
string PrintAinfl() {
	ostringstream printOut;
	int a;
	AINFL ain;
	printOut << "\t\t数组表\t\t" << endl;
	printOut << "序号\t起始\t末尾\t类型\t\t长度" << endl;
	for (a = 0; a < AINFL_List.size(); a++) {
		ain = AINFL_List.at(a);
		printOut << a << "\t";
		printOut << ain.LOW << "\t";
		printOut << ain.UP << "\t";
		printOut << "( " << ain.CTP.type_id << " , " << ain.CTP.num << " )\t"; // 进行数组表的展示。
		printOut << ain.CLEN << endl;
	}
	//写入到文件中
	ofstream fout("AINFL_List.txt");
	if (fout) 
	{
		fout << printOut.str() << endl;
		fout.close();
	}
	return printOut.str();
}

//将函数表写到文件中
string Print_PFINFL() 
{
	ostringstream printOut;
	int a;
	PFINFL fn;
	printOut << "\t函数表\t" << endl;
	printOut << "序号\t层次\t参数数\t参数" << endl;
	for (a = 0; a < PFINFL_List.size(); a++) 
	{
		fn = PFINFL_List.at(a);
		printOut << a << "\t";
		printOut << fn.farLEVEL << "\t";
		printOut << fn.FN << "\t";
		for (int i = 0; i < fn.FN; i++)
		{
			printOut << "( " << fn.parameter[i].type_id << " , " << fn.parameter[i].num << " )  "; // 进行数组表的展示。
		}
		//printOut << "( " << ain.CTP.type_id << " , " << ain.CTP.num << " )\t"; // 进行数组表的展示。
		printOut  << endl;
	}
	//写入到文件中
	ofstream fout("PFINFL_List.txt");
	if (fout) 
	{
		fout << printOut.str() << endl;
		fout.close();
	}
	return printOut.str();
}