#include "conf.h"

Conf::Conf()
{
}

Conf::Conf(std::string lng)
{
    if (lng == "rus")
        makeConfig("../lng json/russian.json");
    else if (lng == "eng")
        makeConfig("../lng json/english.json");
    else if (lng == "lat")
        makeConfig("../lng json/latin.json");
}

Conf::~Conf()
{
}

void Conf::makeConfig(std::string lngPath)
{
    std::ifstream fin(lngPath); // Подгрзка файла json
    nlohmann::json data = nlohmann::json::parse(fin); // Выделение json файла

    makeAlphabetConfig(data["alphabet"]); // Преобразования алфавита из строкового типа в массив букв

    consonants = data["consonants"]; // Выборка символов из consonants

    volves = data["volves"]; // Выборка символов из volves

    // Соединение consonants и volves в один массив
    for (auto& i : consonants)
        words.push_back(i);
    for (auto& i : volves)
        words.push_back(i);

    // Создание словаря: Буква с которой начинается комбинация букв - Сочетание символов (букв)
    std::vector<std::string> jAsOne;
    for (auto& i : data["as_one"])
        jAsOne.push_back(i);
    makeAsOneConfig(jAsOne);

    makeAsSameConfig(data["as_same"], data["alphabet"]); // Создание словаря: Уникальный символ - Как символ слышится ( {'a':'a', 'p':'b', 'ph':'f'} )

    // Создания словаря: (Первая буква - (вторая буква - значение))
    std::map<std::string, std::string> jDictionary;
    for (nlohmann::json::iterator it = data["modifications"].begin(); it != data["modifications"].end(); it++)
        jDictionary.emplace(it.key(), it.value());
    makeModificationsConfig(jDictionary);
}

void Conf::makeAlphabetConfig(std::string jAlphabet)
{
    for (auto& i : jAlphabet)
        alphabet.push_back(i);
}

void Conf::makeAsOneConfig(std::vector<std::string> jAsOne)
{
    for (auto& i : jAsOne)
    {
        auto it = asOne.find((char)i[0]);
        if (it == asOne.end())
            asOne.emplace((char)i[0], i);
        else
            it->second = i;
    }
        
}

void Conf::makeAsSameConfig(std::vector<std::vector<std::string>> jAsSame, std::string jAlphabet)
{
    for (int i = 0; i < jAlphabet.size(); i++) // Добавление всех букв алфавита в словарь: Буква - Буква
    {
        std::string tempStr(1, jAlphabet[i]);
        asSame.emplace(tempStr, tempStr);
    }
    for (int i = 0; i < jAsSame.size(); i++) // Добавление недостающих символов и сочетаний букв, замена значений словаря на "как слышится"
    {
        for (auto& j : jAsSame[i])
        {
            // Добавление уникальных сиволов и сочетаний букв
            if (asSame.find(j) == asSame.end())
                asSame.emplace(j, j);

            // Поиск и замена значений словаря на "как слышится"
            std::map<std::string, std::string> ::iterator it = asSame.find(j);
            it->second = jAsSame[i][0];
        }
    }
}

void Conf::makeModificationsConfig(std::map<std::string, std::string> jDictionary)
{
    int l;
    for (auto& i : jDictionary)
    {
        for(l = 0; i.first[0] & (0x80 >> l); ++l); l = (l)?l:1; 
        // i.first.substr(0,l) -> first letter; i.first.substr(l) -> second letter

        if (modifications.find(i.first.substr(0, l)) == modifications.end()) // Добавление в словарь, если первая буква отсутствует
        {
            std::map<std::string, std::string> tmp;
            tmp.emplace(i.first.substr(l), i.second);
            modifications.emplace(i.first.substr(0, l), tmp);
        }
        else // Добавление в подсловарь, если первая буква присутствует
        {
            std::map<std::string, std::map<std::string, std::string>> ::iterator it = modifications.find(i.first.substr(0, l));
            it->second.emplace(i.first.substr(l), i.second);
        }
    }
}

Conf Conf::operator=(Conf& CONFIG)
{
    this->modifications = CONFIG.modifications;
    this->asOne = CONFIG.asOne;
    this->consonants = CONFIG.consonants;
    this->volves = CONFIG.volves;
    this->words = CONFIG.words;
    this->asSame = CONFIG.asSame;
    this->alphabet = CONFIG.alphabet;

    return *this;
}