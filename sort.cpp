#include "sort.h"

#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iostream>
#include <fstream>
#include <set>
#include <utility>


namespace
{
	struct Line : public std::string {};

	std::istream & operator >> (std::istream & is, Line & line)
	{
		return std::getline(is, line);
	}
}

bool sort::process(Order order, Filter filter, Case compare, FilterSpace medzera, std::istream & input, std::ostream & output)
{
	std::vector<std::string> lines{ std::istream_iterator<Line>(input), std::istream_iterator<Line>() };

	//ak najde medzerove riadky tak ich vymaze:
	if (medzera == FilterSpace::space)
	{
		bool sp = false;
		for (int i = 0; i < lines.size();)
		{
			for (int j = 0; j < lines[i].length(); j++)
			{
				unsigned char ch = lines[i][j];
				if (!std::isspace(ch)) sp = true;
			}
			if (sp == true) lines.erase(lines.begin() + i);
			else ++i;
		}

	}


	//ak robime pripad unique a ignore naraz tak si zmenime na vsetky znaky na rovnaku velkost aby sme mohli porovnavat string
	if (filter == Filter::unique && compare == Case::ignore)
	{
		for (int i = 0; i < lines.size(); i++) {
			for (int j = i+1; j < lines.size(); j++) {
					std::string v1 = lines[i];
					std::string v2 = lines[j];
					std::transform(v1.begin(), v1.end(), v1.begin(), std::toupper);
					std::transform(v2.begin(), v2.end(), v2.begin(), std::toupper);
					if (v1 == v2) {
						lines[i] = lines[j];
					}
			}
		}
	}

	//pre -u unique sort,  najprv zosortujem a potom vymazem posledne ktore su vlastne rovnake s predtym
	if (filter == Filter::unique)
	{
		auto last = std::unique(lines.begin(), lines.end());
		lines.erase(last, lines.end());
	}

	//-i   , case insensitive porovnavanie, t.j. male aj velke pismena berie rovnocenne
	if (compare == Case::ignore)
	{
		std::sort(lines.begin(), lines.end(), [](const std::string& a, const std::string& b) -> bool {
			for ( const char *ptrA = a.c_str(), *ptrB = b.c_str(); ; ++ptrA, ++ptrB)
			{
				if (*ptrA != *ptrB || !*ptrA || !*ptrB)	return *ptrA < *ptrB;
			}

			return false; });
	}
	else {
		//a zosortujem normalne
		std::sort(lines.begin(), lines.end());
	}
	
	//pre pridapd -r reverse/descending
	if (order == Order::descending)
	{
		std::reverse(lines.begin(), lines.end());
	}


	for (int i = 0; i < lines.size(); i++)
	{
		output << lines[i] << std::endl;
	}



	return true;
}


