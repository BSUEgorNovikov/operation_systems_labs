#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "Employee.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    std::string file_name = argv[1];
    int notes_number = std::stoi(argv[2]);

    std::ofstream fout;
    fout.open(file_name, std::ios::binary);

    employee tmp;
    for (int i = 0; i < notes_number; i++)
    {
        std::cout << "Введите данные работника:\n(Порядок ввода: 1.ID 2.Имя 3.Кол-во отработанных часов)\n";
        std::cin >> tmp.num >> tmp.name >> tmp.hours;
        fout.write((char*)&tmp, sizeof(tmp));
    }

    fout.close();

    return 0;
}