#ifndef TABLE_CPP_INCLUDED
#define TABLE_CPP_INCLUDED

#include "tablein.h"
#include "getstring.h"
#include "skipspecialchar.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include "vague_equal.h"
#include <algorithm>

label_info::label_info(const label_info &a)
{
	name = a.name;
	type = a.type;
	non = a.non;
	Default = a.Default;
}

table::table(const std::string name, const int &keyposition, const std::vector<label_info> v, int size)
{
	tablename = name;
	prikey_position = keyposition;
	label = v;
}

table::~table()
{
	for (auto i = datalist.begin(); i != datalist.end(); i++)
		for (auto j = i->second.begin(); j < i->second.end(); j++)
			if (*j)
			{
				delete *j;
				*j = nullptr;
			}
	std::map<std::string, std::vector<node*>> map_empty;
	datalist.swap(map_empty);
	std::vector<label_info> vector_empty;
	label.swap(vector_empty);
}

void table::show_columns() {
	std::cout << "Field\tType\tNull\tKey\tDefault\tExtra\n";
	for (auto it = label.begin(); it != label.end(); it++) {
		std::cout << it->name << '\t';
		if (vague_equal(it->type, "int"))
			std::cout << "int(11)\t";
		else if (vague_equal(it->type, "char"))
			std::cout << "char(1)\t";
		else if (vague_equal(it->type, "double"))
			std::cout << "double\t";
		if (it->non == NOTNULL)
			std::cout << "NO\t";
		else
			std::cout << "YES\t";
		if (it - label.begin() == this->prikey_position)
			std::cout << "PRI\t";
		else
			std::cout << '\t';
		std::cout << it->Default << std::endl;			//目前没用
	}
}

void table::insert_data()
{
	int* num = new int[label.size()];
	int variable_num = 0;
	for (unsigned int i = 0; i < label.size(); i++)
	{
		if (std::cin.peek() == ')')
			break;
		++variable_num;					//这两句用来计算真正读入的数据个数（有些数据可为空）
		std::string s = getstring();
		for (auto k = label.begin(); k < label.end(); k++)
			if (k->name == s)
			{
				num[i] = k - label.begin();
				break;
			}
	}
	getstring();
	std::vector<node*> f(label.size());
	std::string key;
	for (int i = 0; i < variable_num; i++)
	{
		skip();
		if (vague_equal(label[num[i]].type, "int"))
		{
			int flag;
			std::cin >> flag;
			if (num[i] == prikey_position)
				key = std::to_string(flag);
			f[num[i]] = new listnode<int>(flag);
		}
		else if (vague_equal(label[num[i]].type, "char"))
		{
			std::string flag = getstring();
			if (num[i] == prikey_position)
				key = flag;
			f[num[i]] = new listnode<std::string>(flag);
		}
		else if (vague_equal(label[num[i]].type, "double"))
		{
			double flag;
			std::cin >> flag;
			if (num[i] == prikey_position)
				key = std::to_string(flag);
			f[num[i]] = new listnode<double>(flag);
		}
	}
	datalist.insert(make_pair(key, f));
	delete[] num;
}

void table::update() {
	getstring();
	std::string list_name_c = getstring();		//等待修改的项
	std::string changevalue = getstring();
	node* y;
	int c_place = place(list_name_c);
	if (std::cin.peek() == '"')
		getchar();
	std::vector <std::string> keys = whereclause_complete();
	for (int i = 0; i < keys.size(); i++) {
		if (vague_equal(label[c_place].type, "int")) {
			y = new (listnode<int>)(stoi(changevalue));
		}
		else if (vague_equal(label[c_place].type, "double"))
			y = new (listnode<double>)(stod(changevalue));
		else
			y = new (listnode<std::string>)(changevalue);
		datalist[keys[i]][c_place] = y;
		if (c_place == this->prikey_position) {
			std::vector<node*> f(label.size());
			for (int j = 0; j < label.size(); j++)
			{
				if (vague_equal(label[j].type, "int"))
				{
					int flag;
					listnode<int>* p = static_cast<listnode<int>*> (datalist[keys[i]][j]);
					flag = p->data;
					f[j]  = new listnode<int>(flag);
				}
				else if (vague_equal(label[j].type, "char"))
				{
					std::string flag;
					listnode<std::string>* p = static_cast<listnode<std::string>*> (datalist[keys[i]][j]);
					flag = p->data;
					f[j] = new listnode<std::string>(flag);
				}
				else if (vague_equal(label[j].type, "double"))
				{
					double flag;
					listnode<double>* p = static_cast<listnode<double>*> (datalist[keys[i]][j]);
					flag = p->data;
					f[j] = new listnode<double>(flag);
				}
			}
			datalist.erase(datalist.find(keys[i]));
			datalist.insert(make_pair(changevalue, f));
		}
	}
}

void table::delete_data(const std::vector <std::string> &keys)
{
	for (int i = 0; i < keys.size(); i++)
	{
		for (auto it = datalist.find(keys[i])->second.begin(); it != datalist.find(keys[i])->second.end(); it++)
			if (*it)
			{
				delete *it;
				*it = nullptr;
			}
		datalist.erase(datalist.find(keys[i]));
	}
	return;
}

void table::delete_data()
{
	for (auto i = datalist.begin(); i != datalist.end(); i++)
		for (auto j = i->second.begin(); j < i->second.end(); j++)
			if (*j)
			{
				delete *j;
				*j = nullptr;
			}
	std::map<std::string, std::vector<node*>> map_empty;
	datalist.swap(map_empty);
	std::vector<label_info> vector_empty;
	label.swap(vector_empty);
	return;
}

void table::select(std::vector <std::string> lists) {
	std::vector <std::string> all_lists;		//待输出的变量名
	if (lists[0] == "*") {
		for (int i = 0; i < label.size(); i++) {
			all_lists.push_back(label[i].name);
		}
	}
	else {
		all_lists = lists;
	}

	std::vector <std::string> keys = whereclause_complete();
	if (keys.size() == 0)
		return;					//如果没有满足条件的，则略去表头输出

	for (int i = 0; i < all_lists.size(); i++) {
		std::cout << all_lists[i] << "\t";
	}							//select的变量名输出

	std::cout << std::endl;
	if (vague_equal(this->label[this->prikey_position].type, "char")) {
		std::sort(keys.begin(), keys.end());
	}
	else if (vague_equal(this->label[this->prikey_position].type, "int")) {
		std::map<int, std::string> tmp;
		for (unsigned int i = 0; i < keys.size(); i++) {
			tmp.insert(make_pair(stoi(keys[i]), keys[i]));
		}
		int k = 0;
		for (auto i = tmp.begin(); i != tmp.end(); i++) {
			keys[k++] = i->second;
		}
	}
	else {
		std::map<double, std::string> tmp;
		for (unsigned int i = 0; i < keys.size(); i++) {
			tmp.insert(make_pair(stod(keys[i]), keys[i]));
		}
		int k = 0;
		for (auto i = tmp.begin(); i != tmp.end(); i++) {
			keys[k++] = i->second;
		}
	}												//利用map将待输出的keys按字典序排序

	for (int i = 0; i < keys.size(); i++) {
		for (int j = 0; j < all_lists.size(); j++) {
			int x = place(all_lists[j]);
			if (label[x].non == CANNULL && datalist[keys[i]][x] == nullptr)
			{
				std::cout << "NULL\t";
				continue;
			}
			if (vague_equal(label[x].type, "int")) {
				listnode<int>* p = static_cast<listnode<int>*> (datalist[keys[i]][x]);
				std::cout << p->data << "\t";
			}
			else if (vague_equal(label[x].type, "double")) {
				listnode<double>* p = static_cast<listnode<double>*> (datalist[keys[i]][x]);
				std::cout << std::fixed << std::setprecision(4) << p->data << "\t";

			}
			else {
				listnode<std::string>* p = static_cast<listnode<std::string>*> (datalist[keys[i]][x]);
				std::cout << p->data << "\t";
			}
		}
		std::cout << std::endl;
	}
}

std::vector <std::string> table::whereclause_frag(int index, std::string value, char operate) { //分类讨论，根据符号和数据类型执行不同操作
	std::vector <std::string> keys;
	if (operate == '=') {
		if (vague_equal(label[index].type, "INT")) {
			int val = stoi(value);
			auto it = datalist.begin();         //遍历，寻找所有符合条件的元素
			while (it != datalist.end()) {
				if (static_cast<listnode<int>*> (it->second[index]) == NULL) {       //跳过空指针
					it++;
					continue;
				}
				listnode<int>* p = static_cast<listnode<int>*> (it->second[index]);
				if (p->data == val) {
					keys.push_back(it->first);          //将符合条件的元素的主键放入keys中
				}
				it++;
			}
		}
		else if (vague_equal(label[index].type, "DOUBLE")) {
			double val = stod(value);
			auto it = datalist.begin();
			while (it != datalist.end()) {
				if (static_cast<listnode<double>*> (it->second[index]) == NULL) {
					it++;
					continue;
				}
				listnode<double>* p = static_cast<listnode<double>*> (it->second[index]);
				if (p->data == val) {
					keys.push_back(it->first);
				}
				it++;
			}
		}
		else if (vague_equal(label[index].type, "CHAR")) {
			auto it = datalist.begin();
			while (it != datalist.end()) {
				if (static_cast<listnode<std::string>*> (it->second[index]) == NULL) {
					it++;
					continue;
				}
				listnode<std::string>* p = static_cast<listnode<std::string>*> (it->second[index]);
				if (p->data == value) {
					keys.push_back(it->first);
				}
				it++;
			}
		}
	}
	else if (operate == '<') {
		if (vague_equal(label[index].type, "INT")) {
			int val = stoi(value);
			auto it = datalist.begin();
			while (it != datalist.end()) {
				if (static_cast<listnode<int>*> (it->second[index]) == NULL) {
					it++;
					continue;
				}
				listnode<int>* p = static_cast<listnode<int>*> (it->second[index]);
				if (p->data < val) {
					keys.push_back(it->first);
				}
				it++;
			}
		}
		else if (vague_equal(label[index].type, "DOUBLE")) {
			double val = stod(value);
			auto it = datalist.begin();
			while (it != datalist.end()) {
				if (static_cast<listnode<double>*> (it->second[index]) == NULL) {
					it++;
					continue;
				}
				listnode<double>* p = static_cast<listnode<double>*> (it->second[index]);
				if (p->data < val) {
					keys.push_back(it->first);
				}
				it++;
			}
		}
		else if (vague_equal(label[index].type, "CHAR")) {
			auto it = datalist.begin();
			while (it != datalist.end()) {
				if (static_cast<listnode<std::string>*> (it->second[index]) == NULL) {
					it++;
					continue;
				}
				listnode<std::string>* p = static_cast<listnode<std::string>*> (it->second[index]);
				if (p->data < value) {
					keys.push_back(it->first);
				}
				it++;
			}
		}
	}
	else if (operate == '>') {
		if (vague_equal(label[index].type, "INT")) {
			int val = stoi(value);
			auto it = datalist.begin();
			while (it != datalist.end()) {
				if (static_cast<listnode<int>*> (it->second[index]) == NULL) {
					it++;
					continue;
				}
				listnode<int>* p = static_cast<listnode<int>*> (it->second[index]);
				if (p->data > val) {
					keys.push_back(it->first);
				}
				it++;
			}
		}
		else if (vague_equal(label[index].type, "DOUBLE")) {
			double val = stod(value);
			auto it = datalist.begin();
			while (it != datalist.end()) {
				if (static_cast<listnode<double>*> (it->second[index]) == NULL) {
					it++;
					continue;
				}
				listnode<double>* p = static_cast<listnode<double>*> (it->second[index]);
				if (p->data > val) {
					keys.push_back(it->first);
				}
				it++;
			}
		}
		else if (vague_equal(label[index].type, "CHAR")) {
			auto it = datalist.begin();
			while (it != datalist.end()) {
				if (static_cast<listnode<std::string>*> (it->second[index]) == NULL) {
					it++;
					continue;
				}
				listnode<std::string>* p = static_cast<listnode<std::string>*> (it->second[index]);
				if (p->data > value) {
					keys.push_back(it->first);
				}
				it++;
			}
		}
	}
	return keys;
}

std::vector <std::string> table::whereclause_complete() {		//返回符合条件的主键
	bool judged = false;										// 判断是否调用了whereclause，若没有，则返回所有主键
	std::vector <std::string> keys;
	std::vector<std::vector<std::string>> temp_or_keys;			//存储or语句连接的主键
	while (std::cin.peek() != ';' && std::cin.peek() != '"') {	//whereclausej是否结束
		std::string andor = getstring();						//记录and or 也有可能是where
		std::string str1 = getstring();							//记录运算符前的内容
		char operate;
		std::cin >> operate;									//记录运算符< > =
		std::string str2 = getstring();							//记录运算符后的内容
		std::vector <std::string> tmp_keys;
		for (int i = 0; i < label.size(); i++) {				//判断str1和str2哪个是列表的名字，哪个是数值，并将其传入whereclause_frag函数中进行< > =运算符的运算。运算结果储存在std::vector <std::string> tmp_keys中
			if (str1 == label[i].name) {
				tmp_keys = whereclause_frag(i, str2, operate);
			}
			else if (str2 == label[i].name) {
				if (operate == '>') {
					tmp_keys = whereclause_frag(i, str1, '<');
				}
				else if (operate == '<')
					tmp_keys = whereclause_frag(i, str1, '>');
				else {
					tmp_keys = whereclause_frag(i, str1, operate);
				}
			}
		}

		if (vague_equal(andor, "WHERE")) {
			judged = true;								//存在where语句
			keys = tmp_keys;
			continue;
		}
		else if (vague_equal(andor, "OR")) {
			temp_or_keys.push_back(keys);				//将之前得到的结果压栈，在循环外统一进行OR的运算
			keys = tmp_keys;							//把keys替换，对后续的AND表达式进行运算
		}
		else if (vague_equal(andor, "AND")) {
			std::vector <std::string> key_helper;
			for (int i = 0; i < tmp_keys.size(); i++) {
				auto it = find(keys.begin(), keys.end(), tmp_keys[i]);
				if (it != keys.end()) {					//如果tmp_keys中的元素在keys里面，则把共同元素加入key_helper中
					key_helper.push_back(tmp_keys[i]);
				}
			}
			keys = key_helper;
		}
	}

	int cnt = (int)temp_or_keys.size();
	for (int i = 0; i < cnt; i++) {
		for (int j = 0; j < temp_or_keys[i].size(); j++) {
			auto it = find(keys.begin(), keys.end(), temp_or_keys[i][j]);
			if (it == keys.end()) {						//如果temp_or_keys中的元素不在keys里面，则把该元素加入key_helper中
				keys.push_back(temp_or_keys[i][j]);
			}
		}
	}

	if (!judged) {										//没有进行where的运算，返回所有主键
		auto it = datalist.begin();
		while (it != datalist.end()) {
			keys.push_back(it->first);
			it++;
		}
	}									//当没有where时，所有数据都满足要求
	return keys;

}
#endif
