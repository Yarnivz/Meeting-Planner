//
// Created by tishaak on 3/29/26.
//
#include "ContractsDocumentationGenerator.h"
#include <iostream>
#include <filesystem> //reminder to put std before since it seems to be part of iostream

int main()
{
    std::cout << "test" << std::endl;
    for (const std::filesystem::directory_entry& entry: std::filesystem::directory_iterator("./src"))
    {
        if (entry.path().extension() == ".cpp")
        {

        }
    }
}