#pragma once

#include <iostream>

template <typename T>
bool assertEqual(const T &t2, const T &t1, const std::string &testName)
{
	if (t1 == t2)
	{
		std::cout << "\033[1;32mSuccess\033[0m > " << testName << " > assertEqual" << std::endl;
		return true;
	}
	std::cout << "\033[1;31mFail\033[0m: > " << testName << " > Expecting (" << t1 \
	<< ") got (" << t2 << ")" << std::endl;
	return false;
}

bool assertStringEqual(const std::string &s1, const std::string &s2, const std::string &testName)
{
	if (s1.compare(s2) == 0)
	{
		std::cout << "\033[1;32mSuccess\033[0m > " << testName << " > assertEqual" << std::endl;
		return true;
	}
	if (s1.length() != s2.length())
	{
		std::cout << "\033[1;31mFail\033[0m: > " << testName << " > Expecting len(" \
		<< s1.length() << ") got len(" << s2.length() << ")" << std::endl;
		return false;
	}
	for (unsigned long i = 0; i < s1.length(); i++)
	{
		if (s1[i] != s2[i])
		{
			std::cout << "\033[1;31mFail\033[0m: > " << testName << " > Expecting char(" \
			<< s1[i] << ") got char(" << s2[i] << ") at pos " << i << std::endl;
			return false;
		}
	}
	return false;
}
