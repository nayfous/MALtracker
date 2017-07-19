#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <boost/tokenizer.hpp>

using namespace std;

int main()
{
	// Ask user for input
	string entry;
	cout << "Welcome to MALtracker!\n" << "Please enter an entry in the following format: type (string), name (string), chapter/episode (int)" << endl;
	getline(cin, entry);
	// Check if user input is in the right format
	size_t character_count = count(entry.begin(), entry.end(), ',');
	if (character_count != 2) {
		do {
			cout << "Error, entry not in the right format\n"
				<< character_count << " comma's found but 2 are required\n"
				<< "Please enter the entry in the right format" << endl;
			getline(cin, entry);
			character_count = count(entry.begin(), entry.end(), ',');
		} while (character_count != 2);
	}
	//hard way to split string
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(",");
	tokenizer tokens(entry, sep);
	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
		cout << "<" << *tok_iter << "> " << endl;
	}
	//easy way to split string
	//vector<string> tokens;
	//boost::split(tokens, entry, boost::is_any_of(","));
	//for (std::vector<string>::iterator i = tokens.begin(); i != tokens.end(); ++i) {
	//	std::cout << *i;
	//}
	return 0;
}