#ifndef DATABASEIN_H_INCLUDED
#define DATABASEIN_H_INCLUDED

#include "tablein.h"
#include <map>
#include <utility>
#include <string>

class database
{
private:
    std::map<std::string, table> datatable;
public:
	std::string databasename;
	database() {};
	~database();
    void create_table(const std::string &name);	//������
    void delete_table(const std::string &name);	//ɾ��ĳһ��
	void delete_table();
    void show_tables();
    inline void show_columns(const std::string &name)
	{
		datatable[name].show_columns();
		return;
	}
	inline table* operator[](const std::string &name)
	{
		return &(datatable[name]);
	}											//����[]����������Ϊname��tableָ��
};

#endif // DATABASEIN_H_INCLUDED
