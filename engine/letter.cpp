#include "letter.h"

Letter::Letter()
{
	origin = "";
	technic = "";
	printable = "";
	isConsonant = NULL;
	isVolve = NULL;
	syll = NULL;
	pwr = NULL;
	number = NULL;
	word = NULL;
	pEnd = NULL;
	accent = NULL;
}

Letter::Letter(std::string symbol)
{
	origin = symbol;
	technic = "+";
	printable = symbol;
	isConsonant = false;
	isVolve = false;
	syll = 0;
	pwr = 0;
	number = 0;
	word = 0;
	pEnd = 0;
	accent = false;
}

Letter::~Letter()
{
}

std::string Letter::getLetter()
{
	std::string outLetter = "";
	outLetter += origin;
	outLetter += "(";
	outLetter += technic;
	outLetter += printable;
	outLetter += ")";
	return outLetter;
}

std::string Letter::getLetterRepr()
{
	std::string outLetter = "";
	outLetter += origin;
	outLetter += "'";
	outLetter += technic;
	outLetter += "'";
	outLetter += printable;
	outLetter += "'";
	outLetter += (isConsonant ? "C" : "-");
	outLetter += (isVolve ? "V" : "-");
	outLetter += std::to_string(number);
	outLetter += "'";
	outLetter += std::to_string(syll);
	outLetter += "'";
	outLetter += std::to_string(word);
	return outLetter;
}

bool Letter::operator==(const Letter& letter) const
{
	if (this->origin != letter.origin)
		return false;
	if (this->technic != letter.technic)
		return false;
	if (this->printable != letter.printable)
		return false;
	if (this->isConsonant != letter.isConsonant)
		return false;
	if (this->isVolve != letter.isVolve)
		return false;
	if (this->syll != letter.syll)
		return false;
	if (this->pwr != letter.pwr)
		return false;
	if (this->number != letter.number)
		return false;
	if (this->word != letter.word)
		return false;
	if (this->pEnd != letter.pEnd)
		return false;
	if (this->accent != letter.accent)
		return false;
	return true;
}