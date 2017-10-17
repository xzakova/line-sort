#include "options.h"

std::optional<std::tuple<Order, Filter, Case, char *>> options::parse(int argc, char * argv[])
{
	Order order{ Order::ascending };
	Filter filter{ Filter::all };
	Case compare{ Case::sensitive };
	char * input{ nullptr };

	if (argc == 1)
		return std::make_tuple(order, filter, compare, input);


	// parse commandline options


	if (argc==2)
	{ 
		if (argv[1] != "-r" || argv[1] != "-u" || argv[1] != "-i")
		{
			return{};
		}
		if (argv[1] == "-r")
		{
			order={ Order::descending };
			filter={ Filter::all };
			compare={ Case::sensitive };
			input={ nullptr };
		}
		if (argv[1] == "-u")
		{
			order = { Order::ascending };
			filter = { Filter::unique };
			compare = { Case::sensitive };
			input = { nullptr };
		}
		if (argv[1] == "-i")
		{
			order = { Order::ascending };
			filter = { Filter::all };
			compare = { Case::ignore };
			input = { nullptr };
		}
		if (argv[1] != nullptr)
		{
			order = { Order::ascending };
			filter = { Filter::all };
			compare = { Case::ignore };
			input = { argv[2] };
		}
		else
		{
			order = { Order::ascending };
			filter = { Filter::all };
			compare = { Case::sensitive };
			input = { nullptr };
		}
	}
	

	if (argc==3)
	{
		if (argv[1] != "-r" || argv[1] != "-u" || argv[1] != "-i")
		{
			return{};
		}
		if (argv[1] == "-r")
		{
			order = { Order::descending };
			filter = { Filter::all };
			compare = { Case::sensitive };
			input = { argv[2] };
		}
		if (argv[1] == "-u")
		{
			order = { Order::ascending };
			filter = { Filter::unique };
			compare = { Case::sensitive };
			input = { argv[2] };
		}
		if (argv[1] == "-i")
		{
			order = { Order::ascending };
			filter = { Filter::all };
			compare = { Case::ignore };
			input = { argv[2] };
		}

	}
	

	return std::make_tuple(order, filter, compare, input);



}
