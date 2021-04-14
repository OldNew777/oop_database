#ifndef ALL_CPP_INCLUDED
#define ALL_CPP_INCLUDED

#include "databasein.h"
#include "all.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>

void all::show_databases()
{	
	std::cout << "Database" << std::endl;
	for (auto it = info_database.begin(); it != info_database.end(); it++) {
		std::cout << it->first << std::endl;
	}
}

all::~all()
{
	for (auto i = info_database.begin(); i != info_database.end(); i++)
		i->second.delete_table();
	std::map<std::string, database> map_empty;
	info_database.swap(map_empty);
}

void all::create_database(const std::string &name)
{
	database f;
	f.databasename = name;
	info_database.insert(std::make_pair(name, f));
}

void all::delete_database(const std::string & name)
{
	auto t = info_database.find(name);
	if (t == info_database.end())
		return;
	t->second.delete_table();
	info_database.erase(t);
}

#endif //ALL_CPP_INCLUDED