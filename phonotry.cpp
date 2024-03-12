#include <fstream>
#include <chrono>

#include "engine/phonotext.h"
#include "engine/proccessing.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
        return 0;

	auto begin = std::chrono::steady_clock::now();

    std::string localPath = "../";
#ifdef _WIN32
    system("chcp 65001");
    localPath = "../../";
#endif

    std::ifstream fin;
    fin.open(localPath + "/data/" + argv[1], std::ios_base::in);
    if (!fin.is_open()){return 0;};
    std::cout << "start\n";

    std::string data;

    while (!fin.eof())
    {
        std::string line;
        std::getline(fin, line);
        data += line + '\n';
    }
    
    fin.close();
    std::cout << "Read End\n";
    Phonotext pt(data);
    Proccessing proc(pt, "rus", 0., 100.);

    proc.createJson(localPath + "/data/outJson.json");
    proc.print(localPath + "/data/out.txt");

    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time for proccessing: " << elapsed_ms.count() << " ms\n";

#ifdef _WIN32
    system("python ../../rewriteXlsx.py");
#endif

    std::cout << "Full time: " << elapsed_ms.count() << " ms\n";
    return 0;
}