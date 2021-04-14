#ifndef DATABASEIN_CPP_INCLUDED
#define DATABASEIN_CPP_INCLUDED

#include "databasein.h"
#include "getstring.h"
#include "tablein.h"
#include <iostream>
#include <list>
#include <string>
#include "vague_equal.h"

void database::delete_table(const std::string& name) {
	auto it = datatable.find(name);
	if (it == datatable.end())
		return;
	it->second.delete_data();
	datatable.erase(it);
	return;
}

void database::delete_table() {
	for (auto it = datatable.begin(); it != datatable.end(); it++)
		it->second.delete_data();
	std::map<std::string, table> map_empty;
	datatable.swap(map_empty);
}

void database::show_tables()
{
	std::cout << "Tables_in_" << databasename << std::endl;
	for (auto i = datatable.begin(); i != datatable.end(); i++)
		std::cout << i->first << std::endl;
}

database::~database()
{
	for (auto it = datatable.begin(); it != datatable.end(); it++)
		it->second.delete_data();
	std::map<std::string, table> map_empty;
	datatable.swap(map_empty);
}

void database::create_table(const std::string &name) {			//建立新表，并将表头存好
	std::vector<label_info> v;				//表头的vector
	label_info tmp;							//表头信息中转
	int keyposition;
	std::string order;
	std::string type;
	bool flag = false;
	while(std::cin.peek() != ';') {
		order = getstring();
		if (order == "PRIMARY") {
			getstring();
			order = getstring();
			getchar();
			getchar();
			for (auto it = v.begin(); it != v.end(); it++) {
				if (it->name == order) {
					keyposition = it - v.begin();
					break;
				}
			}
		}
		else {
			tmp.name = order;
			tmp.type = getstring();
			getchar();
			if (std::cin.peek()== 'n' || std::cin.peek() == 'N')
			{	
				std::string t1 = getstring();
				std::string t2 = getstring();
				if (!vague_equal(t1, "NOT") || !vague_equal(t2, "NULL"))
					goto CANNULL_judge;
				tmp.non = NOTNULL;
			}
			else
			{
				CANNULL_judge:
				tmp.non = CANNULL;
			}
			v.push_back(tmp);
		}
	}
	datatable[name] = table(name, keyposition, v);
	return;
}

#endif//DATABASEIN_CPP_INCLUDED
