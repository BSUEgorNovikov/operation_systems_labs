#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include "Employee.h"
#include <vector>
#include <string>
int main() {
	setlocale(LC_ALL, "Ru");

	std::string binary_file_name;
	std::string number_of_notes;
	std::ifstream file_input;
	std::vector<employee> emps;
	employee empl;

	std::cout << "Введите имя бинарного файла:\n";
	std::cin >> binary_file_name;
	std::cout << "Введите кол-во рабочих:\n";
	std::cin >> number_of_notes;

	std::string creator_cmd = "Creator.exe " + binary_file_name + " " + number_of_notes;
	std::wstring converting_creator_to_lpwstr = std::wstring(creator_cmd.begin(), creator_cmd.end());
	LPWSTR lpszCreatorProcessCommandLine = &converting_creator_to_lpwstr[0];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(NULL, lpszCreatorProcessCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		std::cout << "Creator.exe не был запущен.\n";
		return GetLastError();
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	file_input.open(binary_file_name, std::ios::binary);
	if (file_input.is_open()) {
		while (!file_input.eof()) {
			file_input.read((char*)&empl, sizeof(employee));
			emps.push_back(empl);
		}
		emps.pop_back();
	}
	std::cout << "Информация из бинарного файла:\nID\tИмя\tЧасы\n";
	for (size_t i = 0; i < emps.size(); i++)
	{
		std::cout << emps[i].num << "\t" << emps[i].name << "\t" << emps[i].hours << "\n";
	}

	std::string txt_file_name;
	std::string pay_for_an_hour;

	std::cout << "Введите имя для выходного файла:\n";
	std::cin >> txt_file_name;
	std::cout << "Введите оплату за час:\n";
	std::cin >> pay_for_an_hour;

	std::string reporter_cmd = "Reporter.exe " + binary_file_name + " " + txt_file_name + " " + pay_for_an_hour;
	std::wstring converting_reporter_to_lpwstr = std::wstring(reporter_cmd.begin(), reporter_cmd.end());
	LPWSTR lpszReporterProcessCommandLine = &converting_reporter_to_lpwstr[0];

	if (!CreateProcess(NULL, lpszReporterProcessCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		std::cout << "Reporter.exe не был запущен.\n";
		return GetLastError();
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	std::ifstream file_output;

	file_output.open(txt_file_name);
	if (file_output.is_open()) {
		while (!file_output.eof()) {
			std::string temporary;
			getline(file_output, temporary);
			std::cout << temporary << "\n";
		}
	}
	file_output.close();
	_getch();
	TerminateProcess(pi.hProcess, 1);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	_getch();
	return 0;
}
