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


//产生式规则：
//第一个元素为非终结符
//第二个元素为终结和非终结符组成的字符串或者空(ascii:0)（代表空串）
typedef std::pair<char, std::string> produce;
typedef std::pair<char, std::set<char>>firfolset;
typedef std::tuple<char, char, produce> tableunit;


/*--------文法类--------*/
class Grammer {
public:
	//构造函数，此类不支持默认构造函数
	Grammer(const std::set<char> &Vt_arg, const std::set<char> &Vn_arg,
		const char &S_arg, const std::set<produce> &Produces_arg);							//四元完整构造文法
	
	//功能函数
	bool EliminateLeftRecursion();															//消除左递归
	bool ExtractLeftCommonFactor();															//提取左公因子
	std::set<char> GetFirstSet(char symbol) const;										    //根据字符求First集
	std::set<char> GetFirstSet(std::string str) const;										//根据字符串求first集
	std::set<char> GetFollowSet(char symbol) const;											//求Follow集
	std::set<std::tuple<char, char, std::set <produce>>> GetForecastAnalysisTable() const;	//求预测分析表
	void GetAllTheFollowSets();																//求得所有的非终结符的Follow集
	
	//打印属性的函数
	void PrintVt() const;
	void PrintVn() const;
	void PrintS() const;
	void PrintProduces() const;
	void PrintGrammer() const;
	void PrintFollowSets() const;

	//外部接口，取得私有数据成员的拷贝
	std::set<char> getVn() const;
	std::set<produce> getProduces() const;
	char getS() const;
	std::set<char> getVt() const;

private:
	std::set<char> Vt;				//终结符的集合，必须非空,不能含有'$'符号
	std::set<char> Vn;				//非终结符的集合，必须非空,必须为大写字母
	char S;							//文法开始非终结符，属于Vn
	std::set<produce> Produces;		//产生式的集合
	std::set<firfolset> Followsets; //所有非终结符的follow集
};
/*---------------------*/


