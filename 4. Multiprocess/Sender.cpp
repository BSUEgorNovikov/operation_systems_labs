#include <fstream>
#include <windows.h>
#include <conio.h>
#include <iostream>

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru");

    std::string file_name = argv[1];
    std::fstream file;

    HANDLE hStartEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"Process Started");
    HANDLE hInputSemaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, L"Input Semaphore started");
    HANDLE hOutputSemaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, L"Output Semaphore started");
    SetEvent(hStartEvent);

    std::cout << "Что вы желаете cделать?\n\t1 - Ввести сообщение.\n\t0 - Завершить выполнение процесса.\n";
    short ans;
    std::cin >> ans;

    while (true)
    {
        if (ans == 1)
        {
            file.open(file_name, std::ios::binary | std::ios::out | std::ios::app);

            char message[20];

            std::cout << "Введите сообщение:\n";
            std::cin >> message;

            if (ReleaseSemaphore(hInputSemaphore, 1, NULL) != 1)
            {
                std::cout << "Файл полон.";
                ReleaseSemaphore(hInputSemaphore, 1, NULL);
            }

            WaitForSingleObject(hOutputSemaphore, INFINITE);
            
            file.write(message, sizeof(message));

            file.close();
            std::cout << "\nЧто вы желаете делать дальше?\n\t1 - Ввести сообщение.\n\t0 - Завершить выполнение процесса.\n";
            std::cin >> ans;
        }
        else if (ans == 0)
        {
            std::cout << "Завершение процесса...";
            break;
        }
        else
        {
            std::cout << "\nНеправильный ввод!\nПопробуйте ещё раз.";
            std::cin >> ans;
        }
    }

    return 0;
}
