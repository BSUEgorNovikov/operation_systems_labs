#include <fstream>
#include <vector>
#include <windows.h>
#include <iostream>
#include <conio.h>

struct employee
{
	int num;
	char name[10];
	double hours;
};

std::string file_name;
int amount_of_employees;
employee* employees;
int amount_of_clients;
HANDLE* hPipes;
HANDLE* hThreads;
HANDLE* hSemaphores;
HANDLE* hProcesses;

DWORD WINAPI ServerOperations(LPVOID pipe)
{
	HANDLE hPipe = (HANDLE)pipe;
	DWORD bytesRead = 0;
	DWORD bytesWrite = 0;

	int message;
	while (true)
	{
		ReadFile(hPipe, &message, sizeof(message), &bytesRead, NULL);

		int ID = message / 10;
		int option = message % 10;
		if (option == 1)
		{
			for (int i = 0; i < amount_of_clients; i++)
			{
				WaitForSingleObject(hSemaphores[ID - 1], INFINITE);
			}

			employee* emp = new employee;
			*emp = employees[ID - 1];
			bool written = WriteFile(hPipe, emp, sizeof(employee), &bytesWrite, NULL);
			if (written)
			{
				std::cout << "Данные отправлены\n";
			}
			else
			{
				std::cout << "Данные не были отправлены\n";
			}

			bool read = ReadFile(hPipe, emp, sizeof(employee), &bytesWrite, NULL);
			if (!read)
			{
				std::cout << "Невозможно прочесть данные\n";
			}
			employees[ID - 1] = *emp;

			std::ofstream fout(file_name);
			for (int i = 0; i < amount_of_employees; i++)
			{
				fout << employees[i].num
					<< " " << employees[i].name
					<< " " << employees[i].hours << "\n";
			}
			fout.close();

			int msg;
			read = ReadFile(hPipe, &msg, sizeof(msg), &bytesWrite, NULL);
			if (!read)
			{
				std::cout << "Невозможно прочесть данные\n";
			}

			if (msg == 1)
			{
				for (int i = 0; i < amount_of_clients; i++)
				{
					ReleaseSemaphore(hSemaphores[ID - 1], 1, NULL);
				}
			}
		}
		if (option == 2)
		{
			WaitForSingleObject(hSemaphores[ID - 1], INFINITE);

			employee* emp = new employee();
			*emp = employees[ID - 1];
			bool written = WriteFile(hPipe, emp, sizeof(employee), &bytesWrite, NULL);
			if (written)
			{
				std::cout << "Данные отправлены\n";
			}
			else
			{
				std::cout << "Данные не были отправлены\n";
			}

			int msg;
			bool read = ReadFile(hPipe, &msg, sizeof(msg), &bytesWrite, NULL);
			if (!read)
			{
				std::cout << "Невозможно прочесть данные\n";
			}

			if (msg == 1)
			{
				ReleaseSemaphore(hSemaphores[ID - 1], 1, NULL);
			}
		}
		if (option == 3)
		{
			break;
		}

	}

	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	return 0;
}

int main()
{
	setlocale(LC_ALL, "ru");

	std::cout << "Введите имя файла: ";
	std::cin >> file_name;

	std::cout << "Введите кол-во сотрудников: ";
	std::cin >> amount_of_employees;

	employees = new employee[amount_of_employees];
	for (int i = 0; i < amount_of_employees; i++)
	{
		std::cout << "\nВведите информацию о сотруднике " << i + 1 << "\n\n";

		employees[i].num = i + 1;

		std::cout << "Введите имя: ";
		std::cin >> employees[i].name;

		std::cout << "Введите количество отработанных часов: ";
		std::cin >> employees[i].hours;
	}

	std::ofstream fout(file_name);
	for (int i = 0; i < amount_of_employees; i++)
	{
		fout << employees[i].num << " " << employees[i].name << " " << employees[i].hours << "\n";
	}
	fout.close();

	std::cout << "\nСотрудники:\n\n";

	std::ifstream fin(file_name);
	for (int i = 0; i < amount_of_employees; i++)
	{
		int id;
		std::string name;
		double hours;
		fin >> id >> name >> hours;

		std::cout << "ID: " << id << "\tИмя: " << name << "\tОтработанные часы: " << hours << "\n\n";
	}
	fin.close();

	std::cout << "Введите кол-во клиентов: ";
	std::cin >> amount_of_clients;

	hSemaphores = new HANDLE[amount_of_employees];
	for (int i = 0; i < amount_of_employees; i++)
	{
		hSemaphores[i] = CreateSemaphore(NULL, amount_of_clients, amount_of_clients, L"semaphore");
	}

	hProcesses = new HANDLE[amount_of_clients];
	for (int i = 0; i < amount_of_clients; ++i)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		std::string cmd = "Client.exe";
		std::wstring strToWstr = std::wstring(cmd.begin(), cmd.end());
		LPWSTR clientCmdLine = &strToWstr[0];

		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);

		CreateProcess(NULL, clientCmdLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		hProcesses[i] = CreateEvent(NULL, FALSE, FALSE, L"Process Started");

		CloseHandle(pi.hProcess);
	}
	WaitForMultipleObjects(amount_of_clients, hProcesses, TRUE, INFINITE);

	hPipes = new HANDLE[amount_of_clients];
	hThreads = new HANDLE[amount_of_clients];
	for (int i = 0; i < amount_of_clients; i++)
	{
		hPipes[i] = CreateNamedPipe(L"\\\\.\\pipe\\pipeName", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, INFINITE, NULL);
		hThreads[i] = CreateThread(NULL, 0, ServerOperations, static_cast<LPVOID>(hPipes[i]), 0, NULL);
	}
	WaitForMultipleObjects(amount_of_clients, hThreads, TRUE, INFINITE);

	std::cout << "\nСотрудники:\n\n";

	fin.open(file_name);
	for (int i = 0; i < amount_of_employees; i++)
	{
		int id;
		std::string name;
		double hours;
		fin >> id >> name >> hours;

		std::cout << "ID: " << id << "\tИмя: " << name << "\tОтработанные часы: " << hours << "\n\n";
	}
	fin.close();

	delete[] hProcesses;
	delete[] hSemaphores;
	delete[] hPipes;
	delete[] hThreads;

	_getch();

	return 0;
}