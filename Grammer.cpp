#include "Grammer.h"


//构造函数
Grammer::Grammer(const std::set<char> &Vt_arg, const std::set<char> &Vn_arg,
	const char &S_arg, const std::set<produce> &Produces_arg) :
	Vt(Vt_arg), Vn(Vn_arg), S(S_arg), Produces(Produces_arg)
{

}

//消除（直接）左递归
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
				flag = 1;//存在左递归
				break;
			}
		}

		//如果存在左递归，进行消除左递归
		if (flag == 1)
		{
			produce tp;
			//首先创建一个新的非终结符
			char ny = 'A';
			while (Vn.find(ny) != Vn.end())
			{
				ny++;
			}
			if (ny > 'Z')
			{
				std::cerr << "Eliminate LeftRecursion failed！";
				return 0;
			}
			Vn.insert(ny);

			//删除原产生式
			for (auto j : tempps)
			{
				Produces.erase(j);
			}

			//植入新的产生式
			tp.first = ny;
			tp.second[0] = 0;
			Produces.insert(tp);//植入空串产生式

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
			}//植入普通产生式


		}//如果存在左递归，进行消除左递归


	}//遍历每一个非终结符

	return 1;
}

//取得某个文法符号的First集
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
		}//把该符号的所有产生式先提取出来

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

//取得某个字符串的First集,不包含空串情况
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
	/*初始化Followsets,把所有的非终结符导入*/
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
	}/*初始化Followsets,把所有的非终结符导入*/

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
						//首先从Followsets中提取出i的所有Follow集
						std::set<char> seti;
						for (auto p : Followsets)
						{
							if (p.first == i.first)
							{
								seti = p.second;
								break;
							}
						}
						//将i的follow集并到*j的follow集中去
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

					}//j是最后一个符号
					else
					{
						j--;
						//先求得j后面符号的first集
						int inte = j - i.second.begin();
						std::set<char> fs = GetFirstSet(i.second.substr(inte + 1, i.second.size() - inte - 1));
						if (fs.find(0) != fs.end())
						{
							//首先从Followsets中提取出i的所有Follow集
							std::set<char> seti;
							for (auto p : Followsets)
							{
								if (p.first == i.first)
								{
									seti = p.second;
									break;
								}
							}
							//将i的follow集并到*j的follow集中去
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
						}//集中有空串符号


						 //把first集里边的东西放入follow中
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



					}//j不是最后一个符号
				}//为非终结符号
			}//挨个扫描右部符号串的所有符号
		}//遍历每一个产生式



		 //中止条件：直到每个集合都不再变大了为止

		if (fbeifen == Followsets)
		{
			return;
		}
		else
		{
			fbeifen = Followsets;
		}
	}//重复使用规则

}

//打印属性的函数
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


//外部接口，访问私有数据成员
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