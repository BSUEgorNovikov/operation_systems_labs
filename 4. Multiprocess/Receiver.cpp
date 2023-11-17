#include <fstream>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>

int main()
{
    setlocale(LC_ALL, "ru");

    std::string bin_file_name;
    int amount_of_notes;
    int amount_of_senders;

    std::cout << "Введите имя бинарного файла:\n";
    std::cin >> bin_file_name;
    bin_file_name += ".bin";

    std::cout << "Введите количество записей:\n";
    std::cin >> amount_of_notes;

    std::cout << "Введите количество процессов Sender:\n";
    std::cin >> amount_of_senders;

    HANDLE hInputSemaphore = CreateSemaphore(NULL, 0, amount_of_notes, L"Input Semaphore started");
    HANDLE hOutputSemaphore = CreateSemaphore(NULL, amount_of_notes, amount_of_notes, L"Output Semaphore started");
    HANDLE* hEventStarted = new HANDLE[amount_of_senders];

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    LPWSTR lpwstrSenderProcessCommandLine;
    for (int i = 0; i < amount_of_senders; i++)
    {
        std::string sender_cmd = "Sender.exe " + bin_file_name;
        std::wstring converting_sender_to_lpwstr = std::wstring(sender_cmd.begin(), sender_cmd.end());
        lpwstrSenderProcessCommandLine = &converting_sender_to_lpwstr[0];

        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        if (!CreateProcess(NULL, lpwstrSenderProcessCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            std::cout << "Процесс Sender не запущен.\n";
            return GetLastError();
        }

        hEventStarted[i] = CreateEvent(NULL, FALSE, FALSE, L"Process Started");

        CloseHandle(pi.hProcess);
    }

    WaitForMultipleObjects(amount_of_senders, hEventStarted, TRUE, INFINITE);

    std::cout << "\nЧто вы желаете делать дальше?\n\t1 - Вывести сообщение из файла.\n\t0 - Завершить выполнение программы.\n";
    short ans;
    std::cin >> ans;
    std::ifstream file(bin_file_name, std::ios::binary);
    while (true)
    {
        if (ans == 1)
        {
            char message[20];

            WaitForSingleObject(hInputSemaphore, INFINITE);
            ReleaseSemaphore(hOutputSemaphore, 1, NULL);

            file.read(message, sizeof(message));
            std::cout << "Полученное сообщение: " << message;

            std::cout << "\nЧто вы желаете делать дальше?\n\t1 - Вывести сообщение из файла.\n\t0 - Завершить выполнение программы.\n";
            std::cin >> ans;
        }
        else if (ans == 0)
        {
            std::cout << "Завершение программы...";
            break;
        }
        else
        {
            std::cout << "\nНеправильный ввод! Попробуйте ещё раз.\n";
            std::cin >> ans;
        }
    }

    CloseHandle(hInputSemaphore);
    CloseHandle(hOutputSemaphore);
    for (int i = 0; i < amount_of_senders; i++)
    {
        CloseHandle(hEventStarted[i]);
    }
    file.clear();
    file.close();

    std::cout << "\n";
    system("pause");
    return 0;
}
