#ifndef ALL_H_INCLUDED
#define ALL_H_INCLUDED

#include "databasein.h"
#include <map>
#include <utility>
#include <string>

class all
{
private:
    std::map<std::string, database> info_database;		//��name��database��map
public:
	all() {};
    ~all();
    void create_database(const std::string &name);
    void delete_database(const std::string &name);
    void show_databases();
	inline database* operator[](const std::string &name)
	{
		return &(info_database[name]);
	}									//����[]����������Ϊname��databaseָ��
};

#endif // ALL_H_INCLUDED
