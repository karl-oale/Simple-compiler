#include <iostream>
#include <stdio.h>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include "���ű�ṹ.h"

using namespace std;


vector<QT_TOKEN> QT_TOKEN_List;



TOKEN now_Token;	//��ǰ�Ķ�ȡtoken
int flag = 1;			//����﷨�����Ƿ���ȷ�ı�־��Ĭ��Ϊ1�������������Ϊ0
int pp = 0;			//now_token��дͷ


stack<TOKEN> SYN;	//����ջ
stack<TOKEN> SEM;	//����ջ
int t = 1;			//��Ԫʽ�м�����±�
int now_level = 20;		//���ڵĲ�
int now_loca = 0;		//���ڵ�λ��

TOKEN TOKEN_t;		//��ʱtoken
SYNBL SYNBL_t;		//��ʱ������
int T_t;			//��ʱ������
int Length_Array;	//���鸳ֵ���±�
int fn = 0;			//�����ĵ�ַ����ֵ
int FM[50];		//�����������ʼ��ֹ����Ԫʽ�±�
int fc = 0;			//ͳ���βθ���
int fs = 0;			//ͳ��ʵ�θ���
int fc_t = 0;		//�����βθ�����ʱ����
//now_Tokenָ����һ��token
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


//����ʼ
void Analysis_run()
{
	string sss;
	read_data("input.txt", sss);
	printf_data(sss);
	readToken();
	Source();
	if (flag == 1)
	{
		cout << "�﷨������ɣ�û�д���" << endl;
	}
	else if (flag == 0)
	{
		cout << "�﷨���������д��ڴ���" << endl;
	}
	else
	{
		cout << "�����־flag�������" << endl;
	}
}

//<Դ����>��<��ʼ>
int Source()
{
	
	begin();
	return 1;
}

//<��ʼ>��<��������><������>|<��������><�ⲿ����>
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
			cout << "������ȱ�ٻ������������" << endl;
			flag = 0;
		}
	}
	else
	{
		cout << "����ȱ�����ͷ���" << endl;
		flag = 0;
	}
	return 1;
}

//<������>��main ( ) {<�������>}
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
						cout << "����������ȱ�� } �һ����ţ�" << endl;
						flag = 0;
					}
				}
				else
				{
					cout << "����������ȱ�� } �һ����ţ�" << endl;
					flag = 0;
				}

			}
			else
			{
				cout << "������������ȱ�� �� �����ţ�" << endl;
				flag = 0;
			}
		}
		else
		{
			cout << "������������ȱ�� ( �����ţ�" << endl;
			flag = 0;
		}
	}
	return 1;
}

//<�ⲿ����>��<����ͷ>{<�������>}<��ʼ>
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
			cout << "��������ȱ�� } �һ����ţ�" << endl;
			flag = 0;
		}
	}
	else
	{
		cout << "�ⲿ������ڴ���" << endl;
		flag = 0;
	}
	return 1;
}

//<����ͷ>��<��ʶ��>(<�β�>)
int function_header()
{
	if (now_Token.type_id == 1)
	{
		
		QT_TOKEN_List.push_back({ "START",{-1,-1},{-1,-1},now_Token });
		TOKEN_t = Is_Exist(IT.at(now_Token.num), 20);
		if (TOKEN_t.num == -1)
		{
			//��ǰ��ʶ��������			 name		type(����)	CAT(���ࣩADDR(��ַ)
			SYNBL_List.push_back({ IT.at(now_Token.num), { 8,declar_type(KT[SYN.top().num]) }, "f", {20,-fn} });
			SEM.push({7,static_cast<int>(SYNBL_List.size()-1) });
			PFINFL_List.push_back({ now_level });
			//SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
			SYN.pop();

		}
		else
		{
			SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
			cout << IT.at(now_Token.num) << "  �����ظ�����" << endl;
			flag = 0;
		}

		readToken();
		if (now_Token.type_id == 2 && PT[now_Token.num] == "(")
		{
			readToken();
			fc = 0;		//�βθ���ͳ��
			Formal();
			if (now_Token.type_id == 2 && PT[now_Token.num] == ")")
			{
				readToken();
				PFINFL_List.at(fn - 1).FN = fc;
				return 1;
			}
			else
			{
				cout << "����������ȱ�� �� �����ţ�" << endl;
				flag = 0;
			}
		}
		else
		{
			cout << "����������ȱ�� ( �����ţ�" << endl;
			flag = 0;
		}
	}

	return 1;
}


//<�β�>��<��������><��ʶ��>|<��������><��ʶ��>,<�β�>
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
				//��ǰ��ʶ��������		   name						type�����ͣ�				CAT(���ࣩ		ADDR����ַ��
				SYNBL_List.push_back({ IT.at(now_Token.num), { 8,declar_type(KT[SYN.top().num]) }, "v", {now_level,now_loca} });
				SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
				PFINFL_List.at(fn - 1).parameter[fc - 1] = SEM.top();
				now_loca += Length_type(SEM.top());
			}
			else
			{
				SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
				cout << IT.at(now_Token.num) << "  �����ظ�����" << endl;
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
			cout << "������β�Ϊ�Ǳ�ʶ����" << endl;
			flag = 0;
		}
	}
	else
	{
		cout << "�������Ͷ������" << endl;
		flag = 0;
	}
	return 1;
}


//<�������>��<��������><�������> | <��ֵ�������������><�������> | <ѡ�����><�������> | <ѭ�����><�������> | <�������><�������> | ��
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



//<��������>��<��������><��ʶ��>��|<��������><��ʶ��>��<��ʶ���б�>��|<��������><��ʶ��><�����б�>;
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
				//��ǰ��ʶ��������		   name						type�����ͣ�				CAT(���ࣩ		ADDR����ַ��
				SYNBL_List.push_back({ IT.at(now_Token.num), { 8,declar_type(KT[SYN.top().num]) }, "v", {now_level,now_loca} });
				SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
				now_loca += Length_type(SEM.top());
			}
			else
			{
				SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
				cout << IT.at(now_Token.num) << "  �����ظ�����" << endl;
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
					cout << "��������ȱ�� ; �ֺţ�" << endl;
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
					cout << "��������ȱ�� ; �ֺţ�" << endl;
					flag = 0;
				}
			}
			else
			{
				cout << "��������ȱ�� ; �ֺţ�" << endl;
				flag = 0;
			}

		}

	}
	else
	{
		cout << "������������" << endl;
		flag = 0;
	}
	return 1;
}

//<��ʶ���б�>��<��ʶ��> | <��ʶ��>,<��ʶ���б�>
int dentifier()
{
	if (now_Token.type_id == 1)
	{
		TOKEN_t = Is_Exist(IT.at(now_Token.num), now_level);
		if (TOKEN_t.num == -1)
		{
			//��ǰ��ʶ��������		   name						type�����ͣ�				CAT(���ࣩ		ADDR����ַ��
			SYNBL_List.push_back({ IT.at(now_Token.num), { 8,declar_type(KT[SYN.top().num]) }, "v", {now_level,now_loca} });
			SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
			now_loca += Length_type(SEM.top());
		}
		else
		{
			SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
			cout << IT.at(now_Token.num) << "  �����ظ�����" << endl;
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
		cout << "��ʶ������ʧ�ܣ�" << endl;
		flag = 0;
	}
	return 1;
}

//<�����б�>��[<���ͳ���>] | ��<��ʶ��>[<���ͳ���>]<�����б�>
int Array()
{
	if (now_Token.type_id == 2 && PT[now_Token.num] == "[")
	{
		readToken();
		if (now_Token.type_id == 3)
		{
			//�˻�ԭ����Ϊ������ĳ���
			now_loca = now_loca - Length_type(SYNBL_List.back().type);
			SYNBL_t = SYNBL_List.back();

			//����ĳ���
			T_t = atoi(IC.at(now_Token.num).c_str()) * (Length_type(SYNBL_t.type));
			//����ǰ����������ӵ��������
			AINFL_List.push_back({ 0, atoi(IC.at(now_Token.num).c_str()),SYNBL_t.type,T_t });
			//����ǰ����������ӵ����ͱ���
			TYPEL_List.push_back({ 'a',{9,static_cast<int>(AINFL_List.size() - 1)} });

			//��ԭ����Ϊ������������������ָ���Ϊָ������
			SYNBL_List.at(SYNBL_List.size() - 1).type.num = TYPEL_List.size() - 1;

			//������ַ��ƫ�Ƶ�ַ
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
				cout << "���鶨��ȱ�� ] �ҷ����ţ�" << endl;
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
				//��ǰ��ʶ��������		   name						type�����ͣ�				CAT(���ࣩ		ADDR����ַ��
				SYNBL_List.push_back({ IT.at(now_Token.num), { 8,declar_type(KT[SYN.top().num]) }, "v", {now_level,now_loca} });
				SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });

			}
			else
			{
				SEM.push({ 7,static_cast<int>(SYNBL_List.size() - 1) });
				cout << IT.at(now_Token.num) << "  �����ظ�����" << endl;
				flag = 0;
			}
			readToken();
			if (now_Token.type_id == 2 && PT[now_Token.num] == "[")
			{
				readToken();
				//Arithmetic_Expression();
				if (now_Token.type_id == 3)
				{
					//�˻�ԭ����Ϊ������ĳ���
					now_loca = now_loca - Length_type(SYNBL_List.back().type);
					SYNBL_t = SYNBL_List.back();

					//����ĳ���
					T_t = atoi(IC.at(now_Token.num).c_str()) * (Length_type(SYNBL_t.type));
					//����ǰ����������ӵ��������
					AINFL_List.push_back({ 0, atoi(IC.at(now_Token.num).c_str()),SYNBL_t.type,T_t });
					//����ǰ����������ӵ����ͱ���
					TYPEL_List.push_back({ 'a',{9,static_cast<int>(AINFL_List.size() - 1)} });

					//��ԭ����Ϊ������������������ָ���Ϊָ������
					SYNBL_List.at(SYNBL_List.size() - 1).type.num = TYPEL_List.size() - 1;

					//������ַ��ƫ�Ƶ�ַ
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
						cout << "���鶨��ȱ�� ] �ҷ����ţ�" << endl;
						flag = 0;
					}
				}
				else
				{
					return 1;
					cout << "���鶨��[]�ڷ����ͳ�����" << endl;
					flag = 0;
				}
			}
			else
			{
				cout << "���鶨��ȱ�� [ �����ţ�" << endl;
				flag = 0;
			}
		}
	}
	return 1;
}


//<���鸳ֵ>��{<�����б�>} 
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
		cout << "�����ʼ���Ҳ�����ȷ��" << endl;
		flag = 0;
	}
	return 1;
}

//<�����б�>��<���ͳ���>|��<���ͳ���>|<ʵ�ͳ���>|��<ʵ�ͳ���>
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
		cout << "���鸳ֵ{ }��ǳ�����" << endl;
		flag = 0;
	}
	return 1;
}



//<ѡ�����>��if(<���ʽ>){<�������>}��else(<���ʽ>){<�������>}��
int Select()
{
	if (now_Token.type_id == 0 && KT[now_Token.num] == "if")
	{
		readToken();
		if (now_Token.type_id == 2 && PT[now_Token.num] == "(")
		{
			readToken();
			Expression();

			//������ʼ
				//������Ԫʽ
			if (Get_Type(SEM.top()) == 'b' || Get_Type(SEM.top()) == 'i' || Get_Type(SEM.top()) == 'r') {
				QT_TOKEN_List.push_back({ "if", SEM.top(), { -1, -1 }, { -1, -1 } });

				//��������ջ�б����������ʽ��Token
				SEM.pop();
				//then����ַ��Token�����ջ��Ϊ������׼��
				SEM.push({ -1, static_cast<int>(QT_TOKEN_List.size() - 1) });
			}
			else {
				cout << "if���ʽ�����ʹ���" << endl;
				return 1;
			}
			//��������


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
							//������Ԫʽ
							QT_TOKEN_List.push_back({ "else", { -1, -1 }, { -1, -1 }, { -1, static_cast<int>(QT_TOKEN_List.size() + 2) } });
							//��ַ������ݸ���SEM.top().num�ҵ�if��䣬�޸�numֵ
							QT_TOKEN_List.at(SEM.top().num).result.num = static_cast<int>(QT_TOKEN_List.size() + 1);
							//��������ջ�б���if����ַ��Token
							SEM.pop();
							//ѹ���ַ���ȴ�����else
							SEM.push({ -1, static_cast<int>(QT_TOKEN_List.size() - 1) });
							
							readToken();
							if (now_Token.type_id == 2 && PT[now_Token.num] == "{")
							{
								readToken();
								Compound();

								//������Ԫʽ
								QT_TOKEN_List.push_back({ "ifend", { -1, -1 }, { -1, -1 }, { -1, -1 } });
								//��ַ�������SEM.top().num�ҵ�else��䣬�޸�numֵ
								QT_TOKEN_List.at(SEM.top().num).result.num = static_cast<int>(QT_TOKEN_List.size());
								//��������ջ�б���then����ַ��Token
								SEM.pop();
								//��������

								if (now_Token.type_id == 2 && PT[now_Token.num] == "}")
								{
									readToken();
									return 1;
								}
								else
								{
									cout << "ѡ�����ȱ�� } �һ����ţ�" << endl;
									flag = 0;
								}
							}
							else
							{
								cout << "ѡ�����ȱ�� { �����ţ�" << endl;
								flag = 0;
							}
						}
						else
						{
							//û��else
							QT_TOKEN_List.at(SEM.top().num).result.num = static_cast<int>(QT_TOKEN_List.size() + 1);
							SEM.pop();
							QT_TOKEN_List.push_back({ "ifend", { -1, -1 }, { -1, -1 }, { -1, -1 } });
							return 1;
						}
					}
					else
					{
						cout << "ѡ�����ȱ�� } �һ����ţ�" << endl;
						flag = 0;
					}
				}
				else
				{
					cout << "ѡ�����ȱ�� { �����ţ�" << endl;
					flag = 0;
				}
			}
		}
		else
		{
			cout << "ѡ�����ȱ�� �� �����ţ�" << endl;
			flag = 0;
		}
	}
	else
		return 1;
	return 1;
}

//<ѭ�����>��while(<���ʽ>){<�������>}
int Cycle()
{
	if (now_Token.type_id == 0 && KT[now_Token.num] == "while")
	{
		//������Ԫʽ
		QT_TOKEN_List.push_back({ "while", { -1, -1 }, { -1, -1 }, { -1, -1 } });
		//��while���ĵ�ַ�����ջ�У�Ϊ��ַ������׼��
		SEM.push({ -1, static_cast<int>(QT_TOKEN_List.size() - 1) });

		readToken();
		if (now_Token.type_id == 2 && PT[now_Token.num] == "(")
		{
			readToken();
			Expression();
			//������Ԫʽ
			QT_TOKEN_List.push_back({ "do", SEM.top(), { -1, -1 }, { -1, -1 } });
			//���������ڶ���ջ���������ʽ��ֵ
			SEM.pop();
			//��do���ĵ�ַ�����ջ�У�Ϊ��ַ������׼��
			SEM.push({ -1, static_cast<int>(QT_TOKEN_List.size() - 1) });

			if (now_Token.type_id == 2 && PT[now_Token.num] == ")")
			{
				readToken();
				if (now_Token.type_id == 2 && PT[now_Token.num] == "{")
				{
					readToken();
					Compound();

					//����do�����תת��ַ
					QT_TOKEN_List.at(SEM.top().num).result.num = static_cast<int>(QT_TOKEN_List.size() + 2);
					//����do����ڶ���ջ�е�Token
					SEM.pop();
					//������Ԫʽ,whend�����ת��ַ
					QT_TOKEN_List.push_back({ "whend", { -1, -1 }, { -1, -1 }, { -1, static_cast<int>(SEM.top().num + 1) } });
					//����while����ڶ���ջ�е�Token
					SEM.pop();

					if (now_Token.type_id == 2 && PT[now_Token.num] == "}")
					{
						readToken();
						return 1;
					}
					else
					{
						cout << "ѭ�����ȱ�� } �һ����ţ�" << endl;
						flag = 0;
					}
				}
				else
				{
					cout << "ѭ�����ȱ�� { �����ţ�" << endl;
					flag = 0;
				}
			}
			else
			{
				cout << "ѭ�����ȱ�٣� �����ţ�" << endl;
				flag = 0;
			}
		}
		else
		{
			cout << "ѭ�����ȱ�٣� �����ţ�" << endl;
			flag = 0;
		}
	}
	return 1;
}

//<�������>��return<��ʶ��>;|return<���ͳ���>;
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
				cout << "�������ȱ�� ; �ֺţ�" << endl;
				flag = 0;
			}
		}
		else
		{
			cout << "����ֵ�Ǳ�ʶ��������������" << endl;
			flag = 0;
		}
	}
	return 1;
}

//<��ֵ�������������>�� < ��ʶ�� >= <���ʽ>�� | < ��ʶ�� >= <��ʶ��>(<ʵ��>); | <��ʶ��>(<ʵ��>); | <��ʶ��>[<�������ʽ>]=
int Call_and_Assignment()
{
	if (now_Token.type_id == 1)
	{
		TOKEN_t = Is_Exist(IT.at(now_Token.num), now_level);
		if (TOKEN_t.num == -1)
		{
			cout << "����δ���壡" << endl;
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
				
				
				int u = search_SYNBL(IT.at(SEM.top().num));	//�������������е�ƫ����
				int w = -(SYNBL_List.at(u).ADDR.num);		//�ú����ڵĵ�ַ���
				
				if (fs<fc_t)
				{
					cout << SYNBL_List.at(u).name << "   ����ʵ�������βΣ�" << endl;
					flag = 0;
					return 1;
				}
				QT_TOKEN_List.push_back({ "call",SEM.top(),{-1,-1},{-1,(FM[2 * (w - 1)]+1)} });
				SEM.pop();	//����������
				
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
						cout << "ȱ�� ; �ֺţ�" << endl;
						flag = 0;
					}
				}
				else
				{
					cout << "ȱ�� �� �����ţ�" << endl;
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
						
						int u = search_SYNBL(IT.at(SEM.top().num));	//�������������е�ƫ����
						int w = -(SYNBL_List.at(u).ADDR.num);		//�ú����ڵĵ�ַ���
						
						if (fs < fc_t)
						{
							cout << SYNBL_List.at(u).name << "   ����ʵ�������βΣ�" << endl;
							flag = 0;
							return 1;
						}
						QT_TOKEN_List.push_back({ "call",SEM.top(),{-1,-1},{-1,FM[2 * (w - 1)]} });
						SEM.pop();	//����������
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
								cout << "ȱ�� ; �ֺţ�" << endl;
								flag = 0;
							}
						}
						else
						{
							cout << "ȱ�� �� �����ţ�" << endl;
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
						cout << "��ֵ�������ô���" << endl;
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
							cout << "���鸳ֵʱȱ�� ; �ֺţ�" << endl;
							flag = 0;
						}
					}
					else
					{
						cout << "���鸳ֵʱȱ�� = �Ⱥţ�" << endl;
						flag = 0;
					}

				}
				else
				{
					cout << "���鸳ֵʱȱ�� ] �ҷ����ţ�" << endl;
					flag = 0;
				}
			}

		}
		
		
	}
	else
	{
		cout << "��ֵ�������ô���" << endl;
		flag = 0;
	}
	return 1;
}



//<ʵ��>��<��ʶ��>|<��ʶ��>,<ʵ��>
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
		cout << "����ʵ�ζ������" << endl;
		flag = 0;
	}
	return 1;
}


//<���ʽ>��<�������ʽ>|<�߼����ʽ>|<�ַ�����>|<�ַ�������>
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

//<�������ʽ>�� <T> ��W1 <T>��        //e -> t e1			
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

//<T>��<F> ��W2 <F>��				
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


//<�������ʽ> -> <T> <E1>	
	
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


//<F>��(<�������ʽ>)|<��ʶ��> |<����>|<ʵ��>		
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
			cout << "�������ʽ��ȱ�� �� �����ţ�" << endl;
			flag = 0;
		}
	}
	else if (now_Token.type_id == 1)
	{

		TOKEN_t = Is_Exist(IT.at(now_Token.num), now_level);
		if (TOKEN_t.num != -1)
		{
			//��ʶ�������ջ
			SEM.push(TOKEN_t);
		}
		else {
			cout << "��ʶ��: " << IT.at(now_Token.num) << " δ����!" << endl;
			flag = 0;
		}
		readToken();
		return 1;
	}
	else if (now_Token.type_id == 3 || now_Token.type_id == 4)
	{
		//���������ջ
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

//�߼����ʽ
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

//�жϸñ�ʶ���Ƿ��Ǻ���
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

//���鸳ֵ�����嶯��
int action_Array()
{
	int type_L, type_R;		//
	int kk = -1;

	//�½��м����������Ԫʽ������
	//char s_t_[50];
	string s_t = "t";
	//sprintf(s_t_, "%d", t);
	s_t += to_string(t);
	t++;

	TOKEN value = SEM.top();	//��ȡ������
	SEM.pop();
	TOKEN arr_IT = SEM.top();	//����ı�ʶ��
	SYNBL arr_SYNBL = SYNBL_List.at(arr_IT.num);			//�����ʶ����������
	TYPEL arr_TYPEL = TYPEL_List.at(arr_SYNBL.type.num);	//�������������ͱ��е���
	AINFL arr_AINFL = AINFL_List.at(arr_TYPEL.TPOINT.num);	//������������е���

	if (arr_TYPEL.TVAL != 'a')
	{
		return 1;
	}

	//���鸳ֵʱû��Խ��
	if (Length_Array >= arr_AINFL.LOW && Length_Array < arr_AINFL.UP)
	{
		TOKEN kt = { 7,static_cast<int>(SYNBL_List.size()) };		//��ʱ����
		SYNBL_List.push_back({ s_t,arr_AINFL.CTP,"v",{ now_level,now_loca } });	//�м����������
		now_loca = now_loca + arr_AINFL.CLEN;	//��ַƫ��������

		//ȡַ��Ԫʽ����
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

//��ֵ�������嶯��
int action_Assignment()
{
	int kk = -1;
	TOKEN t_r = SEM.top();
	SEM.pop();
	TOKEN t_l = SEM.top();
	SEM.pop();
	int type_l = SYNBL_List.at(t_l.num).type.num; // ���� i r c s b f
	int type_r;

	if (t_r.type_id == 3 || t_r.type_id == 4)
	{
		if (type_l == 0 && t_r.type_id == 3)// i = r
		{
			kk = 1;
		}
		else if (type_l == 1 && (t_r.type_id == 3 || t_r.type_id == 4))	// r = i  ����  r = r 
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
		type_r = SYNBL_List.at(t_r.num).type.num;	//���� i r c s b f
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
		cout << SYNBL_List.at(t_l.num).name <<"  ��ֵ������Ͳ�ƥ�䣡" << endl;
		flag = 0;
	}
	return 1;
}

//�������ò�����ֵ���嶯��
int action_F_assignment()
{
		
	int kk = -1;
	TOKEN t_r = SEM.top();	//����token
	SEM.pop();
	
	string ss = IT.at(SEM.top().num);
	int ff = search_SYNBL(ss);	//�������������е�λ��
	
	if (ff == -1)
	{
		cout << ss << "  ����������" << endl;
		flag = 0;
		return 1;
	}
	fc_t = PFINFL_List.at((-(SYNBL_List.at(ff).ADDR.num)-1)).FN;
	if (fs>fc_t)
	{
		cout << ss << "    ʵ�θ��������βΣ�" << endl;
		flag = 0;
		return 1;
	}

	TOKEN t_l = PFINFL_List.at((-(SYNBL_List.at(ff).ADDR.num)-1)).parameter[fs - 1];	//�β��еڣ�fs���������������еļ�¼
	//SEM.pop();
	
	int type_l = SYNBL_List.at(t_l.num).type.num; // ���� i r c s b f
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
		type_r = SYNBL_List.at(t_r.num).type.num;	//���� i r c s b f
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
		cout << "����ʵ���β����Ͳ�ƥ�䣡" << endl;
		flag = 0;
	}
	return 1;
}

//�������ʽ��E1��T1�����嶯��
int action_E1_T1()
{
	QT_TOKEN QT_temp;	//��ʱ��Ԫʽ
	SYNBL SYNBL_temp;	//��ʱ������
	TOKEN T_temp;		//��ʱtoken

	//�½��м����������Ԫʽ������
	//char s_t_[50];
	string s_t = "t";
	//sprintf(s_t_, "%d", t);
	s_t += to_string(t);
	t++;

	//�����Ԫʽ����ʱ��Ԫʽ��
	QT_temp.operate = PT[SYN.top().num];
	SYN.pop();
	QT_temp.res2 = SEM.top();
	SEM.pop();
	QT_temp.res1 = SEM.top();
	SEM.pop();

	T_temp = type_after_two_res_add(QT_temp.res1, QT_temp.res2);
	if (T_temp.num >= 0)
	{
		//��ʱ��Ԫʽ������Ԫʽ��
		QT_temp.result.type_id = 7;
		QT_temp.result.num = SYNBL_List.size();
		QT_TOKEN_List.push_back(QT_temp);

		//����ǰ�м������������
		SYNBL_temp.name = s_t;
		SYNBL_temp.type = T_temp;
		SYNBL_temp.CAT = "v";
		SYNBL_temp.ADDR.type_id = now_level;
		SYNBL_temp.ADDR.num = now_loca;
		SYNBL_List.push_back(SYNBL_temp);

		//��ַƫ��������
		now_loca = now_loca + (T_temp.num + 1) * 4;

		//�����ѹ�����ջ
		SEM.push(QT_temp.result);
	}
	return 1;
}

//�߼����ʽ�����嶯��
int action_Logic()
{
	QT_TOKEN QT_temp;	//��ʱ��Ԫʽ
	SYNBL SYNBL_temp;	//��ʱ������
	TOKEN T_temp;		//��ʱtoken

	//�½��м����������Ԫʽ������
	//char s_t_[50];
	string s_t = "t";
	//sprintf(s_t_, "%d", t);
	s_t += to_string(t);
	t++;

	//�����Ԫʽ����ʱ��Ԫʽ��
	QT_temp.operate = PT[SYN.top().num];
	SYN.pop();
	QT_temp.res2 = SEM.top();
	SEM.pop();
	QT_temp.res1 = SEM.top();
	SEM.pop();
	QT_temp.result = { 7,static_cast<int>(SYNBL_List.size()) };

	//�м����������
	SYNBL_temp = { s_t,{ 8,4 },"v",{ now_level,now_loca } };
	now_loca++;

	//�м������������Ԫʽ����Ԫʽ��
	SYNBL_List.push_back(SYNBL_temp);
	QT_TOKEN_List.push_back(QT_temp);

	//���н�����м����������ջ����������ջջ����
	SEM.push(QT_temp.result);
	//SYN.pop();

	return 0;
}


void autoAddType()
{
	int f;
	//֧�ֵĻ������Ͱ��������Σ�ʵ�ͣ��ַ����ַ�����������,����
	char basicType[6] = { 'i', 'r', 'c', 's', 'b','f' };
	for (f = 0; f < 6; f++)
	{
		TYPEL_List.push_back({ basicType[f], { -1, -1 } });
	}
}

//������ͳ���
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

//����������������Ӻ�����ͣ���������������
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

//Ѱ���������Ƿ���� M ��ʶ��
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

//������������
char Get_Type(TOKEN tok1)
{
	if (tok1.type_id == 7)
	{
		//�������в��ҵ���Token��type�ٴ�TapArray���в鵽����
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


//�жϵ�ǰ��ʶ���Ƿ��Ѿ�����
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

//�������͵��±꣬0���ͣ�1ʵ�ͣ�2�ַ���3�ַ�����4����
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

//�������͵ĳ���
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






/**************   ��ӡ�������     *********/

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

//��ӡ��Ԫʽ��д�뵽�ļ���
string printf_QT_List()
{
	ostringstream printOut;
	QT_token_to_string();
	QT pl;
	printOut << "\n\t\tδ�����Ż�����Ԫʽ" << endl;
	printOut << "���\t������" << "\t\t������1" << "\t\t������2" << "\t\t�����" << endl;
	for (int i = 0; i < QT_List.size(); i++)
	{
		pl = QT_List.at(i);
		printOut << "" << i + 1 << ":\t(" << pl.operate << "\t\t" << pl.res1 << "\t\t" << pl.res2 << "\t\t" << pl.result << "\t)" << endl;
	}
	//д�뵽�ļ���
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
	cout << "\n\n�������ű�չʾ��\n\n";
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
	printOut << "\t\t����\t\t" << endl;
	printOut << "���\t����\t����\t����\t��ַ" << endl;
	for (a = 0; a < SYNBL_List.size(); a++) {
		i = SYNBL_List.at(a);
		printOut << a << "\t";
		printOut << i.name;
		printOut << "\t(" << i.type.type_id << " , " << i.type.num << ")\t";
		printOut <<"  "<< i.CAT;
		printOut << "\t(" << i.ADDR.type_id << " , " << i.ADDR.num << ")" << endl;
	}
	//д�뵽�ļ���
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
	printOut << "\t���ͱ�\t\t" << endl;
	printOut << "���\t����\tָ�����ݽṹָ��" << endl;
	for (a = 0; a < TYPEL_List.size(); a++) {
		tap = TYPEL_List.at(a);
		printOut << a << "\t";
		printOut << tap.TVAL;
		printOut << "\t( " << tap.TPOINT.type_id << " , " << tap.TPOINT.num << " )" << endl;
	}
	//д�뵽�ļ���
	ofstream fout("TYPEL_List.txt");
	if (fout) {
		fout << printOut.str() << endl;
		fout.close();
	}
	return printOut.str();
}

//�������д���ļ���
string PrintAinfl() {
	ostringstream printOut;
	int a;
	AINFL ain;
	printOut << "\t\t�����\t\t" << endl;
	printOut << "���\t��ʼ\tĩβ\t����\t\t����" << endl;
	for (a = 0; a < AINFL_List.size(); a++) {
		ain = AINFL_List.at(a);
		printOut << a << "\t";
		printOut << ain.LOW << "\t";
		printOut << ain.UP << "\t";
		printOut << "( " << ain.CTP.type_id << " , " << ain.CTP.num << " )\t"; // ����������չʾ��
		printOut << ain.CLEN << endl;
	}
	//д�뵽�ļ���
	ofstream fout("AINFL_List.txt");
	if (fout) 
	{
		fout << printOut.str() << endl;
		fout.close();
	}
	return printOut.str();
}

//��������д���ļ���
string Print_PFINFL() 
{
	ostringstream printOut;
	int a;
	PFINFL fn;
	printOut << "\t������\t" << endl;
	printOut << "���\t���\t������\t����" << endl;
	for (a = 0; a < PFINFL_List.size(); a++) 
	{
		fn = PFINFL_List.at(a);
		printOut << a << "\t";
		printOut << fn.farLEVEL << "\t";
		printOut << fn.FN << "\t";
		for (int i = 0; i < fn.FN; i++)
		{
			printOut << "( " << fn.parameter[i].type_id << " , " << fn.parameter[i].num << " )  "; // ����������չʾ��
		}
		//printOut << "( " << ain.CTP.type_id << " , " << ain.CTP.num << " )\t"; // ����������չʾ��
		printOut  << endl;
	}
	//д�뵽�ļ���
	ofstream fout("PFINFL_List.txt");
	if (fout) 
	{
		fout << printOut.str() << endl;
		fout.close();
	}
	return printOut.str();
}