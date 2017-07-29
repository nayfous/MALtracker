// import library's
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
#include "sqlite3.h"

// set namespaces
namespace fs = std::experimental::filesystem;
using namespace std;

// function declarations
vector<string> UserEntry();
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
sqlite3* CreateDB(char *zErrMsg, int rc, const char *sql);
void InsertUserInput(sqlite3 *db, vector<string> user_input, char *zErrMsg, int rc, const char *sql);
static int getCheckResult(void *data, int argc, char **argv, char **azColName);
void UpdateUserInput(sqlite3 *db, vector<string> user_input, char *zErrMsg, int rc, const char *sql);

// main function
int main()
{
	// declaring variables
	sqlite3 *db;
	const char *sql = "";
	const char *data = "Callback function called";
	char *zErrMsg = 0;
	int rc = 0;
	
	// receive user input
	vector<string> user_input = UserEntry();

	// create or open database
	db = CreateDB(zErrMsg, rc, sql);
	// check if user input is present in the table
	string temp = "SELECT EXISTS(SELECT 1 FROM MALTRACKER WHERE NAME = '" + user_input[1] + "' LIMIT 1);";
	sql = temp.c_str();
	// execute sql command
	rc = sqlite3_exec(db, sql, getCheckResult, (void*)data, &zErrMsg);
	// if the user input is present update de table
	if (rc != SQLITE_OK) {
		UpdateUserInput(db, user_input, zErrMsg, rc, sql);
	}
	// if the input is not present, insert the data in the table
	else {
		InsertUserInput(db, user_input, zErrMsg, rc, sql);
	}
	return 0;
}

vector<string> UserEntry()
{
	// declaring variables
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
			boost::to_upper(entryvec[0]);
			// check if first user input is 1 of the 3 choices
			if (!((entryvec[0] == "MANGA") || (entryvec[0] == "ANIME") || (entryvec[0] == "LIGHTNOVEL")))
			{
				cout << "Error: an invalid type has been entered/nPlease enter one of the following types: manga, anime or lightnovel" << endl;
				state = 1;
			}
			// trim whitespace from input
			boost::trim(entryvec[0]);
			boost::trim(entryvec[1]);
			boost::trim(entryvec[2]);
		}
	}
	return entryvec;
}

static int callback(void *data, int argc, char **argv, char **azColName) 
{
	// print starting message
	fprintf(stderr, "%s: ", (const char*)data);

	// loop through given return arguments and print them
	for (int i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static int getCheckResult(void *data, int argc, char **argv, char **azColName)
{
	// return state of execution
	char* temp = argv[0];
	int cal = temp[0] - '0';
	return cal;
}

sqlite3* CreateDB(char *zErrMsg, int rc, const char *sql)
{
	// initialize database
	sqlite3 *database;

	// open or create database
	rc = sqlite3_open("save_database.db", &database);

	// control the return call of the open function and print appropiate error messagess
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
		return 0;
	}
	else {
		fprintf(stderr, "Opened database succesfully\n");
	}

	// create sql statement to create table
	sql = "CREATE TABLE IF NOT EXISTS MALTRACKER(" \
		  "NAME           TEXT  NOT NULL," \
		  "LIGHTNOVEL    TEXT," \
		  "MANGA          TEXT," \
		  "ANIME          TEXT );";
	
	// execute sql statement
	rc = sqlite3_exec(database, sql, callback, 0, &zErrMsg);

	// control the return call of the execute function and print appropiate error messagess
	if (rc != SQLITE_OK) 
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stdout, "Table innitialized successfully\n");
	}

	return database;
}

void InsertUserInput(sqlite3 *db, vector<string> user_input, char *zErrMsg, int rc, const char *sql)
{
	// create sql statement to insert data in table
	string dpz = "INSERT INTO MALTRACKER (NAME," + user_input[0] + ")" \
		"VALUES ('" + user_input[1] + "'," + user_input[2] + ");";
	sql = dpz.c_str();
	
	// execute sql statement
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	// control the return call of the execute function and print appropiate error messagess
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Records created successfully\n");
	}
	
	// close database
	sqlite3_close(db);
}

void UpdateUserInput(sqlite3 *db, vector<string> user_input, char *zErrMsg, int rc, const char *sql)
{
	
	// create sql statement to update entry in table
	string dpz = "UPDATE MALTRACKER set " + user_input[0] + " = " + user_input[2] + " where NAME='" + user_input[1] + "';";
	sql = dpz.c_str();
	
	// execute sql statement
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	// control the return call of the execute function and print appropiate error messagess
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Records updated successfully\n");
	}

	// close database
	sqlite3_close(db);
}
