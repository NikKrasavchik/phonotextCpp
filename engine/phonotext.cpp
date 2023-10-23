#include "phonotext.h"

Phonotext::Phonotext()
{
}

Phonotext::Phonotext(std::string text)
{
	if (text == "")
		return;

	// Генерация односвязного списка букв
	basetext.emplace_front(Letter(" "));

	std::forward_list<Letter>::iterator iter = basetext.begin();
	for (int i = 0, l = 0; i < text.size(); i+=l)
	{
		for(l = 0; text[i] & (0x80 >> l); ++l); l = (l)?l:1; // find next letter
		basetext.emplace_after(iter, Letter(text.substr(i, l)));
		++iter;
	}
}

Phonotext::~Phonotext()
{
}

std::pair<int, int> Phonotext::countLetters()
{
	std::pair<int, int> count(0, 0); // <countSymbols, countVolves>

	for (auto &symb : basetext)
	{
		if (symb.isConsonant)
			count.first++;
		if (symb.isVolve && symb.printable != "\n" && symb.printable != "--endl") // Вернуться к вопросу хранения данных ("--endl")
		{
			count.second++;
			count.first++;
		}
	}

	return count;
}

std::string Phonotext::getOrigin()
{
	std::string originText = "";

	for (auto& symb : basetext)
		originText += symb.origin;

	return originText;
}

std::string Phonotext::getTechnic()
{
	std::string technicText = "";

	for (auto& symb : basetext)
		technicText += symb.technic;

	return technicText;
}

std::string Phonotext::getPrintable()
{
	std::string printableText = "";

	for (auto& symb : basetext)
		printableText += symb.printable;

	return printableText;
}

std::string Phonotext::getPhonotextRepr()
{
	std::string reprText = "";

	for (auto& symb : basetext)
		reprText += symb.getLetterRepr();

	return reprText;
}

int Phonotext::length()
{
	int len = 0;

	for (auto& symb : basetext)
		len++;

	return len;
}