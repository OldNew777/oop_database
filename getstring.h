#ifndef GETSTRING_H_INCLUDED
#define GETSTRING_H_INCLUDED

#include <iostream>
#include "skipspecialchar.h"

inline bool unspecialchar() {
	std::map <char, bool> judgespecial =
	{ {' ', true}, {';', true}, {'\r', true}, {'\n', true},
	{',', true}, {'(', true}, {')', true}, {'=', true}, {'<', true},
	{'>', true}, {EOF, true}, {char(34), true}, {char(39), true} };
	if (judgespecial[std::cin.peek()])
		return false;
	return true;
}                                           //�ж��Ƿ���������ţ��Ǿʹ�϶���

inline std::string getstring()
{
	skip();									//����ǰ��ȥһЩ�����ַ��Լ��ո�
	std::string a = "\0";
	if (std::cin.peek() == char(39) || std::cin.peek() == char(34))
	{
		std::getchar();
		do
		{
			a.append(1, getchar());
		} while (std::cin.peek() != char(39) && std::cin.peek() != char(34));
		std::getchar();
		a[a.length() + 1] = '\0';
		return std::move(a);
	}                                   //�������Ż�˫���ŵĲ����������ʲô���ݣ�ȫ������
	while (unspecialchar())
	{
		a.append(1, getchar());
	}                                   //�������ŵĶ��루�д�ϣ�
	a[a.length()] = '\0';
	return std::move(a);
}                                       //ע����������Լ��ո񡢻��з��ȵ���Ҫ��һ���������Ե�


#endif // GETSTRING_H_INCLUDED
