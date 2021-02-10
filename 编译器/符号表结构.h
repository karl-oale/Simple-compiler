#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include <sstream>
#include <fstream>

using namespace std;


//token���ڱ�ʾ�������
struct TOKEN
{
	/*
	type_id
	0���ؼ��ֱ�
	1����ʶ����
	2�������
	3�����ͳ�����
	4��ʵ�ͳ�����
	5���ַ���
	6���ַ�����

	7��SYNBL��
	8�����ͱ�
	9�������
	10�������
	11��������
	*/
	int type_id;
	/*
	numΪ�����͵ı�ţ�ƫ�Ƶ�ַ��
	*/
	int num;
	/*
	numΪ�����͵ı�ţ�ƫ�Ƶ�ַ��
	*/
};


//SYNBL����ṹ
struct SYNBL
{
	string name;	//����
	TOKEN type;		//����
	string CAT;		//����
	TOKEN ADDR;		//��ַ
};

//���ͱ�ṹ
struct TYPEL
{
	char TVAL;		//���ʹ���
	TOKEN TPOINT;	//��Ϣ����ָ��
};

//�����ṹ
struct AINFL
{
	int LOW;		//������ʼ���
	int UP;			//����ĩβ���
	TOKEN CTP;		//��������
	int CLEN;		//���鳤��
};

//������ṹ
struct PFINFL
{
	int farLEVEL;	//�������ڵĲ��
	int FN;			//��������
	TOKEN parameter[10];	//�β�����
};

//��Ԫʽ�ṹ
struct QT 
{
	string operate;  //������
	string res1;     //��һ��������
	string res2;     //�ڶ���������
	string result;   //�������
};

//��Ԫʽ�ṹ  token����
struct QT_TOKEN
{
	string operate;	//������
	TOKEN res1;		//��һ��������
	TOKEN res2;		//�ڶ���������
	TOKEN result;	//�������
};

//token����
extern vector<TOKEN> TOKEN_List;

//�ؼ��ֱ�
static string KT[11] = { "main","int","char","float","if","else","while","for","return","string","bool" };
//				     	   0      1      2      3      4     5      6      7      8			9		10

//��ʶ����
extern vector<string> IT;

//�����
static string PT[19] = { "+" ,"-","*","/","<",">","<=",">=","=","!=","==",";",",","(",")","[","]","{" ,"}" };
//                        0    1   2   3   4   5    6    7   8    9   10   11  12  13  14  15  16  17  18

//�ַ���
extern vector<string> CT;

//�ַ���������
extern vector<string> ST;

//���ͳ�����
extern vector<string> IC;

//ʵ�ͳ�����
extern vector<string> FC;

//SYNBL����
extern vector<SYNBL> SYNBL_List;

//���ͱ�
extern vector<TYPEL> TYPEL_List;

//�����
extern vector<AINFL> AINFL_List;

//������
extern vector<PFINFL> PFINFL_List;

//���ȱ�
extern vector<int> Len_List;

//��Ԫʽ����
extern vector<QT> QT_List;

//��Ԫʽ����
extern vector<QT_TOKEN> QT_TOKEN_List;

//�ʷ�����ɨ���ע��
extern vector<string> Comment;

//�ʷ�ɨ��ĵ��ʽṹ
struct Scan_Word
{
	string word;	//����
	int End_state;	//���ʵ��Զ�������״̬
};

//�ʷ�ɨ��ĵ����б�
extern vector<Scan_Word> Word_List;

//�ʷ�ɨ��ĵ����б�
extern vector<Scan_Word> Word_List;




/*********************************************       �ʷ���������(��ʼ��       *************************************************/

//�ʷ�����������
int scan_run();


//��ӡ���������
void printf_data(string data);

//���ļ���ȡ��������
void read_data(const string& file, string& Data);

//����ļ���ʼ��������ļ���ԭ����
void clear_file();

//��������浽�ļ���
void printf_tok(string a, string b, int c);

//�����Զ���״̬ת��
int tranS(int beS, char ch);

//�������Զ���״̬�����в����ж�
int scan_word_run(const string& Data);

//�Ե����б��������tokenƥ�����
int find_token();

//���Զ�������״̬�������
void find_tok(string w, int m);

//���ݵ����б���token���﷨�����õ�
TOKEN find_token(string w, int m);


//�ڹؼ��ֱ��в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_KT(string w);

//�ڱ�ʶ�����в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_IT(string w);

//�ڽ�����в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_PT(string w);

//���ַ��������в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_CT(string w);

//���ַ����������в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_ST(string w);

//�����ͳ������в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_IC(string w);

//��ʵ���������в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_FC(string w);


/*********************************************       �ʷ���������(������       *************************************************/








/*********************************************       �﷨�������֣���ʼ��       *************************************************/



//now_Tokenָ����һ��token
void readToken();

//����ʼ
void Analysis_run();

//<Դ����>��<��ʼ>
int Source();

//<��ʼ>��<��������><������>|<��������><�ⲿ����>
int begin();

//<������>��main ( ) {<�������>}
int main_function();

//<�ⲿ����>��<����ͷ>;<��ʼ>|<����ͷ>{<�������>}<��ʼ>
int External();

//<����ͷ>��<��ʶ��>(<�β�>)
int function_header();

//<�β�>��<��������><��ʶ��>|<��������><��ʶ��>,<�β�>
int Formal();

//<�������>��<��������><�������> | <��ֵ�������������><�������> | <ѡ�����><�������> | <ѭ�����><�������> | <�������><�������> | ��
int Compound();

//<��������>��<��������><��ʶ��>��|<��������><��ʶ��>��<��ʶ���б�>��|<��������><��ʶ��><�����б�>;
int Variable();

//<��ʶ���б�>��<��ʶ��> | <��ʶ��>,<��ʶ���б�>
int dentifier();

//<�����б�>��[<�������ʽ>] | ��<��ʶ��>[<�������ʽ>]<�����б�1>
int Array();

//<ѡ�����>��if(<���ʽ>){<�������>}��else(<���ʽ>){<�������>}��
int Select();

//<ѭ�����>��while(<���ʽ>){<�������>}
int Cycle();

//<�������>��return<��ʶ��>;|return<���ͳ���>;
int Back();

//<��ֵ�������������>�� < ��ʶ�� >= <���ʽ>�� | < ��ʶ�� >= <��ʶ��>(<ʵ��>); | <��ʶ��>(<ʵ��>); |< ��ʶ�� >[<�������ʽ>]=<���鸳ֵ>��
int Call_and_Assignment();


//<���鸳ֵ>��{<�����б�>} | <�ַ�������>
int Array_assignment();

//<�����б�>��<���ͳ���>|��<���ͳ���>|<ʵ�ͳ���>|��<ʵ�ͳ���>
int Digital_list();


//<ʵ��>��<��ʶ��>|<��ʶ��>,<ʵ��>
int Actual();

//<���ʽ>��<�������ʽ>|<�߼����ʽ>|<�ַ�����>|<�ַ�������>
int Expression();

//<�������ʽ>�� <T> ��W1 <T>��
int Arithmetic_Expression();

int E1();

//<T>��<F> ��W2 <F>��
int T();

int T1();

//<F>��(<�������ʽ>)|<��ʶ��> |<����>|<ʵ��>
int F();

//�߼����ʽ
int Logic_Expression();

/*********************************************       �﷨��������(������       *************************************************/








/*********************************************       �м��������(��ʼ��       *************************************************/

//�������͵ĳ���
int Length_type(TOKEN k);

//�������͵��±꣬0���ͣ�1ʵ�ͣ�2�ַ���3�ַ�����4����
int declar_type(string m);

//�жϵ�ǰ��ʶ���Ƿ��Ѿ�����
TOKEN Is_Exist(string s, int t);

//����������������Ӻ�����ͣ���������������
TOKEN type_after_two_res_add(TOKEN a, TOKEN b);

//������ͳ���
TOKEN add_IC(int t);

//���鸳ֵ�����嶯��
int action_Array();

//�������ò�����ֵ���嶯��
int action_F_assignment();

//��ֵ�������嶯��
int action_Assignment();

//�������ʽ��E1��T1�����嶯��
int action_E1_T1();

//�߼����ʽ�����嶯��
int action_Logic();

//��Ԫʽ��ӡ����
void print_QT();


//������������
char Get_Type(TOKEN tok1);

//��ʼ�����ͱ�
void autoAddType();

//token���͵���Ԫʽת��Ϊstring���͵���Ԫʽ
void QT_token_to_string();

//token��Ϊstring
string token_to_string(TOKEN q);

//�жϸñ�ʶ���Ƿ��Ǻ���
int Is_function(TOKEN tm);

//Ѱ���������Ƿ���� M ��ʶ��
int search_SYNBL(string M);

//��ʼ���ļ�������ļ�����
void clear_file_syn();

//��ӡ����д���ļ�
string printf_SYNBL_List();

//��ӡ�����д���ļ�
string PrintAinfl();

//��ӡ���ͱ�д���ļ�
string Print_TYPEL();

//��ӡ�����ű�
void print_fuhao();

//��������д���ļ���
string Print_PFINFL();

//��ӡ��Ԫʽ��д�뵽�ļ���
string printf_QT_List();

/*********************************************       �м��������(������       *************************************************/


void print_begin();//��ӡ�˵�