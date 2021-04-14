#ifndef SKIPSPECIALCHAR_H_INCLUDED
#define SKIPSPECIALCHAR_H_INCLUDED

#include <iostream>
#include <map>

inline void skip()
{
	std::map <char, bool> judge =
	{ {' ', true}, {';', true}, {'\r', true}, {'\n', true}, {'=', true},
	{',', true}, {'(', true}, {')', true}, {char(34), true}, {char(39), true} };
    while (judge[std::cin.peek()])
        std::getchar();
}                                                   //Ìø¹ýÌØÊâ×Ö·û¶ÁÈë


#endif // SKIPSPECIALCHAR_H_INCLUDED
