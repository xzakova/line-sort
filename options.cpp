#include "options.h"
#include <string>


std::optional<std::tuple<Order, Filter, Case, char *>> options::parse(int argc, char * argv[])
{
	Order order{ Order::ascending };
	Filter filter{ Filter::all };
	Case compare{ Case::sensitive };
	char * input{ nullptr };

	//parse commandline options
	int pos = 1;
	for (; pos < argc; ++pos)
	{
		std::string arg = argv[pos];
		if (arg.empty() || arg[0] != '-')
			break;

		if (arg == "-u")
		{
			if (filter != Filter::all)
				return {};
			filter = Filter::unique;
		}
		else if (arg == "-r")
		{
			if (order != Order::ascending)
				return {};
			order = Order::descending;
		}
		else if (arg == "-i")
		{
			if (compare != Case::sensitive)
				return {};
			compare = Case::ignore;
		}
		else
		{
			return {};
		}
	}

	if (pos < argc)
	{
		input = argv[pos++];
	}

	if (pos < argc)
	{
		return {};
	}

	return std::make_tuple(order, filter, compare, input);
}