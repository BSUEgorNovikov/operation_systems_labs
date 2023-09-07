#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "Employee.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    std::string bin_file_name = argv[1];
    std::string report_file_name = argv[2];
    int paid_for_hour = std::stoi(argv[3]);

    std::ifstream fin;
    std::ofstream fout;

    std::vector<employee> vector_of_workers;

    fin.open(bin_file_name, std::ios::binary);
    employee tmp;
    while (!fin.eof())
    {
        fin.read((char*)&tmp, sizeof(tmp));
        vector_of_workers.push_back(tmp);
    }

    fout.open(report_file_name);
    int amount_of_workers = vector_of_workers.size();
    fout << "Îò÷¸ò ïî ôàéëó \"" << bin_file_name << "\"\n";
    fout << "\tID\tÈìÿ\t×àñû\tÇÏ";
    for (int i = 0; i < amount_of_workers; i++)
    {
        fout << "\t" << vector_of_workers[i].num << "\t" << vector_of_workers[i].name << "\t" << vector_of_workers[i].hours << "\t" << vector_of_workers[i].hours * paid_for_hour << "\n";
    }

    fin.close();
    fout.close();

    return 0;
}