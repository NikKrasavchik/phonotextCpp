#pragma once

#include <forward_list>
#include <string>
#include <vector>
#include <set>
#include <map>

#include <iostream>

#include "letter.h"

struct Repeat
{
	int count;
	double power;
	std::vector<Letter> letters;
	std::vector<std::vector<std::forward_list<Letter>::iterator>> combs;
	std::set<std::string> _words;
};

class Phonotext
{
public:
	Phonotext();
	Phonotext(std::string text);
	~Phonotext();

	std::forward_list<Letter> basetext;
	std::vector<std::pair<std::forward_list<Letter>::iterator, std::forward_list<Letter>::iterator>> SP;
	std::vector<std::vector<std::vector<std::forward_list<Letter>::iterator>>> syllableCombinations;
	std::map<std::string, Repeat> repeats;


	std::pair<int, int> countLetters();
	std::string getOrigin();
	std::string getTechnic();
	std::string getPrintable();
	std::string getPhonotextRepr();
	int length();
};

