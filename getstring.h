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
}                                           //判断是否是特殊符号，是就打断读入

inline std::string getstring()
{
	skip();									//读入前略去一些特殊字符以及空格
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
	}                                   //带单引号或双引号的不论里面包含什么内容，全部读入
	while (unspecialchar())
	{
		a.append(1, getchar());
	}                                   //不带引号的读入（有打断）
	a[a.length()] = '\0';
	return std::move(a);
}                                       //注：特殊符号以及空格、换行符等等需要加一个函数忽略掉


#endif // GETSTRING_H_INCLUDED
