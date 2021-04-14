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
	virtual ~node() {};			//�յ��麯��Ϊ�����ö�̬
	std::string name;				//��������
};

class label_info
{									//��ͷ���洢�������Ƶ�
public:
	label_info() {};
	std::string name;
	std::string type;				//��������
	nullornot non;					//�Ƿ��Ϊ��
	std::string Default = "NULL";	//��һ�׶β�����
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
	int prikey_position;			//PRIMARY KEY��label���λ��
	std::vector<label_info> label;	//��ͷ���洢�˱�����������Ϣ
	std::map<std::string, std::vector<node*>> datalist;			//����Ϊmap��first��second����������ת����nodeָ��
public:
	table() {};
	table(const std::string name, const int &keyposition, const std::vector<label_info> v, int size = 0);
	~table();
    void insert_data();				//����������
    void delete_data(const std::vector <std::string> &keys);	//����keys���ṩ������ɾ������
	void delete_data();											//�������
    void update();					//updata
	void select(std::vector <std::string> str);
	void show_columns();							
	std::vector <std::string> whereclause_frag(int index, std::string str1, char operate);	//���嵽���αȽ�
	std::vector <std::string> whereclause_complete();			//�������whereclause������vector
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
