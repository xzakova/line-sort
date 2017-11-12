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
			REQUIRE(options == std::make_tuple(Order::ascending, Filter::all, Case::sensitive, FilterSpace::nospace, (char *) nullptr));
		}

		SECTION("reversed")
		{
			char * argv[] = { "line-sort" , "-r" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::descending, Filter::all, Case::sensitive, FilterSpace::nospace, (char *) nullptr));
		}

		SECTION("unique")
		{
			char * argv[] = { "line-sort" , "-u" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::ascending, Filter::unique, Case::sensitive, FilterSpace::nospace, (char *) nullptr));
		}

		SECTION("ignore case")
		{
			char * argv[] = { "line-sort", "-i" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::ascending, Filter::all, Case::ignore, FilterSpace::nospace, (char *) nullptr));
		}
		SECTION("space")
		{
			char * argv[] = { "line-sort" , "-e" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::ascending, Filter::all, Case::sensitive, FilterSpace::space, (char *) nullptr));
		}
		SECTION("multiple")
		{
			char * argv[] = { "line-sort", "-r","-i" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::descending, Filter::all, Case::ignore, FilterSpace::nospace, (char *) nullptr));
			char * argv2[] = { "line-sort", "-u","-i" };
			REQUIRE(options::parse(_countof(argv2), argv2) == std::make_tuple(Order::ascending, Filter::unique, Case::ignore, FilterSpace::nospace, (char *) nullptr));
			char * argv3[] = { "line-sort", "-r","-u","-i" };
			REQUIRE(options::parse(_countof(argv3), argv3) == std::make_tuple(Order::descending, Filter::unique, Case::ignore, FilterSpace::nospace, (char *) nullptr));
			char * argv4[] = { "line-sort", "-r","-u" };
			REQUIRE(options::parse(_countof(argv4), argv4) == std::make_tuple(Order::descending, Filter::unique, Case::sensitive, FilterSpace::nospace, (char *) nullptr));
			char * argv5[] = { "line-sort", "-r","-e" };
			REQUIRE(options::parse(_countof(argv5), argv5) == std::make_tuple(Order::descending, Filter::all, Case::sensitive, FilterSpace::space, (char *) nullptr));

		}

		SECTION("bad")
		{
			char * argv[] = { "line-sort", "-u","-r","-l" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE( !options.has_value() );
		}
		

	}



	SECTION("file")
	{
		SECTION("no options")
		{
			char * argv[] = { "line-sort" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::ascending, Filter::all, Case::sensitive, FilterSpace::nospace, argv[1]));
		}
		SECTION("reversed")
		{
			char * argv[] = { "line-sort" , "-r" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::descending, Filter::all, Case::sensitive, FilterSpace::nospace, argv[2]));
		}
		SECTION("unique")
		{
			char * argv[] = { "line-sort" , "-u" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::ascending, Filter::unique, Case::sensitive, FilterSpace::nospace, argv[2]));
		}

		SECTION("ignore case")
		{
			char * argv[] = { "line-sort" , "-i" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::ascending, Filter::all, Case::ignore, FilterSpace::nospace, argv[2]));
		}

		SECTION("multiple")
		{
			char * argv[] = { "line-sort" ,"-r","-u", "subor.txt" };
			REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::descending, Filter::unique, Case::sensitive,FilterSpace::nospace, argv[3]));
			char * argv2[] = { "line-sort" , "-r","-i" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv2), argv2) == std::make_tuple(Order::descending, Filter::all, Case::ignore,FilterSpace::nospace, argv[3]));
			char * argv3[] = { "line-sort" , "-i","-u" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv3), argv3) == std::make_tuple(Order::ascending, Filter::unique, Case::ignore, FilterSpace::nospace, argv[3]));
			char * argv4[] = { "line-sort" , "-r","-e" , "subor.txt" };
			REQUIRE(options::parse(_countof(argv4), argv4) == std::make_tuple(Order::descending, Filter::all, Case::sensitive, FilterSpace::space, argv[3]));

		}

		SECTION("bad")
		{
			char * argv[] = { "line-sort", "-u","-r","-l" , "subor.txt"};
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(!options.has_value());
			

		}
	}


}

namespace
{
	namespace data
	{
		std::istringstream empty() { return std::istringstream{ R"()" }; }
		std::istringstream one_char() { return std::istringstream{ R"( )" }; }
		std::istringstream one_line() {
			return std::istringstream{ R"(
)" };
		}
		std::istringstream numbered() {
			return std::istringstream{ R"(1
3
9
4
5
2
)" };
		}
		std::istringstream mixed() {
			return std::istringstream{ R"(
aZ
Az
B
c
D
 
Aa
E
f
G
Fx
)" };
		}
	}
}
TEST_CASE("Sorting")
{
	SECTION("ascending")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, FilterSpace::nospace, data::empty(), output) == true);
		REQUIRE(output.str() == "");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, FilterSpace::nospace, data::one_char(), output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive,FilterSpace::nospace, data::one_line(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, FilterSpace::nospace, data::numbered(), output) == true);
		REQUIRE(output.str() == "1\n2\n3\n4\n5\n9\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, FilterSpace::nospace, data::mixed(), output) == true);
		REQUIRE(output.str() == "\n \nAa\nAz\nB\nD\nE\nFx\nG\naZ\nc\nf\n");
	}
	SECTION("medzera")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, FilterSpace::space, data::empty(), output) == true);
		REQUIRE(output.str() == "");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, FilterSpace::space, data::one_char(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, FilterSpace::space, data::one_line(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, FilterSpace::space, data::numbered(), output) == true);
		REQUIRE(output.str() == "1\n2\n3\n4\n5\n9\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, FilterSpace::space, data::mixed(), output) == true);
		REQUIRE(output.str() == "\nAa\nAz\nB\nD\nE\nFx\nG\naZ\nc\nf\n");
	}

	SECTION("ascending - unique")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::sensitive, FilterSpace::nospace, data::empty(), output) == true);
		REQUIRE(output.str() == "");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::sensitive, FilterSpace::nospace, data::one_char(), output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::sensitive, FilterSpace::nospace, data::one_line(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::sensitive, FilterSpace::nospace, data::numbered(), output) == true);
		REQUIRE(output.str() == "1\n2\n3\n4\n5\n9\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::sensitive, FilterSpace::nospace, data::mixed(), output) == true);
		REQUIRE(output.str() == "\n \nAa\nAz\nB\nD\nE\nFx\nG\naZ\nc\nf\n");
	}

	SECTION("ascending - ignore case")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::ascending, Filter::all, Case::ignore,FilterSpace::nospace, data::empty(), output) == true);
		REQUIRE(output.str() == "");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::ignore, FilterSpace::nospace, data::one_char(), output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::ignore, FilterSpace::nospace, data::one_line(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::ignore, FilterSpace::nospace, data::numbered(), output) == true);
		REQUIRE(output.str() == "1\n2\n3\n4\n5\n9\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::ignore, FilterSpace::nospace, data::mixed(), output) == true);
		REQUIRE(output.str() == "\n \nAa\naZ\nAz\nB\nc\nD\nE\nf\nFx\nG\n");
	}

	SECTION("ascending - unique - ignore case")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::ignore, FilterSpace::nospace, data::empty(), output) == true);
		REQUIRE(output.str() == "");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::ignore, FilterSpace::nospace, data::one_char(), output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::ignore, FilterSpace::nospace, data::one_line(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::ignore, FilterSpace::nospace, data::numbered(), output) == true);
		REQUIRE(output.str() == "1\n2\n3\n4\n5\n9\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::ignore, FilterSpace::nospace, data::mixed(), output) == true);
		REQUIRE(output.str() == "\n \nAa\naZ\nB\nc\nD\nE\nf\nFx\nG\n");
	}

	SECTION("descending")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, FilterSpace::nospace, data::empty(), output) == true);
		REQUIRE(output.str() == "");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, FilterSpace::nospace, data::one_char(), output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, FilterSpace::space, data::one_char(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, FilterSpace::nospace, data::one_line(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, FilterSpace::nospace, data::numbered(), output) == true);
		REQUIRE(output.str() == "9\n5\n4\n3\n2\n1\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, FilterSpace::nospace, data::mixed(), output) == true);
		REQUIRE(output.str() == "f\nc\naZ\nG\nFx\nE\nD\nB\nAz\nAa\n \n\n");
	}
	SECTION("descending - medzera")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, FilterSpace::space, data::empty(), output) == true);
		REQUIRE(output.str() == "");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, FilterSpace::space, data::one_char(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, FilterSpace::space, data::one_line(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, FilterSpace::space, data::numbered(), output) == true);
		REQUIRE(output.str() == "9\n5\n4\n3\n2\n1\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, FilterSpace::space, data::mixed(), output) == true);
		REQUIRE(output.str() == "f\nc\naZ\nG\nFx\nE\nD\nB\nAz\nAa\n");
	}

	SECTION("descending - ignore case")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::descending, Filter::all, Case::ignore, FilterSpace::nospace, data::empty(), output) == true);
		REQUIRE(output.str() == "");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::ignore, FilterSpace::nospace, data::one_char(), output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::ignore, FilterSpace::nospace, data::one_line(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::ignore, FilterSpace::nospace, data::numbered(), output) == true);
		REQUIRE(output.str() == "9\n5\n4\n3\n2\n1\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::ignore, FilterSpace::nospace, data::mixed(), output) == true);
		REQUIRE(output.str() == "G\nFx\nf\nE\nD\nc\nB\naZ\nAz\nAa\n \n\n");
	}

	SECTION("descending - unique")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::descending, Filter::unique, Case::sensitive, FilterSpace::nospace, data::empty(), output) == true);
		REQUIRE(output.str() == "");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::sensitive, FilterSpace::nospace, data::one_char(), output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::sensitive, FilterSpace::nospace, data::one_line(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::sensitive, FilterSpace::nospace, data::numbered(), output) == true);
		REQUIRE(output.str() == "9\n5\n4\n3\n2\n1\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::sensitive, FilterSpace::nospace, data::mixed(), output) == true);
		REQUIRE(output.str() == "f\nc\naZ\nG\nFx\nE\nD\nB\nAz\nAa\n \n\n");
	}

	SECTION("descending - unique - ignore case")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::descending, Filter::unique, Case::ignore, FilterSpace::nospace, data::empty(), output) == true);
		REQUIRE(output.str() == "");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::ignore, FilterSpace::nospace, data::one_char(), output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::ignore, FilterSpace::nospace, data::one_line(), output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::ignore, FilterSpace::nospace, data::numbered(), output) == true);
		REQUIRE(output.str() == "9\n5\n4\n3\n2\n1\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::ignore, FilterSpace::nospace, data::mixed(), output) == true);
		REQUIRE(output.str() == "G\nFx\nf\nE\nD\nc\nB\naZ\nAa\n \n\n");
	}
}