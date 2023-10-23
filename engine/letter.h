#pragma once

#include <string>

class Letter
{
public:	
	Letter();
	Letter(std::string symbol);
	~Letter();

	// char to string
	// Какие именно данные будут храниться в переменных
	std::string origin;
	std::string technic;
	std::string printable;
	bool isConsonant;
	bool isVolve;
	int syll; // В каком слоге находится
	int pwr; 
	int number; // Номер по порядку в тексте
	int word; // Номер слова
	int pEnd; // Последняя буква в строке
	bool accent;

	int w_pos; //номер буквы в слове

	std::string getLetter();
	std::string getLetterRepr();

	bool operator ==(const Letter& letter) const;
};

