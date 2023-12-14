#include <conio.h>
#include <windows.h>
#include <iostream>

struct employee
{
	int num;
	char name[10];
	double hours;
};

int main(int argc, char* argv)
{
	setlocale(LC_ALL, "ru");

	HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"Process Started");
	SetEvent(hEvent);

	HANDLE hPipe = CreateFile(L"\\\\.\\pipe\\pipeName", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	int ans = 0;
	while (true)
	{
		std::cout << "Выберите метод\n1. Модифицировать запись\n2. Прочитать запись\n3. Завершить процесс\n";

		std::cin >> ans;

		if (ans == 1)
		{
			DWORD bytesWriten = 0;
			DWORD bytesRead = 0;

			int ID;
			std::cout << "Введите ID сотрудника: ";
			std::cin >> ID;

			int msg = ID * 10 + ans;
			bool writen = WriteFile(hPipe, &msg, sizeof(msg), &bytesWriten, NULL);
			if (writen)
			{
				std::cout << "Сообщение отправлено\n";
			}
			else
			{
				std::cout << "Сообщение не было отправлено\n";
			}

			employee* emp = new employee();
			bool read = ReadFile(hPipe, emp, sizeof(employee), &bytesRead, NULL);

			std::cout << "\nID: " << emp->num << "\n" << "Имя: " << emp->name << "\n" << "Отработанные часы: " << emp->hours << "\n";

			std::cout << "Введите новое имя: ";
			std::cin >> emp->name;

			std::cout << "Введите новое кол-во отработанных часов: ";
			std::cin >> emp->hours;

			writen = WriteFile(hPipe, emp, sizeof(employee), &bytesWriten, NULL);
			if (writen)
			{
				std::cout << "Сообщение отправлено\n";
			}
			else
			{
				std::cout << "Сообщение не было отправлено\n";
			}

			msg = 1;
			WriteFile(hPipe, &msg, sizeof(msg), &bytesWriten, NULL);
		}
		else if (ans == 2)
		{
			DWORD bytesWritten = 0;
			DWORD bytesRead = 0;

			int ID;
			std::cout << "Введите ID сотрудника: ";
			std::cin >> ID;

			int msg = ID * 10 + ans;
			bool writen = WriteFile(hPipe, &msg, sizeof(msg), &bytesWritten, NULL);
			if (writen)
			{
				std::cout << "Сообщение отправлено\n";
			}
			else
			{
				std::cout << "Сообщение не было отправлено\n";
			}

			employee* emp = new employee();
			bool read = ReadFile(hPipe, emp, sizeof(employee), &bytesRead, NULL);
			if (!read)
			{
				std::cout << "Сообщение не было прочитано\n";
			}

			std::cout << "\nID: " << emp->num << "\n" << "Имя: " << emp->name << "\n" << "Отработанные часы: " << emp->hours << "\n";

			msg = 1;
			WriteFile(hPipe, &msg, sizeof(msg), &bytesWritten, NULL);
		}
		else if (ans == 3)
		{
			DWORD bytesWritten;
			DWORD bytesRead;

			int msg = ans;
			bool writen = WriteFile(hPipe, &msg, sizeof(msg), &bytesWritten, NULL);
			if (writen)
			{
				std::cout << "Сообщение отправлено\n";
			}
			else
			{
				std::cout << "Сообщение не было отправлено\n";
			}

			break;
		}
		else
		{
			std::cout << "Некоректный ввод. Попробуйте ещё раз\n\n";
		}
	}

	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	return 0;
}