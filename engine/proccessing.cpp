#include "proccessing.h"

Proccessing::Proccessing(Phonotext pt, std::string lng, double min_pwr, double max_pwr)
{

    std::cout << "proccess start\n";
    this->pt = pt;
    this->min_pwr = min_pwr;
    this->max_pwr = max_pwr;

    std::cout << "Add config\n";
    Conf CONF(lng);
    this->CONFIG = CONF;
    std::cout << "Config added\n";

    this->proccess();
}

// Полная обработка текста
void Proccessing::proccess()
{
    std::cout << "modify\n";
    modifyProccessor(); // Замена
    std::cout << "same\n";
    sameProccessor(); // Замена и запись
    std::cout << "join\n";
    joinProccessor(); // Объединение
    std::cout << "number\n";
    numberProccessor(); // Подсчёт
    std::cout << "finder\n";
    finderVolve(); // Поиск
    std::cout << "SP\n";
    SPmaxProccessor(); // Выборка
    std::cout << "combinations\n";
    combinationsProccessor(); // Комбинирование
    std::cout << "repeat\n";
    repeatProccessor(); // Подсчёт
    std::cout << "proccessing end\n";
    clock_t end = clock();
}


// Изменение символов по json
// Если изменять, то только напрямую с преподавателем
void Proccessing::modifyProccessor()
{
    std::string tmp_a;
    std::string tmp_b;
    std::string tmp_c;

    auto itPreviosLetter = pt.basetext.begin();

    bool needChange = false;
    for (auto it = pt.basetext.begin(); it != pt.basetext.end(); it++)
    {
        if (needChange)
        {
            // it->printable = "`";
            needChange = false;
        }
        if (it == pt.basetext.begin())
        {
            tmp_b = it->origin;
        }
        else
        {
            tmp_a = tmp_b;
            tmp_b = it->origin;

            auto modFirstKey = CONFIG.getModifications()->find(tmp_a);
            auto modFirstKeyEnd = CONFIG.getModifications()->end();
            if (modFirstKey != modFirstKeyEnd)
            {
                int i, l;
                auto modSecondKey = modFirstKey->second.find(tmp_b);
                auto modSecondKeyEnd = modFirstKey->second.end();
                if (modSecondKey != modSecondKeyEnd)
                {
                    tmp_c = modSecondKey->second;
                    for (i = 0; tmp_c[0] & (0x80 >> i); ++i); i = (i) ? i : 1; // find first letter
                    for (l = 0; tmp_c[i] & (0x80 >> l); ++l); l = (l) ? l : 1; // find second letter

                    itPreviosLetter->origin = tmp_c.substr(0, i);
                    it->origin = tmp_c.substr(i, l);
                    pt.basetext.emplace_after(it, Letter(tmp_c.substr(i + l)));
                    auto b = it;
                    (++b)->printable = it->printable;
                    it->printable = "`";
                    needChange = true;
                }

            }
        }
        itPreviosLetter = it;
    }
}


// Запись technic и printable, а так же замена символов в первом, если они являются одинаковыми по json
// P.S. стоит вопрос по переписи
void Proccessing::sameProccessor()
{
    for (auto& symb : pt.basetext)
    {
        std::map<std::string, std::string> asSame = CONFIG.getAsSame();
        auto sameKey = asSame.find(symb.printable);
        auto sameKeyEnd = asSame.end();
        if (sameKey != sameKeyEnd)
            symb.technic = sameKey->second;
        else if (symb.technic == "+")
            symb.technic = "&";

        if (symb.origin == " ")
            symb.technic = "|";

        // if (symb.technic == "&" || symb.technic == "|")
            // symb.printable = symb.technic;

        if (symb.technic == "&")
            symb.technic = symb.origin;
    }
}

// Объединение объектов, если они являются одним символом
void Proccessing::joinProccessor()
{
    std::string tmp_a; // Первый символ совмещающейся в комбинации
    std::string tmp_b; // Второй символ совмещающейся в комбинации

    // Поиск итератора на последний объект в Phonotext
    auto itLast = pt.basetext.begin();
    for (auto it = pt.basetext.begin(); it != pt.basetext.end(); it++)
        itLast = it;

    auto it = pt.basetext.begin(); // Данная переменная будет итератором на объект, с которым происходит работа в данный момент
    auto itPreviosLetter = pt.basetext.begin(); // Данная переменная будет указателем на объект, с которым происходила работа в прошлый раз

    while (it != pt.basetext.end()) // Проходка до конца
    {
        if (it == pt.basetext.begin()) // В первый раз указывается только первый символ ищущейся комбинации
            tmp_a = it->origin;
        else
        {
            tmp_a = itPreviosLetter->origin; // Запись первого символа (origin из предыдущего итератора)
            tmp_b = it->origin; // Запись второго символа (origin из настоящего символа)

            std::map<char, std::string> asOne = CONFIG.getAsOne();
            auto oneKey = asOne.find(tmp_a[0]); // Поиск комбинации по первому символу
            auto oneKeyEnd = asOne.end();
            if (oneKey != oneKeyEnd) // Если комбинация найдена
            {
                if ((tmp_a + tmp_b) == oneKey->second) // Если наша пара совпадает с комбинацией
                {
                    // Перезапись прошлого итератора
                    itPreviosLetter->origin = oneKey->second;
                    itPreviosLetter->printable = oneKey->second;
                    itPreviosLetter->technic = oneKey->second;
                    if (it == itLast) // Если это последний элемент, то удаление и выход
                    {
                        pt.basetext.erase_after(itPreviosLetter);
                        break;
                    }
                    // Если это не последний элемент, то перескок на следующий итератор, во избежание ошибки и удаление настоящего символа
                    it++;
                    pt.basetext.erase_after(itPreviosLetter);
                }
            }
            itPreviosLetter = it; // Смена прошлого итератора на настоящий
        }
        it++; // Следующий итератор
    }
}

// Функция для подсчёта различной информации по тексту
void Proccessing::numberProccessor()
{
    int i = 0; // Для подсчёта номера буквы в тексте
    int j = 1; // Для подсчёта слога в тексте
    int k = 1; // Для подсчёта слова в тексте
    int num = 1; // Не используется, но в оригинале было
    bool space = false; // Несёт информацию, является ли данный итерируемый объект пробелом
    int space_pos = 0; // Для подсчёта нахождения объекта в слове после пробела

    int l_number = 0; // Не используется, но в оригинале было
    for (auto it = pt.basetext.begin(); it != pt.basetext.end(); it++, l_number++)
    {
        it->number = i;
        it->word = k;

        if (it->technic == " " || it->technic == "|" || it->technic == "\n")
        {
            space_pos = 0;
            space = true;
        }
        else
        {
            std::vector<std::string> words = CONFIG.getWords();
            space_pos++;
            // Для чего всё делается дальше не совсем помню, но по-факту это ничего не делает
            bool flag = false;
            for (int t = 0; t < words.size(); t++)
                if (it->technic == words[t])
                    flag = true;
            space = (space && flag);
            // До этого момента
        }
        if (it->technic == "\n")
        {
            // обработка
            // P.S. хз почему я не рерайтнул обработку для этого, мб какая то проблема
        }
        // Определение гласных объектов
        std::vector<std::string> volves = CONFIG.getVolves();
        for (int t = 0; t < volves.size(); t++)
            if (it->printable == volves[t])
            {
                it->isVolve = true;
                j++;
                break;
            }
        // Определение согласных объектов
        std::vector<std::string> consonants = CONFIG.getConsonants();
        for (int t = 0; t < consonants.size(); t++)
            if (it->printable == consonants[t])
            {
                it->isConsonant = true;
                break;
            }
        it->syll = j;
        i++;
        num++;
        it->w_pos = space_pos; // Возникала ощибка с этим, а именно, значения не сохранялись
    }
}

// Нахождение итераторов гласных и добавление возврат массива со всеми этими итераторами
void Proccessing::finderVolve()
{
    for (auto it = pt.basetext.begin(); it != pt.basetext.end(); it++)
        if (it->isVolve)
            volveIterators.push_back(it);
}

// Создание массива элементов, которые представляют собой 
// итераторы на следующюю согласную после прошлой гласой и последнюю согласную перед следующей гласной
void Proccessing::SPmaxProccessor()
{
    std::vector<std::pair<std::forward_list<Letter>::iterator, std::forward_list<Letter>::iterator>> dividedVolveIterators;

    std::forward_list<Letter>::iterator startVolveIt, middleVolveIt, endVolveIt; // итераторы на прошлую, настоящую и следующую гласную
    bool firstVolve = false;

    if (volveIterators.size() > 1) // если гласных больше одной
    {
        // первоначальное расположение итератора в SP - второй символ, т.к. первый - пробел по умолчанию
        startVolveIt = pt.basetext.begin();
        startVolveIt++;
        // итератор на первый центр = итератор на первую гласную
        middleVolveIt = volveIterators[0];

        for (int i = 1; i < volveIterators.size(); i++) // проход по всем итераторам гласных, начиная со второго
        {
            endVolveIt = volveIterators[i]; // конечный итератор элемента массива, указывающий на последнюю согласную перед следующей гласной

            dividedVolveIterators.push_back(std::make_pair(startVolveIt, endVolveIt)); // Создание пары на последнюю согласную после последней гласной и последней согласной перед следующей гласной

            // следующее начало - следующая гласная после той, которая является центральной на данный момент
            // следовательно смещаем и переназначаем
            middleVolveIt++;
            startVolveIt = middleVolveIt;
            // следующая центральная - та, которая являлась следующей на данном цикле
            middleVolveIt = endVolveIt;
            // дальше идёт условие из оригинала на ударение

            int counter = 0;
            for (auto it = startVolveIt; it != endVolveIt; it++)
                if (it->technic == "\n")
                    counter++;
        }

        // добавление элементе массива, который включает согласные после последней гласной
        endVolveIt = pt.basetext.end();
        startVolveIt++;

        if (middleVolveIt->origin != "\n")
            dividedVolveIterators.push_back(std::make_pair(startVolveIt, endVolveIt));
    }
    else // если гласная одна - элемент в массиве только один: весь Phonotext
        dividedVolveIterators.push_back(std::make_pair(pt.basetext.begin(), pt.basetext.end()));

    pt.SP = dividedVolveIterators; // перенос всех элементов в Phonotext
}

// Нахождение индексов гласной и согласных
std::pair<int, std::vector<int>> Proccessing::findLocalWordsInds(std::pair<std::forward_list<Letter>::iterator, std::forward_list<Letter>::iterator> localSP)
{
    int indVolve;
    std::vector<int> indCons;

    int i = 0;
    for (auto it = localSP.first; it != localSP.second; it++, i++)
        if (it->isVolve)
            indVolve = i;
        else
            indCons.push_back(i);

    return std::make_pair(indVolve, indCons);
}

bool isCorrectComb(std::forward_list<Letter>::iterator it1, std::forward_list<Letter>::iterator it2, std::forward_list<Letter>::iterator it3)
{            
    if (it1->origin == "\n" || it2->origin == "\n" || it3->origin == "\n")
        return false;
    if (it1->origin == "-" || it2->origin == "-" || it3->origin == "-")
        return false;
    if (it1->origin == "—" || it2->origin == "—" || it3->origin == "—")
        return false;
    if (it1->origin == "–" || it2->origin == "–" || it3->origin == "–")
        return false;
    if (it1->origin == "," || it2->origin == "," || it3->origin == ",")
        return false;
    if (it1->origin == "." || it2->origin == "." || it3->origin == ".")
        return false;
    if (it1->origin == "’" || it2->origin == "’" || it3->origin == "’")
        return false;
    if (it1->origin == "€" || it2->origin == "€" || it3->origin == "€")
        return false;
    if (it1->origin == "!" || it2->origin == "!" || it3->origin == "!")
        return false;
    if (it1->origin == "?" || it2->origin == "?" || it3->origin == "?")
        return false;
    if (it1->origin == "j" || it2->origin == "j" || it3->origin == "j")
        return false;
    if (it1->origin == " " || it2->origin == " " || it3->origin == " ")
        return false;
    if (it1->origin == ":" || it2->origin == ":" || it3->origin == ":")
        return false;
    if (it1->origin == "a" || it2->origin == "a" || it3->origin == "a")
		return false;
	if (it1->origin == "b" || it2->origin == "b" || it3->origin == "b")
			return false;
	if (it1->origin == "c" || it2->origin == "c" || it3->origin == "c")
			return false;
	if (it1->origin == "d" || it2->origin == "d" || it3->origin == "d")
			return false;
	if (it1->origin == "e" || it2->origin == "e" || it3->origin == "e")
			return false;
	if (it1->origin == "f" || it2->origin == "f" || it3->origin == "f")
			return false;
	if (it1->origin == "g" || it2->origin == "g" || it3->origin == "g")
			return false;
	if (it1->origin == "h" || it2->origin == "h" || it3->origin == "h")
			return false;
	if (it1->origin == "i" || it2->origin == "i" || it3->origin == "i")
			return false;
	if (it1->origin == "j" || it2->origin == "j" || it3->origin == "j")
			return false;
	if (it1->origin == "k" || it2->origin == "k" || it3->origin == "k")
			return false;
	if (it1->origin == "l" || it2->origin == "l" || it3->origin == "l")
			return false;
	if (it1->origin == "m" || it2->origin == "m" || it3->origin == "m")
			return false;
	if (it1->origin == "n" || it2->origin == "n" || it3->origin == "n")
			return false;
	if (it1->origin == "o" || it2->origin == "o" || it3->origin == "o")
			return false;
	if (it1->origin == "p" || it2->origin == "p" || it3->origin == "p")
			return false;
	if (it1->origin == "q" || it2->origin == "q" || it3->origin == "q")
			return false;
	if (it1->origin == "r" || it2->origin == "r" || it3->origin == "r")
			return false;
	if (it1->origin == "s" || it2->origin == "s" || it3->origin == "s")
			return false;
	if (it1->origin == "t" || it2->origin == "t" || it3->origin == "t")
			return false;
	if (it1->origin == "u" || it2->origin == "u" || it3->origin == "u")
			return false;
	if (it1->origin == "v" || it2->origin == "v" || it3->origin == "v")
			return false;
	if (it1->origin == "w" || it2->origin == "w" || it3->origin == "w")
			return false;
	if (it1->origin == "x" || it2->origin == "x" || it3->origin == "x")
			return false;
	if (it1->origin == "y" || it2->origin == "y" || it3->origin == "y")
			return false;
	if (it1->origin == "z" || it2->origin == "z" || it3->origin == "z")
			return false;

    return true;
}

// Создание комбинаций
void Proccessing::combinationsProccessor(int N)
{
    N++;
    for (int i = 0; i < pt.SP.size(); i++)
    {
        // Из каждого участка выбирается гласная и все согласные в отдельный массив
        std::pair<int, std::vector<int>> localInds = findLocalWordsInds(pt.SP[i]);
        int posVolve = localInds.first;
        std::vector<int> posCons = localInds.second;

        // Всевозможные пары комбинаций из индексов массива согласных
        std::vector<std::pair<int, int>> consCombs;
        for (int j = 0; j < posCons.size(); j++)
            for (int k = j + 1; k < posCons.size(); k++)
                consCombs.push_back(std::make_pair(posCons[j], posCons[k]));

        // Сборка комбинаций из СГС в разных вариантах (в итераторах)
        std::vector<std::vector<std::forward_list<Letter>::iterator>> itCombs;
        for (int j = 0; j < consCombs.size(); j++)
        {
            // Создание сортированных комбинаций из пар индексов соглассных + индекс гласной
            std::vector<int> combs;
            combs.push_back(consCombs[j].first);
            combs.push_back(consCombs[j].second);
            combs.push_back(posVolve);
            sort(combs.begin(), combs.end());

            // Поиск итераторов элементов выбранных в пары
            std::forward_list<Letter>::iterator it1, it2, it3;
            it1 = it2 = it3 = pt.SP[i].first;

            for (int k = 0; k < combs[0]; k++)
                it1++;
            for (int k = 0; k < combs[1]; k++)
                it2++;
            for (int k = 0; k < combs[2]; k++)
                it3++;

            if (isCorrectComb(it1, it2, it3))
            {
                std::vector<std::forward_list<Letter>::iterator> tmpItCombs;
                tmpItCombs.push_back(it1);
                tmpItCombs.push_back(it2);
                tmpItCombs.push_back(it3);
                itCombs.push_back(tmpItCombs);
            }
        }
        if (itCombs.size())
            pt.syllableCombinations.push_back(itCombs);

        // Добавить то, что вытаскивается из итераторов и обработку из filter_combination
    }
}

// Функция, которая подсчитывает количество повторений комбинаций и их силы
void Proccessing::repeatProccessor()
{
    for (int n_syll = 0; n_syll < pt.syllableCombinations.size(); n_syll++)
    {
        for (auto& comb : pt.syllableCombinations[n_syll])
        {
            std::vector<std::string> a;

            // Создание структуры Repeat, в которой распологаются комбинации и инфа о них (комбинации состоят из СС: согласная + согласная)
            for (auto& i : comb)
                if (i->isConsonant)
                    a.push_back(i->technic);

            // Удаление повторяющихся символов
            std::set<std::string> tmpWords(a.begin(), a.end());
            std::string setToStr = "";
            for (auto& i : tmpWords)
                setToStr += i;

            // Отбрасывание невалидных комбинаций
            std::pair<bool, double> filter = rusFilterComb(comb, CONFIG.getWords());
            if (filter.first)
            {
                std::map<std::string, Repeat>::iterator it = pt.repeats.find(setToStr); // Ïðîâåðêà íà ïðèñóòñòâèå äàííîé êîìáèíàöèè â óæå äîáàâëåííûõ

                if (it == pt.repeats.end()) // Создание структуры Repeat, если структуры с такой комбинации ещё небыло
                {
                    Repeat tmpRepeat;
                    tmpRepeat._words = tmpWords; // Уникальные символы структуры
                    tmpRepeat.count = 1; // Структура только создаётся, следовательно это только первая добавленная
                    tmpRepeat.power = filter.second; // Добавление силы первой добавленной комбинации

                    for (auto& i : comb) // Добавление уникальных объектов класса Letter из временного массива в структуру
                    {
                        bool flag = false;
                        for (int j = 0; j < tmpRepeat.letters.size(); j++)
                            if (*i == tmpRepeat.letters[j])
                                flag = true;
                        if (!flag)
                            tmpRepeat.letters.push_back(*i);
                    }

                    tmpRepeat.combs.push_back(comb); // Добавление всех комбинации в структуру


                    if (tmpRepeat._words.size() != 1)
                        pt.repeats.insert(std::make_pair(setToStr, tmpRepeat)); // Перенос структуры в объект Phonotext
                }
                else
                {
                    it->second._words = tmpWords; // Уникальные символы структуры
                    it->second.count += 1; // Так как нашлась ещё одна комбинация, счётчик увеличивается
                    it->second.power = filter.second; // Добавление силы следующей добавленной комбинации

                    for (auto& i : comb) // Добавление уникальных объектов класса Letter из временного массива в структуру
                    {
                        bool flag = false;
                        for (int j = 0; j < it->second.letters.size(); j++)
                            if (*i == it->second.letters[j])
                                flag = true;
                        if (!flag)
                            it->second.letters.push_back(*i);
                    }

                    it->second.combs.push_back(comb); // Добавление всех комбинации в структуру
                }
            }
        }                
        //auto end = std::chrono::steady_clock::now();
        //auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        //std::cout << "The time-=-: " << elapsed_ms.count() << " ms\n";
    }
    pt.repeats.erase(pt.repeats.begin());
}

// Функция создана для фильтрации комбинаций, возвращает первым параметром <false, 0>, если комбинация является отбрасываемой,
// возвращает <true, pwr>, если комбинация валидна, где pwr - сила комбинации в тексте
// Доработать: сделать проверку на то, что текст, получаемый из комбинации являются двуконсонантным
std::pair<bool, double> Proccessing::rusFilterComb(std::vector<std::forward_list<Letter>::iterator> comb, std::vector<std::string> words)
{
    // Запись technic символов комбинации
    std::string tmptxt = "";
    for (auto it = comb[0]; it != comb[2]; it++)
        tmptxt += it->technic;

    // Отбрасывание повторяющихся символов
    std::string txt = "";
    for (int i = 0; i < tmptxt.size(); i++)
    {
        bool notUnique = false;
        for (int j = 0; j < txt.size(); j++)
            if (tmptxt[i] == txt[j])
                notUnique = true;

        if (!notUnique)
            txt += tmptxt[i];
    }

    // Комбинация не валидна, если получаемый текст меньше трёх символов
    if (txt.size() < 3)
        return std::make_pair(false, 0);

    // Подсчёт силы комбинации
    double pwr;
    if (comb[0]->isVolve) // Если гласная первая в комбинации
        pwr = 2;
    else if (comb[2]->isVolve) // Если гласная последняя в комбинации
        pwr = 1;
    else // Если гласная в середине комбинации
        pwr = 3;

    int count = 0;
    for (auto& i : txt) // Подсчёт количества пробелов
        if (i == '|')
            count++;
    pwr += (comb[2]->number - comb[0]->number - count == 2 ? 5 : 0); // Расположение гласных без пробелов
    pwr += (count == 0 ? 2 : 0); // количество пробелов
    count = 0;
    for (int i = txt.size() - 3; i < txt.size(); i++) // подсчёт количества букв "й" в последних трёх символах получаемого текста
        if ((txt[i] == "й"[0]) && (txt[i] == "й"[1]))
            count++;
    pwr += (count == 0 ? 4 : 0);
    pwr += (!comb[0]->w_pos || !comb[1]->w_pos || !comb[2]->w_pos ? 1 : 0); // Проверка на то, является ли символы из комбинации первым в слове
    pwr /= 15;

    return std::make_pair(min_pwr <= pwr <= max_pwr, pwr);
}

// Вывод
void Proccessing::print(std::string filename)
{
    std::ofstream jout("../data/json.json");

    std::ofstream fout;
    fout.open(filename);
    fout << "===========\n";

    fout << "-----------\n";
    fout << "origin      : ";
    for (auto& i : pt.basetext)
    {
        fout << i.origin;
    }
    fout << std::endl;

    fout << "-----------\n";
    fout << "technic     : ";
    for (auto& i : pt.basetext)
    {
        fout << i.technic;
    }
    fout << std::endl;

    fout << "-----------\n";
    fout << "printable   : ";
    for (auto& i : pt.basetext)
    {
        fout << i.printable;
    }
    fout << std::endl;

    fout << "-----------\n";
    fout << "isWord      : ";
    for (auto& i : pt.basetext)
    {
        if (i.isVolve)
            fout << "v";
        else if (i.isConsonant)
            fout << "c";
        else
            fout << "n";
    }
    fout << std::endl;

    fout << "-----------\n";
    fout << "w_pos      : ";
    for (auto& i : pt.basetext)
    {
        fout << i.w_pos;
    }
    fout << std::endl;

    fout << "-----------\n";
    fout << "SP          :\n";
    for (int i = 0; i < pt.SP.size(); i++)
    {
        fout << i << ": \"";
        for (auto it = pt.SP[i].first; it != pt.SP[i].second; it++)
        {
            fout << it->technic;
        }
        fout << "\"" << std::endl;
    }
    fout << std::endl;
    for (auto& i : pt.basetext)
    {
        fout << i.origin << i.technic << i.printable << i.isVolve << i.isConsonant << i.number << '|';
    }

    fout << "-----------\n";

    fout << "combinations:\n";
    for (int i = 0; i < pt.syllableCombinations.size(); i++)
    {
        fout << i << ":\n";
        for (int j = 0; j < pt.syllableCombinations[i].size(); j++)
        {
            for (int k = 0; k < 3; k++)
                fout << pt.syllableCombinations[i][j][k]->origin;
            fout << std::endl;
        }
        fout << std::endl;
    }
    fout << std::endl;

    std::string printable = "";
    for (auto it = pt.basetext.begin(); it != pt.basetext.end(); it++)
        printable += it->printable;

    nlohmann::json j = {"printable", printable};

    fout << "-----------\n";
    fout << "repeats:\n";
    for (auto& i : pt.repeats)
    {
        std::string letters = "";
        for (int j = 0; j < i.second.letters.size(); j++)
            letters += i.second.letters[j].origin;
        std::string swords = "";
        std::vector<std::string> vwords;
        for (auto& j : i.second._words)
        {
            swords += "<" + j + ">";
            vwords.push_back(j);
        }
        std::vector<std::string> combs;

        fout << "key : " << i.first << std::endl;
        fout << "Repeat.power : " << i.second.power << std::endl;
        fout << "Repeat.count : " << i.second.count << std::endl;
        fout << "Repeat.letters : " << letters << std::endl;
        fout << "Repeat._words : " << swords << std::endl;
        fout << "Repeat.combs : ";
        for (int j = 0; j < i.second.combs.size(); j++)
        {
            std::string tmpComb = "";
            for (int k = 0; k < i.second.combs[j].size(); k++)
            {
                fout << i.second.combs[j][k]->origin;
                tmpComb+= i.second.combs[j][k]->origin;
            }
            combs.push_back(tmpComb);
            fout << " ";
        }
        fout << std::endl << std::endl;

        j.push_back({i.first,
        {
            {"Repeat.power", i.second.power},
            {"Repeat.count", i.second.count},
            {"Repeat.letters", letters},
            {"Repeat.words", vwords},
            {"Repeat.combs", combs},
        }});
    }
    fout << std::endl;
    fout << "-----------\n";
    fout.close();
    jout << j;
    jout.close();
}

void Proccessing::createJson(std::string filename)
{
    std::string printable = "";
    for (auto it = pt.basetext.begin(); it != pt.basetext.end(); it++)
        printable += it->printable;


    std::string str = "{\n";
    for (auto& i : pt.repeats)
    {
        std::string key = "";
        std::string power = "";
        std::string count = "";
        std::string letters = ""; // удалить повторяющиеся
        std::string words = "";
        std::vector<std::string> combs;

        key = i.first;
        power = std::to_string(i.second.power);
        count = std::to_string(i.second.count);
        for (int j = 0; j < i.second.letters.size(); j++)
            letters += i.second.letters[j].origin;
        for (auto& j : i.second._words)
            words += j;
        for (int j = 0; j < i.second.combs.size(); j++)
        {
            std::string tCombs = "";
            for (int k = 0; k < i.second.combs[j].size(); k++)
                tCombs += i.second.combs[j][k]->origin;
            combs.push_back(tCombs);
        }

        nlohmann::json tmpOutJson = {
            {"printable", printable},
            {"repeat", {
                {"key", key},
                {"power", power},
                {"count", count},
                {"letters", letters},
                {"words", words}
            }}
        };

        str += "\t\"" + key + "\": {\n";
        str += "\t\t\"key\": \"" + key + "\",\n";
        str += "\t\t\"power\": \"" + power + "\",\n";
        str += "\t\t\"count\": \"" + count + "\",\n";
        str += "\t\t\"letters\": \"" + letters + "\",\n";
        str += "\t\t\"words\": \"" + words + "\",\n";
        str += "\t\t\"combs\": [";
        for (int j = 0; j < combs.size(); j++)
        {
            str += "\"" + combs[j] + "\"" + (j + 1 < combs.size() ? ", " : "");
        }
        str += "]\n";
        str += "\t},\n";
        outJson.push_back(tmpOutJson);
    }
    str += "}\n";

    std::ofstream fout;
    fout.open(filename);
    fout << str;
    fout.close();


}