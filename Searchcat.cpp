// Searchcat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cctype>

#include <algorithm>

using namespace std;


void show_match_options() {
	cout << "\nSelect options" << endl;
	cout << "1) Word (exact matches only)" << endl;
	cout << "2) Pattern (exact matches + part of a larger word)" << endl;
	cout << "Your choice: ";
}

string tolowercase(string word) {
	// this is to change everything to lowercase
	transform(word.begin(), word.end(), word.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return word;
}


// this function will remove the . , ! ? and other special characters.
string remove_special_char(string sentence) {
	// this removes , ! . and any other special characters
	sentence.erase(std::remove_if(sentence.begin(), sentence.end(),
		[](auto const& c) -> bool { return !std::isalnum(c); }), sentence.end());
	return sentence;
}

int count_word_from_file(istream& file, string word) {
	string word_from_file;
	int countwords = 0;
	while (file >> word_from_file) // for each candidate word read from the file 
	{
		word_from_file = remove_special_char(word_from_file);
		word_from_file = tolowercase(word_from_file);
		if (word == word_from_file) ++countwords;

	}
	return countwords;
}


int count_pattern_from_file(istream& file, string word) {
	string line;
	int countwords = 0;
	while (getline(file, line)) {

		line = tolowercase(line);
		int found = line.find(word);
		while (found != string::npos) {
			++countwords;
			found = line.find(word, found + 1);

		}
	}
	return countwords;
}

int count_pattern_from_console(string word) {

	cout << "\nEnter the text below. type ESC to exit press Enter key twice" << endl;
	string line;
	int countwords = 0, found;
	do {
		getline(cin, line);
		line = tolowercase(line);
		found = line.find(word);
		while (found != string::npos) {
			++countwords;
			found = line.find(word, found + 1);
		}
	} while (line != "esc");

	return countwords;
}

int count_word_from_console(string word) {
	cout << "Enter the text below. type ESC to stop" << endl;
	string line, word_input;
	int countwords = 0;

	do // for each candidate word read from the file 
	{
		word_input = remove_special_char(word_input);
		word_input = tolowercase(word_input);

		if (word == word_input) ++countwords;
		cin >> word_input;


	} while (word_input != "esc");
	return countwords;
}


int main()
{
	int file_choice, countwords, exitcode = 1, match_choice, dummy;
	string word, line, input, path = "", oldpath;

	cout << "Welcome to the Cat Finder! \n" << endl;
	while (exitcode) {
		cout << "1) From File" << endl;
		cout << "2) From Console" << endl;
		cout << "Choose 1 or 2: ";
		cin >> file_choice;
		if (file_choice == 1) {
			cout << "Write the path of the file\n";
			oldpath = path;
			if (path == "") {
				cin >> path;
			}
			else {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "\nThe default will be the previous path you opened" << endl;

				getline(cin, path);
				if (path.empty()) {
					path = oldpath;
				}

			}

			ifstream file(path.c_str());

			if (!file.is_open())
			{
				cerr << "\nError! File not found!\nTry again\n";
				continue;
			}

			cout << "\nFile '" << path << "' opened.\n";

			cout << "\nWrite the word you're searching for\n";
			cin >> word;
			word = tolowercase(word);
			show_match_options();
			cin >> match_choice;
			if (match_choice == 1) {
				countwords = count_word_from_file(file, word);
			}
			else {
				countwords = count_pattern_from_file(file, word);
			}

		}



		else if (file_choice == 2) {
			cout << "\nWrite the word you're searching for\n";
			cin >> word;
			word = tolowercase(word);
			show_match_options();
			cin >> match_choice;

			cout << "\nNow, write the text to search from" << endl;
			if (match_choice == 1) {
				countwords = count_word_from_console(word);
			}
			else {
				countwords = count_pattern_from_console(word);
			}


		}
		else {
			cout << "\nPlease Enter a correct choice.\n" << endl;
			continue;
		}

		// this clears the input buffer

		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nThe word '" << word << "' has been found " << countwords << " times.\n";
		cout << endl;
		cout << "\nPress 0 to exit \nPress any number to search again: \n";
		cin >> exitcode;
	}

	return 0;

}