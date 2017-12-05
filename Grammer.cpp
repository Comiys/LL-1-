#include "Grammer.h"


//���캯��
Grammer::Grammer(const std::set<char> &Vt_arg, const std::set<char> &Vn_arg,
	const char &S_arg, const std::set<produce> &Produces_arg) :
	Vt(Vt_arg), Vn(Vn_arg), S(S_arg), Produces(Produces_arg)
{

}

//������ֱ�ӣ���ݹ�
bool Grammer::EliminateLeftRecursion()
{
	auto tempvn = Vn;
	for (auto i : tempvn)
	{
		std::set<produce> tempps;
		int flag = 0;
		for (auto j : Produces)
		{
			if (j.first == i)
			{
				tempps.insert(j);
			}
		}
		for (auto j : tempps)
		{
			if (j.first == j.second[0])
			{
				flag = 1;//������ݹ�
				break;
			}
		}

		//���������ݹ飬����������ݹ�
		if (flag == 1)
		{
			produce tp;
			//���ȴ���һ���µķ��ս��
			char ny = 'A';
			while (Vn.find(ny) != Vn.end())
			{
				ny++;
			}
			if (ny > 'Z')
			{
				std::cerr << "Eliminate LeftRecursion failed��";
				return 0;
			}
			Vn.insert(ny);

			//ɾ��ԭ����ʽ
			for (auto j : tempps)
			{
				Produces.erase(j);
			}

			//ֲ���µĲ���ʽ
			tp.first = ny;
			tp.second[0] = 0;
			Produces.insert(tp);//ֲ��մ�����ʽ

			for (auto j : tempps)
			{
				if (j.first == j.second[0])
				{
					tp.first = ny;
					tp.second = j.second;
					tp.second.erase(0, 1);
					tp.second.push_back(ny);
				}
				else
				{
					tp.first = j.first;
					tp.second = j.second;
					tp.second.push_back(ny);
				}
				Produces.insert(tp);
			}//ֲ����ͨ����ʽ


		}//���������ݹ飬����������ݹ�


	}//����ÿһ�����ս��

	return 1;
}

//ȡ��ĳ���ķ����ŵ�First��
std::set<char> Grammer::GetFirstSet(char symbol) const
{
	std::set<char> charset;
	std::set<produce> temp_pro;


	if (this->Vt.find(symbol) != this->Vt.end())
	{
		charset.insert(symbol);
		return charset;
	}
	else
	{
		for (auto i : Produces)
		{
			if (i.first == symbol)
			{
				temp_pro.insert(i);
			}
		}//�Ѹ÷��ŵ����в���ʽ����ȡ����

		for (auto i : temp_pro)
		{
			if (Vt.find(i.second[0]) != Vt.end())
			{
				charset.insert(i.second[0]);
			}
			else if (i.second[0] == 0)
			{
				charset.insert(0);
			}
			else
			{
				std::set<char> charset2;
				charset2 = GetFirstSet(i.second[0]);
				for (auto j : charset2)
				{
					charset.insert(j);
				}
			}
		}
		return charset;
	}

}

//ȡ��ĳ���ַ�����First��,�������մ����
std::set<char> Grammer::GetFirstSet(std::string str) const
{
	std::set<char> tempcharset;

	if (str.size() == 0)
	{
		tempcharset.insert(0);
		return tempcharset;
	}
	for (auto i : str)
	{
		if (Vt.find(i) != Vt.end())
		{
			tempcharset.insert(i);
			break;
		}
		else
		{
			std::set<char> temp2 = GetFirstSet(i);
			if (temp2.find(0) != temp2.end())
			{
				for (auto p : temp2)
				{
					tempcharset.insert(p);
				}
			}
			else
			{
				for (auto p : temp2)
				{
					tempcharset.insert(p);
				}
				break;
			}
		}//
	}//for

	return tempcharset;
}


std::set<char> Grammer::GetFollowSet(char symbol) const
{
	for (auto i : Followsets)
	{
		if (i.first == symbol)
		{
			return i.second;
		}
	}
}



void Grammer::GetAllTheFollowSets()
{
	/*��ʼ��Followsets,�����еķ��ս������*/
	for (auto i : Vn)
	{
		if (i == S)
		{
			firfolset tempset;
			tempset.first = i;
			tempset.second.insert('$');
			Followsets.insert(tempset);
		}
		else
		{
			firfolset tempset;
			tempset.first = i;
			tempset.second.clear();
			Followsets.insert(tempset);
		}
	}/*��ʼ��Followsets,�����еķ��ս������*/

	auto fbeifen = Followsets;

	for (;;)
	{

		for (auto i : Produces)
		{
			for (auto j = i.second.begin(); j != i.second.end(); j++)
			{
				if (Vt.find(*j) == Vt.end())
				{
					j++;
					if (j == i.second.end())
					{
						j--;
						//���ȴ�Followsets����ȡ��i������Follow��
						std::set<char> seti;
						for (auto p : Followsets)
						{
							if (p.first == i.first)
							{
								seti = p.second;
								break;
							}
						}
						//��i��follow������*j��follow����ȥ
						auto iter3 = Followsets.begin();
						for (; iter3 != Followsets.end(); iter3++)
						{
							if ((*iter3).first == *j)
							{
								break;
							}
						}
						auto tempss = *iter3;
						for (auto p : seti)
						{
							tempss.second.insert(p);
						}
						Followsets.erase(iter3);
						Followsets.insert(tempss);

					}//j�����һ������
					else
					{
						j--;
						//�����j������ŵ�first��
						int inte = j - i.second.begin();
						std::set<char> fs = GetFirstSet(i.second.substr(inte + 1, i.second.size() - inte - 1));
						if (fs.find(0) != fs.end())
						{
							//���ȴ�Followsets����ȡ��i������Follow��
							std::set<char> seti;
							for (auto p : Followsets)
							{
								if (p.first == i.first)
								{
									seti = p.second;
									break;
								}
							}
							//��i��follow������*j��follow����ȥ
							auto iter3 = Followsets.begin();
							for (; iter3 != Followsets.end(); iter3++)
							{
								if ((*iter3).first == *j)
								{
									break;
								}
							}
							auto tempss = *iter3;
							for (auto p : seti)
							{
								tempss.second.insert(p);
							}
							Followsets.erase(iter3);
							Followsets.insert(tempss);
						}//�����пմ�����


						 //��first����ߵĶ�������follow��
						fs.erase(0);
						auto iter3 = Followsets.begin();
						for (; iter3 != Followsets.end(); iter3++)
						{
							if ((*iter3).first == *j)
							{
								break;
							}
						}
						auto tempss = *iter3;
						for (auto p : fs)
						{
							tempss.second.insert(p);
						}
						Followsets.erase(iter3);
						Followsets.insert(tempss);



					}//j�������һ������
				}//Ϊ���ս����
			}//����ɨ���Ҳ����Ŵ������з���
		}//����ÿһ������ʽ



		 //��ֹ������ֱ��ÿ�����϶����ٱ����Ϊֹ

		if (fbeifen == Followsets)
		{
			return;
		}
		else
		{
			fbeifen = Followsets;
		}
	}//�ظ�ʹ�ù���

}

//��ӡ���Եĺ���
void Grammer::PrintVt() const
{
	std::cout << "The terminal symbols are:";
	for (auto i : Vt)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
}
void Grammer::PrintVn() const
{
	std::cout << "The none-terminal symbols are:";
	for (auto i : Vn)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
}
void Grammer::PrintS() const
{
	std::cout << "The start symbol is :" << S << std::endl;
}
void Grammer::PrintProduces() const
{
	std::cout << "The Produces are:" << std::endl;
	for (auto i : Produces)
	{
		std::cout << i.first << "-" << i.second << std::endl;
	}
}
void Grammer::PrintGrammer() const
{
	PrintVn();
	PrintVt();
	PrintS();
	PrintProduces();
}
void Grammer::PrintFollowSets() const
{
	std::cout << " The Followsets are:" << std::endl;
	for (auto i : Followsets)
	{
		std::cout << i.first << ":";
		for (auto j : i.second)
		{
			std::cout << j << ",";
		}
		std::cout << std::endl;
	}
}


//�ⲿ�ӿڣ�����˽�����ݳ�Ա
std::set<char> Grammer::getVn() const
{
	return Vn;
}
std::set<produce> Grammer::getProduces() const
{
	return Produces;
}
char Grammer::getS() const
{
	return S;
}
std::set<char> Grammer::getVt() const
{
	return Vt;
}