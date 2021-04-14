#ifndef VAGUE_EQUAL_H_INCLUDED
#define VAGUE_EQUAL_H_INCLUDED

#include <string>
#include <cmath>

inline bool vague_equal(const std::string &a, const std::string &b)
{
	if (a.length() != b.length())
		return false;
	for (int i = 0; i < a.length(); i++)
	{
		if (a[i] < 'A' || (a[i] > 'Z' && a[i] < 'a') || a[i] > 'z')
			return false;
		if (b[i] < 'A' || (b[i] > 'Z' && b[i] < 'a') || b[i] > 'z')
			return false;
		if (a[i] != b[i] && abs(int(a[i] - b[i])) != 32)
			return false;
	}
	return true;
}

#endif //VAGUE_EQUAL_H_INCLUDED