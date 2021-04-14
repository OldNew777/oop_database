#ifndef TABLEIN_H_INCLUDED
#define TABLEIN_H_INCLUDED

#include <list>
#include <string>
#include <map>
#include <utility>
#include <vector>

enum nullornot
{
    NOTNULL = 0,
    CANNULL = 1
};

class node
{
public:
	virtual ~node() {};			//空的虚函数为了能用多态
	std::string name;				//变量名称
};

class label_info
{									//表头，存储变量名称等
public:
	label_info() {};
	std::string name;
	std::string type;				//变量类型
	nullornot non;					//是否可为空
	std::string Default = "NULL";	//第一阶段不考虑
	label_info(const label_info &a);
};

template <typename t> class listnode:public node
{
public:
	t data;
	listnode(const t& val) :data(val) {}
	listnode() {};
	~listnode() {};
};

class table{
	std::string tablename;
	int prikey_position;			//PRIMARY KEY在label里的位置
	std::vector<label_info> label;	//表头，存储了变量的属性信息
	std::map<std::string, std::vector<node*>> datalist;			//主键为map的first，second是向上类型转换的node指针
public:
	table() {};
	table(const std::string name, const int &keyposition, const std::vector<label_info> v, int size = 0);
	~table();
    void insert_data();				//插入新数据
    void delete_data(const std::vector <std::string> &keys);	//依照keys里提供的主键删除数据
	void delete_data();											//清空数据
    void update();					//updata
	void select(std::vector <std::string> str);
	void show_columns();							
	std::vector <std::string> whereclause_frag(int index, std::string str1, char operate);	//具体到单次比较
	std::vector <std::string> whereclause_complete();			//计算符合whereclause的主键vector
	inline int place(const std::string &listname)
	{
		for (auto k = label.begin(); k < label.end(); k++)
			if (k->name == listname)
				return (k - label.begin());
		return 0;
	}
};

#endif 
// TABLEIN_H_INCLUDED
