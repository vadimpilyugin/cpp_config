#include "config.h"
#include <assert.h>
#include <string>

void test_existing () {
	Config::load("config/cfg1.txt");
	assert (Config::get()["SECTION1"]["param1"] == "value1");
	assert (Config::section("SECTION1")["param1"] == "value1");
	Printer::note ("OK!", "test_existing");
}
void test_non_existing () {
	Config::load("config/cfg1.txt");
	try {
		Config::get()["SECTION_1"];
	}
	catch (std::out_of_range &exc) {
		Printer::note ("OK!", "test_non_existing");
		return;
	}
	assert (0);
}

void test_modification () {
	Config::load ("config/cfg1.txt");
	int exists = 1;
	try {
		Config::section ("SECTION1").at ("modif_line");
	}
	catch (std::out_of_range &exc) {
		exists = 0;
	}
	if (exists) {
		Printer::note ("Remove 'modif_line = \"modified\"' from SECTION1. Press Enter when finished");
		std::string s;
		getline (std::cin, s);
		try {
			Config::section ("SECTION1").at ("modif_line");
		}
		catch (std::out_of_range &exc) {
			Printer::note ("OK!", "test_modification");
			return;
		}
		Printer::error ("Fail!", "test_modification");
		return;
	}
	else {
		Printer::note ("Add 'modif_line = \"modified\"' to SECTION1. Press Enter when finished");
		std::string s;
		getline (std::cin, s);
		try {
			Config::section ("SECTION1").at ("modif_line");
		}
		catch (std::out_of_range &exc) {
			Printer::error ("Fail!", "test_modification");
			return;
		}
		Printer::note ("OK!", "test_modification");
		return;
	}
}

int main () {
	Printer::debug ("Hello, world!");
	test_existing ();
	test_non_existing ();
	test_modification ();
	return 0;
}