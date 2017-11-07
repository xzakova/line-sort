#pragma once

#include <fstream>
#include <ostream>

#include "defs.h"





namespace sort
{
	bool process(Order order, Filter filter, Case compare, std::istream & input, std::ostream & output);
}

