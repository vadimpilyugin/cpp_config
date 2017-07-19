#include "config.h"

std::string Config::fn;

enum class States {
	SECTION_EXPECT,
	SECTION_NAME,
	PARAMS_EXPECT,
	PARAM_NAME,
	DELIM_EXPECT,
	PARAM_VALUE_EXPECT,
	PARAM_VALUE,
	SUCCESS,
};

Config::Config (std::ifstream &config_file) {
	char c;
	std::string section;
	// Hash params;
	std::string param_name;
	std::string param_value;
	States state = States::SECTION_EXPECT;
	while (state != States::SUCCESS) {
		// read next character
		c = config_file.get ();
		switch (state) {
			case States::SECTION_EXPECT:
				Printer::debug (to_string (c), "SECTION_EXPECT");
				if (isspace (c))
					;
				else if (c == OPENING_BRACE) {
					state = States::SECTION_NAME;
					section.clear ();
				}
				else if (c == std::char_traits<char>::eof()) {
					state = States::SUCCESS;
				}
				else {
					Printer::error ("Unexpected character", "SECTION_EXPECT", {{"Character", to_string (c)}});
					throw std::string ("Unexpected character: ") + to_string (c);
				}
				break;
			case States::SECTION_NAME:
				Printer::debug (to_string (c), "SECTION_NAME");
				if (isalpha (c) || isdigit (c) || c == UNDERSCORE) {
					section.push_back (c);
					
				}
				else if (c == CLOSING_BRACE) {
					state = States::PARAMS_EXPECT;
					// params = config[section];
					Printer::debug (section, "New section");
				}
				else {
					Printer::error ("Unexpected character", "SECTION_NAME", {{"Character", to_string (c)}});
					throw std::string ("Unexpected character: ") + to_string (c);
				}
				break;
			case States::PARAMS_EXPECT:
				Printer::debug (to_string (c), "PARAMS_EXPECT");
				if (isspace (c))
					;
				else if (c == OPENING_BRACE) {
					state = States::SECTION_NAME;
					section.clear ();
				}
				else if (isalpha (c) || isdigit (c) || c == UNDERSCORE) {
					state = States::PARAM_NAME;
					param_name.clear ();
					param_name.push_back (c);
				}
				else if (c == std::char_traits<char>::eof()) {
					state = States::SUCCESS;
				}
				else {
					Printer::error ("Unexpected character", "PARAMS_EXPECT", {{"Character", to_string (c)}});
					throw std::string ("Unexpected character: ") + to_string (c);
				}
				break;
			case States::PARAM_NAME:
				Printer::debug (to_string (c), "PARAM_NAME");
				if (isalpha (c) || isdigit (c) || c == UNDERSCORE) {
					param_name.push_back (c);
				}
				else if (isspace (c)) {
					state = States::DELIM_EXPECT;
				}
				else if (c == DELIM) {
					state = States::PARAM_VALUE_EXPECT;
				}
				else {
					Printer::error ("Unexpected character", "PARAM_NAME", {{"Character", to_string (c)}});
					throw std::string ("Unexpected character: ") + to_string (c);
				}
				break;
			case States::DELIM_EXPECT:
				Printer::debug (to_string (c), "DELIM_EXPECT");
				if (isspace (c))
					;
				else if (c == DELIM)
					state = States::PARAM_VALUE_EXPECT;
				else {
					Printer::error ("Unexpected character", "DELIM_EXPECT", {{"Character", to_string (c)}});
					throw std::string ("Unexpected character: ") + to_string (c);
				}
				break;
			case States::PARAM_VALUE_EXPECT:
				Printer::debug (to_string (c), "PARAM_VALUE_EXPECT");
				if (isspace (c))
					;
				else if (c == QUOTES) {
					state = States::PARAM_VALUE;
					param_value.clear ();
				}
				else {
					Printer::error ("Unexpected character", "PARAM_VALUE_EXPECT", {{"Character", to_string (c)}});
					throw std::string ("Unexpected character: ") + to_string (c);
				}
				break;
			case States::PARAM_VALUE:
				Printer::debug (to_string (c), "PARAM_VALUE");
				if (c == QUOTES) {
					state = States::PARAMS_EXPECT;
					config[section][param_name] = param_value;
					Printer::debug ("", section, config[section]);
				}
				else {
					param_value.push_back (c);
				}
				break;
		}
	}
}

Config &Config::load (const std::string &filename) {
	std::ifstream config_file (filename);
	if (!config_file.is_open ()) {
		Printer::error (filename, "Could not open configuration file");
		throw std::string ("Could not open configuration file");
	}
	Printer::debug (filename, "Found config file");
	if (fn.empty ())
		fn = filename;
	static Config cfg (config_file);
	return cfg;
}

Hash & Config::operator[] (const std::string &section) {
	return config.at(section);
}

void Config::out () const {
	for (const auto &section: config) {
		Printer::debug ("", std::string ("[") + section.first + std::string ("]"), section.second);
	}
}