#include <iostream>
#include <string>
#include <algorithm>
#include <boost/tokenizer.hpp>

int main()
{
	// Ask user for input
	std::string entry;
	std::cout << "Welcome to MALtracker!\n" << "Please enter an entry in the following format: type (string), name (string), chapter/episode (int)" << std::endl;
	std::getline(std::cin, entry);
	// Check if user input is in the right format
	size_t character_count = std::count(entry.begin(), entry.end(), ',');
	if (character_count != 2) {
		do {
			std::cout << "Error, entry not in the right format\n"
				<< character_count << " comma's found but 2 are required\n"
				<< "Please enter the entry in the right format" << std::endl;
			std::getline(std::cin, entry);
			character_count = std::count(entry.begin(), entry.end(), ',');
		} while (character_count != 2);
	}
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(",");

	return 0;
}