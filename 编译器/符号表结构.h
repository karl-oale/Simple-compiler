#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include <sstream>
#include <fstream>

using namespace std;


//token用于表示表的类型
struct TOKEN
{
	/*
	type_id
	0：关键字表
	1：标识符表
	2：界符表
	3：整型常量表
	4：实型常量表
	5：字符表
	6：字符串表

	7：SYNBL表
	8：类型表
	9：数组表
	10：种类表
	11：函数表
	*/
	int type_id;
	/*
	num为该类型的标号（偏移地址）
	*/
	int num;
	/*
	num为该类型的标号（偏移地址）
	*/
};


//SYNBL主表结构
struct SYNBL
{
	string name;	//名字
	TOKEN type;		//类型
	string CAT;		//种类
	TOKEN ADDR;		//地址
};

//类型表结构
struct TYPEL
{
	char TVAL;		//类型代码
	TOKEN TPOINT;	//信息内容指向
};

//数组表结构
struct AINFL
{
	int LOW;		//数组起始标号
	int UP;			//数组末尾标号
	TOKEN CTP;		//数组类型
	int CLEN;		//数组长度
};

//函数表结构
struct PFINFL
{
	int farLEVEL;	//函数所在的层次
	int FN;			//参数个数
	TOKEN parameter[10];	//形参名称
};

//四元式结构
struct QT 
{
	string operate;  //操作符
	string res1;     //第一个操作数
	string res2;     //第二个操作数
	string result;   //操作结果
};

//四元式结构  token类型
struct QT_TOKEN
{
	string operate;	//操作符
	TOKEN res1;		//第一个操作数
	TOKEN res2;		//第二个操作数
	TOKEN result;	//操作结果
};

//token序列
extern vector<TOKEN> TOKEN_List;

//关键字表
static string KT[11] = { "main","int","char","float","if","else","while","for","return","string","bool" };
//				     	   0      1      2      3      4     5      6      7      8			9		10

//标识符表
extern vector<string> IT;

//界符表
static string PT[19] = { "+" ,"-","*","/","<",">","<=",">=","=","!=","==",";",",","(",")","[","]","{" ,"}" };
//                        0    1   2   3   4   5    6    7   8    9   10   11  12  13  14  15  16  17  18

//字符表
extern vector<string> CT;

//字符串常量表
extern vector<string> ST;

//整型常量表
extern vector<string> IC;

//实型常量表
extern vector<string> FC;

//SYNBL主表
extern vector<SYNBL> SYNBL_List;

//类型表
extern vector<TYPEL> TYPEL_List;

//数组表
extern vector<AINFL> AINFL_List;

//函数表
extern vector<PFINFL> PFINFL_List;

//长度表
extern vector<int> Len_List;

//四元式序列
extern vector<QT> QT_List;

//四元式序列
extern vector<QT_TOKEN> QT_TOKEN_List;

//词法分析扫描的注释
extern vector<string> Comment;

//词法扫描的单词结构
struct Scan_Word
{
	string word;	//单词
	int End_state;	//单词的自动机结束状态
};

//词法扫描的单词列表
extern vector<Scan_Word> Word_List;

//词法扫描的单词列表
extern vector<Scan_Word> Word_List;




/*********************************************       词法分析部分(开始）       *************************************************/

//词法分析主函数
int scan_run();


//打印输入的数据
void printf_data(string data);

//从文件读取测试数据
void read_data(const string& file, string& Data);

//输出文件初始化，清空文件的原内容
void clear_file();

//将输出保存到文件中
void printf_tok(string a, string b, int c);

//有限自动机状态转换
int tranS(int beS, char ch);

//根据现自动机状态，进行操作判断
int scan_word_run(const string& Data);

//对单词列表逐个进行token匹配输出
int find_token();

//按自动机结束状态进行输出
void find_tok(string w, int m);

//根据单词列表返回token，语法分析用到
TOKEN find_token(string w, int m);


//在关键字表中查找w，找到则返回所在位置下标，否则返回-1
int search_KT(string w);

//在标识符表中查找w，找到则返回所在位置下标，否则返回-1
int search_IT(string w);

//在界符表中查找w，找到则返回所在位置下标，否则返回-1
int search_PT(string w);

//在字符常量表中查找w，找到则返回所在位置下标，否则返回-1
int search_CT(string w);

//在字符串常量表中查找w，找到则返回所在位置下标，否则返回-1
int search_ST(string w);

//在整型常量表中查找w，找到则返回所在位置下标，否则返回-1
int search_IC(string w);

//在实数常量表中查找w，找到则返回所在位置下标，否则返回-1
int search_FC(string w);


/*********************************************       词法分析部分(结束）       *************************************************/








/*********************************************       语法分析部分（开始）       *************************************************/



//now_Token指向下一个token
void readToken();

//程序开始
void Analysis_run();

//<源程序>→<开始>
int Source();

//<开始>→<数据类型><主函数>|<数据类型><外部定义>
int begin();

//<主函数>→main ( ) {<复合语句>}
int main_function();

//<外部定义>→<函数头>;<开始>|<函数头>{<复合语句>}<开始>
int External();

//<函数头>→<标识符>(<形参>)
int function_header();

//<形参>→<数据类型><标识符>|<数据类型><标识符>,<形参>
int Formal();

//<复合语句>→<变量声明><复合语句> | <赋值、函数调用语句><复合语句> | <选择语句><复合语句> | <循环语句><复合语句> | <返回语句><复合语句> | ε
int Compound();

//<变量声明>→<数据类型><标识符>；|<数据类型><标识符>，<标识符列表>；|<数据类型><标识符><数组列表>;
int Variable();

//<标识符列表>→<标识符> | <标识符>,<标识符列表>
int dentifier();

//<数组列表>→[<算术表达式>] | ，<标识符>[<算术表达式>]<数组列表1>
int Array();

//<选择语句>→if(<表达式>){<复合语句>}【else(<表达式>){<复合语句>}】
int Select();

//<循环语句>→while(<表达式>){<复合语句>}
int Cycle();

//<返回语句>→return<标识符>;|return<整型常量>;
int Back();

//<赋值、函数调用语句>→ < 标识符 >= <表达式>； | < 标识符 >= <标识符>(<实参>); | <标识符>(<实参>); |< 标识符 >[<算术表达式>]=<数组赋值>；
int Call_and_Assignment();


//<数组赋值>→{<数字列表>} | <字符串常量>
int Array_assignment();

//<数字列表>→<整型常量>|，<整型常量>|<实型常量>|，<实型常量>
int Digital_list();


//<实参>→<标识符>|<标识符>,<实参>
int Actual();

//<表达式>→<算术表达式>|<逻辑表达式>|<字符常量>|<字符串常量>
int Expression();

//<算数表达式>→ <T> 【W1 <T>】
int Arithmetic_Expression();

int E1();

//<T>→<F> 【W2 <F>】
int T();

int T1();

//<F>→(<算术表达式>)|<标识符> |<整数>|<实数>
int F();

//逻辑表达式
int Logic_Expression();

/*********************************************       语法分析部分(结束）       *************************************************/








/*********************************************       中间代码生成(开始）       *************************************************/

//返回类型的长度
int Length_type(TOKEN k);

//返回类型的下标，0整型，1实型，2字符，3字符串，4布尔
int declar_type(string m);

//判断当前标识符是否已经定义
TOKEN Is_Exist(string s, int t);

//返回两个操作数相加后的类型，即保存结果的类型
TOKEN type_after_two_res_add(TOKEN a, TOKEN b);

//添加整型常量
TOKEN add_IC(int t);

//数组赋值的语义动作
int action_Array();

//函数调用参数赋值语义动作
int action_F_assignment();

//赋值语句的语义动作
int action_Assignment();

//算术表达式中E1、T1的语义动作
int action_E1_T1();

//逻辑表达式的语义动作
int action_Logic();

//四元式打印函数
void print_QT();


//返回数据类型
char Get_Type(TOKEN tok1);

//初始化类型表
void autoAddType();

//token类型的四元式转化为string类型的四元式
void QT_token_to_string();

//token变为string
string token_to_string(TOKEN q);

//判断该标识符是否是函数
int Is_function(TOKEN tm);

//寻找主表中是否存在 M 标识符
int search_SYNBL(string M);

//初始化文件，清空文件内容
void clear_file_syn();

//打印主表并写入文件
string printf_SYNBL_List();

//打印数组表并写到文件
string PrintAinfl();

//打印类型表并写入文件
string Print_TYPEL();

//打印各符号表
void print_fuhao();

//将函数表写到文件中
string Print_PFINFL();

//打印四元式并写入到文件中
string printf_QT_List();

/*********************************************       中间代码生成(结束）       *************************************************/


void print_begin();//打印菜单