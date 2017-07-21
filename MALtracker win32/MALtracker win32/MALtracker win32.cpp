#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <experimental\filesystem>
#include <Windows.h>
#include <boost\algorithm\string.hpp>
#include <climits>
#include <cctype>
#include <locale>

namespace fs = std::experimental::filesystem;
using namespace std;

vector<string> UserEntry();
string ExePath();
int CheckSaveFile(string path);
string GetUserSaveFilePath();
void CreateSaveFile(string path);
void SaveUserInput(vector<string> user_input, string file_path);
vector<string> CreateSaveFileVector(string file_path);
int CalculateScore(string entry);
int CompareScores(int user_score, int test_score);
static inline void ltrim(std::string &s);
static inline void rtrim(std::string &s);
static inline void trim(std::string &s);

int main()
{
	string path_file = ExePath();
	int file_state = CheckSaveFile(path_file);
	string file_path = "";
	if (file_state == 0)
	{
		file_path = GetUserSaveFilePath();
		boost::to_lower(file_path);
		if (file_path == "no" || file_path == "n")
		{
			//create save file
			CreateSaveFile(path_file);
		}
	}
	vector<string> user_input = UserEntry();
	SaveUserInput(user_input, path_file+"\\save_file.txt");
	
	return 0;
}

vector<string> UserEntry()
{
	size_t character_count;
	vector<string> entryvec;
	string entry;
	int state = 1;
	int episode;

	// Welcome message 
	cout << "Welcome to MALtracker!\n" << "Please enter an entry in the following format: type (string), name (string), chapter/episode (int)" << endl;

	// Ask and check for user input
	while (state) {
		// Get user input
		getline(cin, entry);
		// count frequentie of comma's in input
		character_count = count(entry.begin(), entry.end(), ',');
		// check if input is in the right format
		if (character_count != 2)
		{
			cout << "Error, entry not in the right format\n"
				<< character_count << " comma's found but 2 are required\n"
				<< "Please enter the entry in the right format" << endl;
			state = 1;
		}
		else
		{
			// split input in a vector
			boost::split(entryvec, entry, boost::is_any_of(","));
			// try converting the episode/chapter entry in an int
			try
			{
				episode = stoi(entryvec[2]);
				state = 0;
			}
			// if the episode/chapter entry is a non-number re-ask user for input
			catch (std::invalid_argument)
			{
				cout << "Error: A non number entered for episodes/chapters" << endl;
				state = 1;
			}
			// if the episode/chapter entry is too big for a int re-ask user
			catch (std::out_of_range)
			{
				cout << "Error: A too big number to handel or invalid int number has been entered for episodes/chapters" << endl;
				state = 1;
			}
			// lower first input
			boost::to_lower(entryvec[0]);
			// check if first user input is 1 of the 3 choices
			if (!((entryvec[0] == "manga") || (entryvec[0] == "anime") || (entryvec[0] == "lightnovel")))
			{
				cout << "Error: an invalid type has been entered/nPlease enter one of the following types: manga, anime or lightnovel" << endl;
				state = 1;
			}
		}
	}
	return entryvec;
}

string ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}

int CheckSaveFile(string path)
{
	string save_path = path + "\\save_file.txt";
	int state = 0;
	for (auto & p : fs::directory_iterator(path)) 
	{
		if (p.path().string() == save_path)
		{
			state = 1;
			break;
		}

	}
	return state;
}

string GetUserSaveFilePath()
{
	string input_user = "no";
	cout << "No save file detected.\n"
		<< "Please enter a save file path if located elsewhere.\n"
		<< "Enter no to create a new save file." << endl;
	getline(cin, input_user);
	return input_user;
}

void CreateSaveFile(string path)
{
	ofstream save_file(path + "//save_file.txt");
	save_file << "Anime entries: 0" << endl;
	save_file << "Manga entries: 0" << endl;
	save_file << "Lightnovel entries: 0" << endl;
	save_file << "@#!\nAnime:\n@#1\nentries" << endl;
	save_file << "@#!\nManga:\n@#2\nentries" << endl;
	save_file << "@#!\nLightnovels:\n@#3\nentries" << endl;
	save_file.close();
}

void SaveUserInput(vector<string> user_input, string file_path)
{
	int loop_state = 0;
	int score_user = CalculateScore(user_input[1]);
	vector<string> data_vector = CreateSaveFileVector(file_path);
	vector<int>::iterator it;
	trim(user_input[1]);
	for (int i = 3; i < data_vector.size(); ++i)
	{
		boost::to_lower(data_vector[i]);
		if (data_vector[i] == "@#!")
		{
			loop_state = 0;
		}
		else if (data_vector[i] == (user_input[0] + ":"))
		{
			loop_state = 1;
			i += 2;
		}
		
		if (loop_state)
		{
			if (CompareScores(score_user, CalculateScore(data_vector[i])))
			{
				auto it = data_vector.begin() + i;
				it = data_vector.insert(it, user_input[1]);
				cout << data_vector[i] << endl;
				break;
			}
		}
	}
}

vector<string> CreateSaveFileVector(string file_path)
{
	vector<string> save_file_lines;
	ifstream save_file(file_path.c_str());
	string line;
	while (getline(save_file, line))
	{
		if (!line.empty())
			save_file_lines.push_back(line);
	}
	return save_file_lines;
}

int CalculateScore(string entry)
{
	int score = 0;
	if (entry != "@#!")
	{
		for (int i = 0; i < entry.size(); ++i)
		{
			if (entry[i] != ' ')
				score += entry[i] - 'A';
		}
	}
	else
		score = INT_MAX;
	return score;
}

int CompareScores(int user_score, int test_score)
{
	if (user_score < test_score)
		return 1;
	else
		return 0;
}

static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

// voeg sql database integratie