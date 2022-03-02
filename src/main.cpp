#include <iostream>

#include "file_walker.h"

int main(/*int argc, char** argv*/)
{
    // if (argc < 2) {
    //     std::cerr << "Usage: ./traver <directory>" << std::endl;
    //     return -1;
    // }
    // std::string path = argv[1];
    FileWalker walker(FileWalker::Method::BFS);

    auto res = walker.walk(R"(E:\ENV)");
    auto res2 = walker.walk(R"(D:\Code)");

    std::cout << "res size:" << res.size() << std::endl;
    for (const std::string& file : res) {
        std::cout << file << std::endl;
    }

    std::cout << "res2 size:" << res2.size() << std::endl;

    return 0;
}
