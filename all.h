#ifndef ALL_H_INCLUDED
#define ALL_H_INCLUDED

#include "databasein.h"
#include <map>
#include <utility>
#include <string>

class all
{
private:
    std::map<std::string, database> info_database;		//从name到database的map
public:
	all() {};
    ~all();
    void create_database(const std::string &name);
    void delete_database(const std::string &name);
    void show_databases();
	inline database* operator[](const std::string &name)
	{
		return &(info_database[name]);
	}									//重载[]，返回名称为name的database指针
};

#endif // ALL_H_INCLUDED
