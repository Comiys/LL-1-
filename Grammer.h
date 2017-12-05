#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <set>
#include <tuple>
#include <stdexcept>
#include <algorithm>
#include <stack>


//����ʽ����
//��һ��Ԫ��Ϊ���ս��
//�ڶ���Ԫ��Ϊ�ս�ͷ��ս����ɵ��ַ������߿�(ascii:0)������մ���
typedef std::pair<char, std::string> produce;
typedef std::pair<char, std::set<char>>firfolset;
typedef std::tuple<char, char, produce> tableunit;


/*--------�ķ���--------*/
class Grammer {
public:
	//���캯�������಻֧��Ĭ�Ϲ��캯��
	Grammer(const std::set<char> &Vt_arg, const std::set<char> &Vn_arg,
		const char &S_arg, const std::set<produce> &Produces_arg);							//��Ԫ���������ķ�
	
	//���ܺ���
	bool EliminateLeftRecursion();															//������ݹ�
	bool ExtractLeftCommonFactor();															//��ȡ������
	std::set<char> GetFirstSet(char symbol) const;										    //�����ַ���First��
	std::set<char> GetFirstSet(std::string str) const;										//�����ַ�����first��
	std::set<char> GetFollowSet(char symbol) const;											//��Follow��
	std::set<std::tuple<char, char, std::set <produce>>> GetForecastAnalysisTable() const;	//��Ԥ�������
	void GetAllTheFollowSets();																//������еķ��ս����Follow��
	
	//��ӡ���Եĺ���
	void PrintVt() const;
	void PrintVn() const;
	void PrintS() const;
	void PrintProduces() const;
	void PrintGrammer() const;
	void PrintFollowSets() const;

	//�ⲿ�ӿڣ�ȡ��˽�����ݳ�Ա�Ŀ���
	std::set<char> getVn() const;
	std::set<produce> getProduces() const;
	char getS() const;
	std::set<char> getVt() const;

private:
	std::set<char> Vt;				//�ս���ļ��ϣ�����ǿ�,���ܺ���'$'����
	std::set<char> Vn;				//���ս���ļ��ϣ�����ǿ�,����Ϊ��д��ĸ
	char S;							//�ķ���ʼ���ս��������Vn
	std::set<produce> Produces;		//����ʽ�ļ���
	std::set<firfolset> Followsets; //���з��ս����follow��
};
/*---------------------*/


