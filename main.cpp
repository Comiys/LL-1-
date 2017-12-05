#include "Grammer.h"

bool analyze(std::string s, std::set<tableunit> M, char S, std::set<char> vt);



//main函数作用，通过键盘输入一个LL(1)文法，计算出此文法的预测分析表并打印输出
int main(int argc, char* argv) 
{
	char *pc = new char;
	std::string *pstr = new std::string;
	std::set<char> *pvt = new std::set<char>;
	std::set<char> *pvn = new std::set<char>;
	char *ps = new char;
	std::set<produce> *pproduces = new std::set<produce>;
	std::pair<char, std::string> *ppair = new std::pair<char, std::string>;
	/*输入非终结符集*/
	std::cout << "Please input the Non-terminal symbol(You can only use the uppercase letter):";
	*pc = std::cin.get();
	while (*pc != '\n')
	{
		if (*pc != ' ')
		{
			pvn->insert(*pc);
			*pc = std::cin.get();
		}
		else
		{
			*pc = std::cin.get();
		}
	}
	/*检测非终结符的合法性*/
	if (pvn->size() == 0)
	{
		std::cerr << "Input error, goodbye.:-)";
		getchar();
		return 0;
	}
	else
	{
		for (auto i : *pvn)
		{
			if (i<'A' || i>'Z')
			{
				std::cerr << "Input error, goodbye.:-)";
				getchar();
				return 0;
			}
		}
	}

	/*读入终结符集*/
	std::cout << "Please input the terminal symbols\n(You can't use the character '$' and the characters that used in the non-terminal sysmbols set):";
	*pc = std::cin.get();
	while (*pc != '\n')
	{
		if (*pc != ' ')
		{
			pvt->insert(*pc);
			*pc = std::cin.get();
		}
		else
		{
			*pc = std::cin.get();
		}
	}
	/*检测终结符的合法性*/
	if (pvt->size() == 0)
	{
		std::cerr << "Input error, goodbye.:-)";
		getchar();
		return 0;
	}
	else
	{
		for (auto i : *pvt)
		{
			if (i=='$' || pvn->find(i) != pvn->end() )
			{
				std::cerr << "Input error, goodbye.:-)";
				getchar();
				return 0;
			}
		}
	}
	/*读入文法的开始符号*/
	std::cout << "Please input the start non-terminal symbol:(must be included in the vn set):";
	*ps = std::cin.get();
	getchar();
	/*检测文法开始符号的准确性*/
	if (pvn->find(*ps) == pvn->end())
	{
		std::cerr << "Input error, goodbye.:-)";
		getchar();
		return 0;
	}


	/*读入文法的产生式*/
	std::cout << "Please input the produces of the grammer:(format: A-BCDF,double-type the enter key to end input)" << std::endl;
	std::getline(std::cin, *pstr);
	while(pstr->size() != 0)
	{
		*pc = (*pstr)[0];
		pstr->erase(0, 2);
		ppair->first = *pc;
		ppair->second = *pstr;
		pproduces->insert(*ppair);
		std::getline(std::cin, *pstr);
	}
	/*检测文法开始符号的准确性*/
	//检测个毛，去死吧

	/*正式创建该文法*/
	Grammer grammer(*pvt, *pvn, *ps, *pproduces);

	/*释放一波内存*/
	delete pc, pstr, pvt, pvn, ps, pproduces, ppair;

	/*消除左递归*/
	grammer.EliminateLeftRecursion();
	std::cout << "After eliminate the left recursion, the new grammmer is as follow:" << std::endl;
	grammer.PrintGrammer();

	/*求得Follow集*/
	grammer.GetAllTheFollowSets();

	//构造预测分析表
	std::set<tableunit> table;
	tableunit tempunit;
	auto pros = grammer.getProduces();
	for (auto i : pros)
	{
		std::set<char> tempset = grammer.GetFirstSet(i.second);
		for (auto j : tempset)
		{
			if (j != 0)
			{

				auto bar = std::make_tuple(i.first, j, i);
				tempunit = bar;
				table.insert(tempunit);
			}
		}

		if (tempset.find(0) != tempset.end())
		{
			std::set<char> tempset2 = grammer.GetFollowSet(i.first);
			for (auto p : tempset2)
			{
				auto bar = std::make_tuple(i.first, p, i);
				tempunit = bar;
				table.insert(tempunit);
			}
		}

	}//对于每个产生式

	///*输出预测分析表*/
	//std::cout << "Here are the Forecast Analysis Table: " << std::endl;
	//for (auto i : table)
	//{
	//	std::cout << std::get<0>(i) << "  " << std::get<1>(i) << "  " << std::get<2>(i).first <<  " -> " << std::get<2>(i).second;
	//	std::cout << std::endl;
	//}

	int errorflag = 0;
	int numflag = 0;
	std::string anastr;
	std::string anastr2;
	std::cout << "Please input the string to analyze:";
	std::getline(std::cin, anastr);
	
	//做初步的词法分析
	for (auto i : anastr)
	{
		if (i <= '9' && i >= '0')
		{
			if (numflag == 0)
			{
				numflag = 1;
			}
		}
		else
		{
			if (numflag == 1)
			{
				anastr2.push_back('n');
				numflag = 0;
				
			}
			switch (i)
			{
			case '+': anastr2.push_back('+'); break;
			case '-': anastr2.push_back('-'); break;
			case '*': anastr2.push_back('*'); break;
			case '/': anastr2.push_back('/'); break;
			case '(': anastr2.push_back('('); break;
			case ')': anastr2.push_back(')'); break;
			default:
				std::cerr << "The result is : false!";
				getchar();
				getchar();
				return 0;
				break;
			}
			
		}
		
	}

	if (numflag == 1)
	{
		anastr2.push_back('n');
	}

	bool c = analyze(anastr2, table, grammer.getS(), grammer.getVt());

	std::cout << "The result is :" << c;
	getchar();
	getchar();
	return 0;
}



bool analyze(std::string s , std::set<tableunit> M , char S , std::set<char> vt)
{
	std::stack<char> analystack;
	analystack.push('$');
	analystack.push(S);
	s.push_back('$');
	auto ip = s.begin();
	char x = analystack.top();
	do {
		if (x == '$' || vt.find(x) != vt.end())
		{
			if (x == *ip)
			{
				analystack.pop();
				x = analystack.top();
				ip++;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			tableunit tu;
			int flag = 0;
			for (auto i : M)
			{
				if (std::get<0>(i) == x && std::get<1>(i) == *ip)
				{
					tu = i;
					flag = 1;
					break;
				}
			}


			if( flag ==1)
			{
				analystack.pop();
				std::string ss = std::get<2>(tu).second;
				int sss = ss.size();
				sss--;
				while(sss>=0)
				{
					analystack.push(ss[sss]);
					sss--;
				}
				x = analystack.top();
				
				std::cout << std::get<2>(tu).first << "->" << std::get<2>(tu).second << std::endl;
			}
			else
			{
				return 0;
			}
		}
	} while (x != '$');

	return true;
}
