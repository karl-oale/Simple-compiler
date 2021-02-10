#define CURL_STATICLIB

//�ʷ�����.cpp
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include "���ű�ṹ.h"

using namespace std;

//token����
vector<TOKEN> TOKEN_List;

//�ؼ��ֱ�
//string KT[10] = { "main","int","char","float","if","else","while","for","return" };
//					0      1      2      3     4     5       6      7      8

//��ʶ����
vector<string> IT;

//�����
//string PT[19] = { "+" ,"-","*","/","<",">","<=",">=","=","!=","==",";",",","(",")","[","]","{" ,"}" };
//                 0    1   2   3   4   5    6    7   8    9   10   11  12  13  14  15  16  17  18

//�ַ���
vector<string> CT;

//�ַ���������
vector<string> ST;

//���ͳ�����
vector<string> IC;

//ʵ�ͳ�����
vector<string> FC;

//SYNBL����
vector<SYNBL> SYNBL_List;

//���ͱ�
vector<TYPEL> TYPEL_List;

//�����
vector<AINFL> AINFL_List;

//������
vector<PFINFL> PFINFL_List;

//���ȱ�
vector<int> Len_List;

//��Ԫʽ����
vector<QT> QT_List;

//�ʷ�ɨ��ĵ����б�
vector<Scan_Word> Word_List;

//token��������
string B[7] = { "KT","IT","PT","IC" ,"FC","CT","ST"};

struct Comment_
{
	string word;
	int num;
};

//�ʷ�����ɨ���ע��
vector <Comment_> Comment_list;


int r = 0;
int p = 0;
string word = "";
string Data;


//�����Զ���״̬
const int ERROR = -1;
const int S0 = 0;
const int S1 = 1;
const int S2 = 2;
const int S3 = 3;
const int S4 = 4;
const int S5 = 5;
const int S6 = 6;
const int S7 = 7;
const int S8 = 8;
const int S9 = 9;
const int S10 = 10;
const int S11 = 11;
const int S12 = 12;
const int S13 = 13;
const int S14 = 14;
const int S15 = 15;
const int S16 = 16;
const int S17 = 17;
const int S18 = 18;
const int S19 = 19;
const int S20 = 20;
const int S21 = 21;
const int S22 = 22;
const int S23 = 23;
const int S24 = 24;
const int S25 = 25;
const int S26 = 26;
const int S27 = 27;
const int S28 = 28;
const int S29 = 29;
const int S30 = 30;
const int ok1 = 90;
const int ok2 = 91;
const int ok3 = 92;
const int ok4 = 93;
const int ok5 = 94;
const int ok6 = 95;
const int ok7 = 96;
const int ok8 = 97;


//��ӡ���������
void printf_data(string data)
{
	cout << endl << "��ǰ�����Դ����Ϊ��" << endl;
	cout << data << endl << endl;
}

//���ļ���ȡ��������
void read_data(const string& file, string& Data)
{
	ifstream fin(file.c_str());
	if (!fin)
	{
		cerr << file << " error!" << endl;
		exit(1);
	}
	Data.clear();
	string line;
	while (getline(fin, line))
	{
		Data += line + '\n';
	}
}



//�����Զ���״̬ת��
int tranS(int beS, char ch)
{
	switch (beS)
	{
	case S0:
	{
		if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
			return S1;
		else if (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t')
			return S0;
		else if (ch == '\'')
			return S2;
		else if (ch == '"')
			return S4;
		else if (ch >= '0' && ch <= '9')
			return S6;
		else if (ch == '+')
			return S8;
		else if (ch == '-')
			return S9;
		else if (ch == '*')
			return S10;
		else if (ch == '/')
			return S11;
		else if (ch == '<')
			return S12;
		else if (ch == '>')
			return S13;
		else if (ch == '=')
			return S14;
		else if (ch == ':')
			return S15;
		else if (ch == ';')
			return S16;
		else if (ch == ',')
			return S17;
		else if (ch == '(')
			return S18;
		else if (ch == ')')
			return S19;
		else if (ch == '[')
			return S20;
		else if (ch == ']')
			return S21;
		else if (ch == '{')
			return S22;
		else if (ch == '}')
			return S23;
		else if (ch == '!')
			return S24;
		else
			return ERROR;
	}
	case S1:
	{
		if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
			return S1;
		else
			return ok1;
	}
	case S2:
	{
		//if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
		if (1)
		{
			//word += ch;
			//p++;
			return S3;
		}
		else
			return ERROR;
	}
	case S3:
	{
		if (ch == '\'')
		{
			word += ch;
			p++;
			while (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t')
			{
				p++;
				ch = Data[p];
			}
			return ok3;
		}


	}
	case S4:
	{
		//if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
		if (ch != '"')
			return S4;
		else if (ch == '"')
		{
			word += ch;
			p++;
			while (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t')
			{
				p++;
				ch = Data[p];
			}
			return ok4;
		}

		else
			return ERROR;
	}
	case S6:
	{
		if (ch >= '0' && ch <= '9')
			return S6;
		else if (ch == '.')
			return S26;
		else
			return ok5;
	}
	case S26:
	{
		if (ch >= '0' && ch <= '9')
			return S26;
		else
			return ok7;
	}
	case S8:
	{
		return ok6;
	}
	case S9:
	{
		return ok6;
	}case S10:
	{
		return ok6;
	}case S11:
	{
		if (ch == '/')
			return S27;
		return ok6;
	}
	case S27:
	{
		while (ch == '\n')
		{
			p++;
			ch = Data[p];
		}
		return ok8;
	}
	case S12:
	{
		//char c = data[p + 1];
		if (ch == '=')
		{
			return S25;
		}
		return ok6;
	}
	case S13:
	{
		if (ch == '=')
		{
			return S25;
		}
		return ok6;
	}
	case S14:
	{
		if (ch == '=')
		{
			return S25;
		}
		return ok6;
	}
	case S15:
	{
		return ok6;
	}
	case S16:
	{
		return ok6;
	}
	case S17:
	{
		return ok6;
	}
	case S18:
	{
		return ok6;
	}
	case S19:
	{
		return ok6;
	}
	case S20:
	{
		return ok6;
	}
	case S21:
	{
		return ok6;
	}
	case S22:
	{
		return ok6;
	}
	case S23:
	{
		return ok6;
	}
	case S24:
	{
		return S25;
	}
	case S25:
	{
		return ok6;
	}
	default:
	{
		return ERROR;
	}
	}
}

//�������Զ���״̬�����в����ж�
int scan_word_run(const string& Data)
{
	//int_List();//int main (){
	char ch = Data[p];
	while (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t')
	{
		++p;
		ch = Data[p];
	}
	word += ch;
	int now_s = 0, next_s = 0;
	next_s = tranS(now_s, ch);
	p++;
	now_s = next_s;
	while (p < Data.size())
	{
		//�Զ���δ������״̬
		if ((now_s >= 0) && (now_s < 90))
		{
			ch = Data[p];
			next_s = tranS(now_s, ch);

			if ((next_s >= 0) && (next_s < 90))
			{
				if (now_s == S4)
				{
					word += ch;
				}
				else
				{
					if (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t');
					else
					{
						word += ch;
					}
				}
				p++;
			}
			else if ((next_s >= 90) && (next_s < 98));
			else
			{
				p++;
				ch = Data[p];
			}
			now_s = next_s;


		}
		//�Զ����������״̬
		else if ((now_s >= 90) && (now_s < 98))
		{
			Word_List.push_back({word,next_s });
			//find_tok(word, next_s);
			now_s = S0;
			next_s = 0;
			word = "";
			if (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t')
			{
				while (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t')
				{
					p++;
					ch = Data[p];
				}
			}
			else
				;
		}
		//����
		else
		{
			cout << "error!" << endl;
			while (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t')
			{
				p++;
				ch = Data[p];
			}
			now_s = S0;
			next_s = 0;
			word = "";
		}
	}
	return 0;
}

//�Ե����б��������tokenƥ�����  ���ʷ����������)
int find_token()
{
	cout << "����˵����\nKT:�ؼ���  IT:��ʶ��  PT:���  CT:�ַ�����  \nFT:�ַ�������  IC:���ͳ���  FC:ʵ�ͳ��� \n\n����\t\t Token" << endl;
	for (int i = 0; i < Word_List.size(); i++)
	{
		find_tok(Word_List.at(i).word, Word_List.at(i).End_state);
		//if (Word_List.at(i).End_state == 97)
		//{
		//	Comment_list.push_back({ Word_List.at(i).word, Word_List.at(i).End_state });
		//}
	}
	return 1;
}


//�ݵ��ʺ͵��ʵĽ���״̬�����ظõ��ʵ�token���ͣ� ���﷨�����õģ�
TOKEN find_token(string w, int m)
{
	
	TOKEN temp;
	int k;
	switch (m)
	{
		//�ؼ��ַ��ͱ�ʶ��
	case 90:
		int KT_s, IT_s;
		KT_s = search_KT(w);
		IT_s = search_IT(w);
		if (KT_s != -1)			//�ؼ���
		{
			temp = { 0,KT_s };
			//cout << "0 , " << KT_s << endl;
		}
		else     				//��ʶ��
		{
			if (IT_s != -1)
			{
				temp = { 1,IT_s };
				//cout << "1 , " << IT_s << endl;
			}
			else
			{
				k = IT.size();
				IT.push_back(w);
				temp = { 1,k };
				//cout << "1 , " << k << endl;
			}
		}break;
		//�ַ�����
	case 92:
		int CT_s;
		CT_s = search_CT(w);
		if (CT_s != -1)
		{
			temp = { 5,CT_s };
			//cout << "5 , " << CT_s << endl;
		}
		else
		{
			k = CT.size();
			CT.push_back(w);
			temp = { 5,k };
			//cout << "5 , " << k << endl;
		}break;
		//�ַ�������
	case 93:
		int ST_s;
		ST_s = search_CT(w);
		if (ST_s != -1)
		{
			temp = { 6,ST_s };
			//cout << "6 , " << ST_s << endl;
		}
		else
		{
			k = ST.size();
			ST.push_back(w);
			temp = { 6,k };
			//cout << "6 , " << k << endl;
		}break;
		//���ͳ���
	case 94:
		int IC_s;
		IC_s = search_IC(w);
		if (IC_s != -1)
		{
			temp = { 3,IC_s };
			//cout << "3 , " << IC_s << endl;
		}
		else
		{
			k = IC.size();
			IC.push_back(w);
			temp = { 3,k };
			//cout << "3 , " << k << endl;
		}break;
		//���
	case 95:
		int PT_s;
		PT_s = search_PT(w);
		if (PT_s != -1)
		{
			temp = { 2,PT_s };
			//cout << "2 , " << PT_s << endl;
		}
		else
			temp = { -1,-1 };
		break;
		//ʵ���ͳ���
	case 96:
		int FC_s;
		FC_s = search_FC(w);
		if (FC_s != -1)
		{
			temp = { 4,FC_s };
			//cout << "4 , " << FC_s << endl;
		}
		else
		{
			k = FC.size();
			FC.push_back(w);
			temp = { 4,k };
			//cout << "4 , " << k << endl;
		}break;
		//ע��
	case 97:
		temp = { -2,-2 };
		break;
	default:
		temp = { -1,-1 };
		break;
	}
	return temp;

}








//���Զ�������״̬�������
void find_tok(string w, int m)
{
	int k;
	switch (m)
	{
	//�ؼ��ַ��ͱ�ʶ��
	case 90:
		int KT_s, IT_s;
		KT_s = search_KT(w);
		IT_s = search_IT(w);
		if (KT_s != -1)			//�ؼ���
		{
			TOKEN_List.push_back({ 0,KT_s });
			printf_tok(w, B[0], KT_s);
			
		}
		else     				//��ʶ��
		{
			if (IT_s != -1)
			{
				TOKEN_List.push_back({ 1,IT_s });
				printf_tok(w, B[1], IT_s);
			}
			else
			{
				k = IT.size();
				IT.push_back(w);
				TOKEN_List.push_back({ 1,k });
				printf_tok(w, B[1], k);
			}
		}break;
	//�ַ�����
	case 92:
		int CT_s;
		CT_s = search_CT(w);
		if (CT_s != -1)
		{
			TOKEN_List.push_back({ 5,CT_s });
			printf_tok(w, B[5], CT_s);
		}
		else
		{
			k = CT.size();
			CT.push_back(w);
			TOKEN_List.push_back({ 5,k });
			printf_tok(w, B[5],k);
		}break;
	//�ַ�������
	case 93:
		int ST_s;
		ST_s = search_CT(w);
		if (ST_s != -1)
		{
			TOKEN_List.push_back({ 6,ST_s });
			printf_tok(w, B[6], ST_s);
		}
		else
		{
			k = ST.size();
			ST.push_back(w);
			TOKEN_List.push_back({ 6,k });
			printf_tok(w, B[6], k);
		}break; 
	//���ͳ���
	case 94:
		int IC_s;
		IC_s = search_IC(w);
		if (IC_s != -1)
		{
			TOKEN_List.push_back({ 3,IC_s });
			printf_tok(w, B[3], IC_s);
		}
		else
		{
			k = IC.size();
			IC.push_back(w);
			TOKEN_List.push_back({ 3,k });
			printf_tok(w, B[3], k);
			
		}break;
	//���
	case 95:
		int PT_s;
		PT_s = search_PT(w);
		if (PT_s != -1)
		{
			printf_tok(w, B[2], PT_s);
			
		}
		break;
	//ʵ���ͳ���
	case 96:
		int FC_s;
		FC_s = search_FC(w);
		if (FC_s != -1)
		{
			TOKEN_List.push_back({ 4,FC_s });
			printf_tok(w, B[4], FC_s);
			//cout << "4 , " << FC_s << endl;
		}
		else
		{
			k = FC.size();
			FC.push_back(w);
			TOKEN_List.push_back({ 5,k });
			printf_tok(w, B[4], k);
			//cout << "4 , " << k << endl;
		}break;
	default:
		break;
	}
}

//�ļ���ʼ��������ļ���ԭ����
void clear_file()
{
	ofstream outfile("Token_List.txt", ios::trunc);
	outfile.close();
}

//��������浽�ļ���
void printf_tok(string a, string b, int c)
{
	ofstream out1file;
	out1file.open("Token_List.txt", ios::binary | ios::app | ios::in | ios::out);

	out1file <<"\t"<< a << "\t\t <" << b << "," << c << ">" << "\n";

	out1file.close();//�ر��ļ��������ļ�

	cout << "" << a << "\t\t <" << b << "," << c << ">" << endl;
}


//�ڹؼ��ֱ��в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_KT(string w)
{
	//const char* p = w.data();
	for (int i = 0; i < 11; i++)
	{
		if (KT[i] == w)
		{
			return i;
		}
	}
	return -1;
}

//�ڱ�ʶ�����в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_IT(string w)
{
	for (int i = 0; i < IT.size(); i++)
	{
		if (w == IT.at(i))
		{
			return i;
		}
	}
	return -1;
}

//�ڽ�����в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_PT(string w)
{
	for (int i = 0; i < 19; i++)
	{
		if (PT[i] == w)
		{
			return i;
		}
	}
	return -1;
}

//���ַ��������в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_CT(string w)
{
	string m;
	for (int i = 0; i < CT.size(); i++)
	{
		if (w == CT.at(i))
		{
			return i;
		}
	}
	return -1;
}

//���ַ����������в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_ST(string w)
{
	for (int i = 0; i < ST.size(); i++)
	{
		if (w == ST.at(i))
		{
			return i;
		}
	}
	return -1;
}

//�����ͳ������в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_IC(string w)
{
	for (int i = 0; i < IC.size(); i++)
	{
		if (w == IC.at(i))
		{
			return i;
		}
	}
	return -1;
}

//��ʵ���������в���w���ҵ��򷵻�����λ���±꣬���򷵻�-1
int search_FC(string w)
{
	for (int i = 0; i < FC.size(); i++)
	{
		if (w == FC.at(i))
		{
			return i;
		}
	}
	return -1;
}



//�ʷ�����������
int scan_run()
{
	string input_file;
	string input_data;
	clear_file();
	input_file = "input.txt";
	//cin >> input_file;
	read_data(input_file, input_data);
	printf_data(input_data);
	scan_word_run(input_data);
	find_token();
	//system("pause");
	return 0;
}