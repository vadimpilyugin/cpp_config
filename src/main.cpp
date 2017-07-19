#include "config.h"
#include <assert.h>

void test_existing () {
	Config::load("config/cfg1.txt");
	assert (Config::get()["SECTION1"]["param1"] == "value1");
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

int main () {
	Printer::debug ("Hello, world!");
	test_existing ();
	test_non_existing ();
	return 0;
}