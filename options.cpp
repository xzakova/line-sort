#include "options.h"

std::optional<std::tuple<Order, Filter, Case, char *>> options::parse(int argc, char * argv[])
{
	Order order { Order::ascending };
	Filter filter { Filter::all };
	Case compare { Case::sensitive };
	char * input { nullptr };

	// parse commandline options

	return std::make_tuple(order, filter, compare, input);
}
