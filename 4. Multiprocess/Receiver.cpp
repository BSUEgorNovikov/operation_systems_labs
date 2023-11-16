#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <conio.h>

int main()
{
	setlocale(LC_ALL, "ru");

	std::fstream bin_file;

	std::cout << "Введите имя бинарного файла: ";
	std::string file_name;
	getline(std::cin, file_name);

	std::cout << "Введите количество записей в файле: ";
	int amount_of_notes;
	std::cin >> amount_of_notes;

	std::cout << "Введите количество процессов Sender: ";
	int amount_of_senders;
	std::cin >> amount_of_senders;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE* hEventReadyToWork = new HANDLE[amount_of_senders];
	for (int i = 0; i < amount_of_senders; i++)
	{
		std::string creator_cmd = "Sender.exe " + file_name;
		std::wstring converting_creator_to_lpwstr = std::wstring(creator_cmd.begin(), creator_cmd.end());
		LPWSTR lpszCreatorProcessCommandLine = &converting_creator_to_lpwstr[0];

		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);

		if (!CreateProcess(NULL, lpszCreatorProcessCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
			std::cout << "Sender.exe не был запущен.\n";
			return GetLastError();
		}

		hEventReadyToWork[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		SetEvent(hEventReadyToWork[i]);

		CloseHandle(pi.hProcess);
	}

	WaitForMultipleObjects(amount_of_senders, hEventReadyToWork, TRUE, INFINITE);

	bin_file.open(file_name, std::ios::in);
	while (true)
	{
		std::cout << "Введите что желаете делать далее?\n\t1 - прочитать сообщение из бинарного файла.\n\t2 - завершить работу.\n";
		short ans;
		std::cin >> ans;

		if (ans == 1)
		{
			char message[20] = {'\0'};
			bin_file >> message;
			std::cout << message;
		}
		else if (ans == 2)
		{
			std::cout << "Завершение работы...\n";
			break;
		}
		else
		{
			std::cout << "Неправильный ввод! Попробуйте ещё раз.\n";
		}
	}

	for (int i = 0; i < amount_of_senders; i++)
	{
		CloseHandle(hEventReadyToWork[i]);
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	bin_file.close();

	return 0;
}