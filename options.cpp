#include "options.h"
#include <iostream>
#include <vector>



std::optional<std::tuple<Order, Filter, Case, char *>> options::parse(int argc, char * argv[])
{
	Order order{ Order::ascending };
	Filter filter{ Filter::all };
	Case compare{ Case::sensitive };
	char * input{ nullptr };

	//iba pre obyc line sort
	if (argc == 1)
		return std::make_tuple(order, filter, compare, input);

	//ocakavame vstup ako ("line-sort","-r","-i")  cize zvlast kazdy parameter podla ktoreho triedime
	//urobime bez posledneho, to nevieme ci je prepinac alebo subor

	for (int i = 1; i < argc-1; i++)
	{
		std::string s = std::string(argv[i]); 
		s.erase(0, 1);

		if (s == "r")
			order = { Order::descending };
		if (s == "u")
			filter = { Filter::unique };
		if (s == "i")
			compare = { Case::ignore };

		else return options::parse(argc, argv); //{}


	}

	//tu riesime posledny, ak ma dlzku dva je to prepinac, ak nie je to subor. plus ak je to zly prepinac tak vrati chzbu

	if (strlen(argv[argc - 1]) == 2)
	{
		std::string s = std::string(argv[argc-1]);
		s.erase(0, 1);

		if (s == "r")
			order = { Order::descending };
		if (s == "u")
			filter = { Filter::unique };
		if (s == "i")
			compare = { Case::ignore };

		else if (strlen(argv[argc - 1]) != 2)
		{
			input = argv[argc-1];
		}
		else return options::parse(argc, argv); //{}

	}

	
	
	

	


	return std::make_tuple(order, filter, compare, input);



}
