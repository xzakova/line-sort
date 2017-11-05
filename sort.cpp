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

bool sort::process(Order order, Filter filter, Case compare, std::istream & input, std::ostream & output)
{
	std::vector<std::string> lines { std::istream_iterator<Line>(input), std::istream_iterator<Line>() };


	//nasjkor zosortujem normalne pre pripad ze mame zadany len line-sort obyc
	std::sort(lines.begin(), lines.end());

	//-i   , case insensitive porovnavanie, t.j. male aj velke pismena berie rovnocenne
	if (compare == Case::ignore)
	{
		std::sort(lines.begin(), lines.end(), [](const std::string& a, const std::string& b) -> bool {
			for (size_t c = 0; c < a.size() && c < b.size(); c++) {
				if (std::tolower(a[c]) == std::tolower(b[c]))
					continue;
				if (std::tolower(a[c]) < std::tolower(b[c]))
					return true;
				else
					return false;
			}
			return a.size() < b.size();
		});
	}

	//pre pridapd -r reverse/descending
	if (order == Order::descending)
	{
		std::reverse(lines.begin(), lines.end());
	}


	//pre -u unique sort,  najprv zosortujem a potom vymazem posledne ktore su vlastne rovnake s predtym
	if (filter == Filter::unique)
	{
		std::sort(lines.begin(),lines.end());
		auto last = std::unique(lines.begin(), lines.end());
		lines.erase(last, lines.end());
	}
	




	for (int i = 0; i < lines.size(); i++)
	{
		output << lines[i] << std::endl;
	}

	



	return true;
}


