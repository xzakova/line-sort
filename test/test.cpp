#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "options.h"
#include "sort.h"

#include <sstream>



TEST_CASE("Options")
{
	SECTION("cin")
	{
		SECTION("no options")
		{
			char * argv[] = { "line-sort" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.has_value());
			REQUIRE(options == std::make_tuple(Order::ascending, Filter::all, Case::sensitive, (char *) nullptr));
		}

		SECTION("reversed")
		{
			char * argv[] = { "line-sort" , "-r" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::descending, Filter::all, Case::sensitive, (char *) nullptr));
		}

		SECTION("unique")
		{
			char * argv[] = { "line-sort" , "-u" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::ascending, Filter::unique, Case::sensitive, (char *) nullptr));
		}

		SECTION("ignore case")
		{
			char * argv[] = { "line-sort", "-i" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::ascending, Filter::all, Case::ignore, (char *) nullptr));
		}

		SECTION("multiple")
		{
			char * argv[] = { "line-sort", "-r-i" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::descending, Filter::all, Case::ignore, (char *) nullptr));
			char * argv2[] = { "line-sort", "-u-i" };
			REQUIRE(options::parse(_countof(argv2), argv2) == std::make_tuple(Order::ascending, Filter::unique, Case::ignore, (char *) nullptr));
			char * argv3[] = { "line-sort", "-r-u-i" };
			REQUIRE(options::parse(_countof(argv3), argv3) == std::make_tuple(Order::descending, Filter::unique, Case::ignore, (char *) nullptr));
			char * argv4[] = { "line-sort", "-r-u" };
			REQUIRE(options::parse(_countof(argv4), argv4) == std::make_tuple(Order::descending, Filter::unique, Case::sensitive, (char *) nullptr));

		}

		SECTION("bad")
		{
			char * argv[] = { "line-sort", "-u-r-l" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE( !options.has_value() );
		}
		

	}



	SECTION("file")
	{
		SECTION("no options")
		{
			char * argv[] = { "line-sort" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::ascending, Filter::all, Case::sensitive, argv[1]));
		}
		SECTION("reversed")
		{
			char * argv[] = { "line-sort" , "-r" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::descending, Filter::all, Case::sensitive, argv[2]));
		}
		SECTION("unique")
		{
			char * argv[] = { "line-sort" , "-u" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::ascending, Filter::unique, Case::sensitive, argv[2]));
		}

		SECTION("ignore case")
		{
			char * argv[] = { "line-sort" , "-i" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::ascending, Filter::all, Case::ignore, argv[2]));
		}

		SECTION("multiple")
		{
			char * argv[] = { "line-sort" , "-r-u" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::descending, Filter::unique, Case::sensitive, argv[2]));
			char *argv2[] = { "line-sort" , "-r-i" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv2), argv2) == std::make_tuple(Order::descending, Filter::all, Case::ignore, argv[2]));
			char *argv3[] = { "line-sort" , "-i-u" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv3), argv3) == std::make_tuple(Order::ascending, Filter::unique, Case::ignore, argv[2]));
			char *argv4[] = { "line-sort" , "-i-u-r" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv4), argv4) == std::make_tuple(Order::descending, Filter::unique, Case::ignore, argv[2]));

		}

		SECTION("bad")
		{
			char * argv[] = { "line-sort", "-u-r-l" , "subor.txt"};
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(!options.has_value());
			

		}
	}

	

	








}

namespace
{
	namespace data
	{
		std::istringstream empty { R"()" };
		std::istringstream one_char { R"( )" };
		std::istringstream one_line { R"(
)" };
		std::istringstream numbered { R"(1
3
9
4
5
2
)" };
	}
}

TEST_CASE("Sorting")
{
	SECTION("ascending")
	{
		std::ostringstream output {};

		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, data::empty, output) == true);
		REQUIRE(output.str() == "");

		//output.str("");
		//REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, data::one_char, output) == true);
		//REQUIRE(output.str() == " \n");

		//output.str("");
		//REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, data::one_line, output) == true);
		//REQUIRE(output.str() == "\n");

		//output.str("");
		//REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, data::numbered, output) == true);
		//REQUIRE(output.str() == "1\n2\n3\n4\n5\n9\n");
	}

	SECTION("ascending - unique")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::sensitive, data::empty, output) == true);
		REQUIRE(output.str() == "");
	}

	SECTION("ascending - unique - ignore case")
	{
	}

	SECTION("descending")
	{
	}

	SECTION("descending - unique")
	{
	}

	SECTION("descending - unique - ignore case")
	{
	}
}

