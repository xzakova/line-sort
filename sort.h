#pragma once

#include <fstream>
#include <ostream>

#include "defs.h"





namespace sort
{
	bool process(Order order, Filter filter, Case compare, FilterSpace medzera, std::istream & input, std::ostream & output);
}

