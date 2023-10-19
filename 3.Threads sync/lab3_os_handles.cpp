#include <Windows.h>
#include <iostream>

int array_size = 0;
int* arr = nullptr;
CRITICAL_SECTION cs;
HANDLE* handle_threads;
HANDLE* handle_threads_are_started;
HANDLE* handle_threads_are_stopped;
HANDLE* handle_threads_are_exited;
HANDLE handle_mutex;

DWORD WINAPI marker(LPVOID thread_index)
{
	WaitForSingleObject(handle_threads_are_started[(int)thread_index], INFINITE);

	int marked_numbers_counter = 0;
	srand((int)thread_index);

	while (true)
	{
		EnterCriticalSection(&cs);

		int random_number = rand() % array_size;
		if (arr[random_number] == 0)
		{
			Sleep(5);
			arr[random_number] = (int)thread_index + 1;
			marked_numbers_counter++;
			Sleep(5);

			LeaveCriticalSection(&cs);
		}
		else
		{
			std::cout << "\n\tНомер потока: " << (int)thread_index + 1 << "\n\tКоличество помеченных элементов: " << marked_numbers_counter <<
				"\n\tИндекс элемента который невозможно пометить: " << random_number << "\n";

			LeaveCriticalSection(&cs);

			SetEvent(handle_threads_are_stopped[(int)thread_index]);
			ResetEvent(handle_threads_are_started[(int)thread_index]);

			HANDLE handle_thread_started_exited_pair[]{ handle_threads_are_started[(int)thread_index], handle_threads_are_exited[(int)thread_index] };

			if (WaitForMultipleObjects(2, handle_thread_started_exited_pair, FALSE, INFINITE) == WAIT_OBJECT_0 + 1)
			{
				EnterCriticalSection(&cs);
				for (size_t i = 0; i < array_size; i++)
				{
					if (arr[i] == (int)thread_index + 1)
						arr[i] = 0;
				}
				LeaveCriticalSection(&cs);

				ExitThread(NULL);
			}
			else
			{
				ResetEvent(handle_threads_are_stopped[(int)thread_index]);
				continue;
			}
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Ru");

	int amount_of_threads = 0;

	std::cout << "Введите размер массива элементов: ";
	std::cin >> array_size;

	arr = new int[array_size] {};

	std::cout << "Введите количество потоков: ";
	std::cin >> amount_of_threads;

	InitializeCriticalSection(&cs);

	handle_threads = new HANDLE[amount_of_threads];
	handle_threads_are_started = new HANDLE[amount_of_threads];
	handle_threads_are_stopped = new HANDLE[amount_of_threads];
	handle_threads_are_exited = new HANDLE[amount_of_threads];
	handle_mutex = CreateMutex(NULL, FALSE, NULL);

	for (int i = 0; i < amount_of_threads; i++)
	{
		handle_threads[i] = CreateThread(NULL, 1, marker, (LPVOID)i, NULL, NULL);
		handle_threads_are_started[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		handle_threads_are_stopped[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		handle_threads_are_exited[i] = CreateEvent(NULL, TRUE, FALSE, NULL);

	}

	for (int i = 0; i < amount_of_threads; i++)
	{
		SetEvent(handle_threads_are_started[i]);
	}

	int amount_of_completed_threads = 0;
	bool* is_thread_exited = new bool[amount_of_threads] {};
	while (amount_of_completed_threads < amount_of_threads)
	{
		WaitForMultipleObjects(amount_of_threads, handle_threads_are_stopped, TRUE, INFINITE);
		
		handle_mutex = OpenMutex(NULL, FALSE, NULL);

		std::cout << "\nПолученный массив: ";
		for (int i = 0; i < array_size; i++)
		{
			std::cout << arr[i] << " ";
		}
		std::cout << "\n";

		ReleaseMutex(handle_mutex);

		int stop_marker_id;
		std::cout << "\nВведите номер потока, который хотите остановить:\n";
		std::cin >> stop_marker_id;
		stop_marker_id--;

		if (!is_thread_exited[stop_marker_id])
		{
			amount_of_completed_threads++;
			is_thread_exited[stop_marker_id] = true;

			SetEvent(handle_threads_are_exited[stop_marker_id]);
			WaitForSingleObject(handle_threads[stop_marker_id], INFINITE);
			CloseHandle(handle_threads[stop_marker_id]);
			CloseHandle(handle_threads_are_exited[stop_marker_id]);
			CloseHandle(handle_threads_are_started[stop_marker_id]);
		}

		handle_mutex = OpenMutex(NULL, FALSE, NULL);

		std::cout << "\nМассив: ";
		for (int i = 0; i < array_size; i++)
		{
			std::cout << arr[i] << " ";
		}
		std::cout << "\n";

		ReleaseMutex(handle_mutex);

		for (int i = 0; i < amount_of_threads; i++)
		{
			if (!is_thread_exited[i]) {
				ResetEvent(handle_threads_are_stopped[i]);
				SetEvent(handle_threads_are_started[i]);
			}
		}
	}

	for (int i = 0; i < amount_of_threads; i++)
	{
		CloseHandle(handle_threads_are_stopped[i]);
	}
	DeleteCriticalSection(&cs);
	return 0;
}