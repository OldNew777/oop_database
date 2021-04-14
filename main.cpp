#include <iostream>
#include "getstring.h"
#include "skipspecialchar.h"
#include "all.h"
#include "databasein.h"
#include "tablein.h"
#include "vague_equal.h"

using namespace std;

int main()
{
    string order, name, attrname, value, type;
    all allinfo;
    database *baseptr = nullptr;			//database的指针，指向当前use的
    table *tableptr = nullptr;				//指向insert、delete、updata等操作的table
    while (cin.peek() != EOF)
    {
        order = getstring();
		if (order == "")
			break;
		if (vague_equal(order, "USE"))
		{
			name = getstring();
			baseptr = allinfo[name];		//database指针设置
		}
        else if (vague_equal(order, "CREATE"))
        {
            order = getstring();
            if (vague_equal(order, "DATABASE"))
            {
                name = getstring();
                allinfo.create_database(name);
            }
            else if (vague_equal(order, "TABLE"))
            {
                name = getstring();
				if (baseptr)
					baseptr->create_table(name);
            }
        }
        else if (vague_equal(order, "DROP"))
        {
            order = getstring();
            if (vague_equal(order, "DATABASE"))
            {
                name = getstring();
                allinfo.delete_database(name);
            }
            else if (vague_equal(order, "TABLE"))
            {
                name = getstring();
				if (baseptr)
					baseptr->delete_table(name);
            }
        }
        else if (vague_equal(order, "SHOW"))
        {
            order = getstring();
            if (vague_equal(order, "DATABASES"))
            {
                allinfo.show_databases();
            }
            else if (vague_equal(order, "TABLES"))
            {
				if (baseptr) {
					baseptr->show_tables();
				}
            }
            else if(vague_equal(order, "columns"))
			{
                getstring();
                name = getstring();
				if (baseptr)
					baseptr->show_columns(name);
            }
        }
		else if (vague_equal(order, "INSERT"))
		{
			getstring();
			name = getstring();
			if (baseptr)
				(*baseptr)[name]->insert_data();
		}
        else if (vague_equal(order, "DELETE"))
        {
            getstring();
            name = getstring();
            tableptr = (*baseptr)[name];
			if (tableptr == nullptr)
				continue;
			std::vector <std::string> keys = tableptr->whereclause_complete();		//用whereclause_complete()函数得出符合条件的数据的主键
            tableptr->delete_data(keys);
        }
        else if (vague_equal(order, "UPDATE"))
        {
            name = getstring();
            tableptr = (*baseptr)[name];
			if (tableptr == nullptr)
				continue;
            tableptr->update();
        }
		else if (vague_equal(order, "SELECT"))
		{
			std::string list_name = getstring();
			std::vector<std::string> all_lists;				//所有需要select列出来展示的变量
			while (!vague_equal(list_name, "FROM")) {
				all_lists.push_back(list_name);
				list_name = getstring();
			}
			std::string name = getstring();
			tableptr = (*baseptr)[name];
			tableptr->select(all_lists);					//展示table里所以all_lists里的变量
		}
    }
    return 0;
}
