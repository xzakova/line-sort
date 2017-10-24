#include "options.h"
#include <iostream>
#include <vector>

std::vector<std::string> rozbi_multiple(char *retazec)
{
	
	char pomlcka = '-';
	
	std::vector<std::string> parametre;
	std::string s = std::string(retazec);
	size_t pozicia = 0;
	std::string par;
	

	while ((pozicia = s.find(pomlcka)) != std::string::npos)
	{
		par = s.substr(1, 1);
		parametre.push_back(par);
		s.erase(0, pozicia + 2);
	}

	return parametre;
}


std::optional<std::tuple<Order, Filter, Case, char *>> options::parse(int argc, char * argv[])
{
	Order order{ Order::ascending };
	Filter filter{ Filter::all };
	Case compare{ Case::sensitive };
	char * input{ nullptr };


	if (argc == 1)
		return std::make_tuple(order, filter, compare, input);
	if (argc == 3)
		input = argv[2];

	
	int dlzka = strlen(argv[1]);

	if (dlzka != 2 || dlzka != 4 || dlzka != 6)							//pre pripad ze robime obyc. line sort so suborom
	{
		input = argv[1];
		return std::make_tuple(order, filter, compare, input);

	}
	else
	{
		std::vector<std::string> viacparam;								
		viacparam = rozbi_multiple(argv[1]);


		for (std::string i : viacparam)
		{
			if (i == "r")
				order = { Order::descending };
			if (i == "u")
				filter = { Filter::unique };
			if (i == "i")
				compare = { Case::ignore };

			else return options::parse(argc, argv); //{}
				
		}

	}

	


	return std::make_tuple(order, filter, compare, input);



}
