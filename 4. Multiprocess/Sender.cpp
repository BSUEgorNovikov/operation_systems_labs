#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");

	std::string file_name = argv[1];

	std::fstream bin_file;
	bin_file.open(file_name, std::ios::out | std::ios::app);

	HANDLE hEventReadyToWork = OpenEvent(NULL, FALSE, NULL);
	SetEvent(hEventReadyToWork);

	while (true)
	{
		std::cout << "Введите что желаете делать далее?\n\t1 - записать сообщение в бинарный файл.\n\t2 - завершить работу.\n";
		short ans;
		std::cin >> ans;

		if (ans == 1)
		{
			std::string message;
			std::cout << "Введите сообщение: ";
			std::cin >> message;
			
			char chstr[20] = {'\0'};
			strcpy_s(chstr, message.c_str());

			bin_file.write(chstr, 20);
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

	bin_file.close();
	return 0;
}