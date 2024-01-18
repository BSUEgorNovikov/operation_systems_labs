#include <iostream>
#include <Windows.h>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <algorithm>
#include <condition_variable>


std::mutex mtx;
std::condition_variable cv;
std::vector<bool> threadExited;
std::vector<bool> threadSleep;


void marker(size_t id, std::vector<int>& arr)
{
	srand(id);

	int size = arr.size();
	int countOfColored = 0;

	while (true)
	{
		std::unique_lock<std::mutex> ul(mtx);
		if (threadExited[id] == 1)
		{
			ul.unlock();
			break;
		}

		cv.wait(ul, [&]
			{
				return threadSleep[id] == 0;
			});

		if (threadExited[id] == 1)
		{
			ul.unlock();
			cv.notify_all();
			break;
		}

		int rng = rand() % size;
		if (arr[rng] == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			arr[rng] = id + 1;

			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			countOfColored++;
		}
		else
		{
			std::cout << "\nID потока: " << id + 1 << "\n"
				<< "Кол-во уникальных помеченных элементов: " << countOfColored << "\n"
				<< "Невозможно пометить элемент под номером: " << rng << "\n\n";

			threadSleep[id] = 1;
			countOfColored = 0;
		}

		ul.unlock();
		cv.notify_all();
	}

	cv.notify_all();
}


int main()
{
	setlocale(LC_ALL, "ru");

	int size;
	std::cout << "Введите размер массива: ";
	while (true)
	{

		std::cin >> size;

		if (size <= 0)
		{
			std::cout << "Некоректный ввод! Попробуйте ещё раз.\n"
				<< "Введите размер массива: ";
			continue;
		}

		break;
	}

	std::vector<int> arr(size, 0);

	int countOfThreads;
	std::cout << "Введите количество потоков marker: ";
	while (true)
	{

		std::cin >> countOfThreads;

		if (countOfThreads <= 0)
		{
			std::cout << "Некоректный ввод! Попробуйте ещё раз.\n"
				<< "Введите количество потоков marker: ";
			continue;
		}

		break;
	}

	std::vector<std::thread> threads;
	threadExited.resize(countOfThreads, 0);
	threadSleep.resize(countOfThreads, 0);

	for (int i = 0; i < countOfThreads; i++)
	{
		threads.emplace_back(marker, i, std::ref(arr));
	}

	cv.notify_all();

	int amountOfExitedThreads = 0;
	while (amountOfExitedThreads < countOfThreads)
	{
		std::unique_lock<std::mutex> ul(mtx);

		cv.wait(ul, [&]
			{return std::find(threadSleep.begin(), threadSleep.end(), 0) == threadSleep.end(); });

		if (amountOfExitedThreads == countOfThreads - 1)
		{
			ul.unlock();
			break;
		}

		for (auto i : arr)
		{
			std::cout << i << " ";
		}

		int stopId;
		std::cout << "\nВведите ID потока, который хотите остановить: ";
		while (true)
		{
			std::cin >> stopId;
			if (stopId <= 0 || stopId > countOfThreads)
			{
				std::cout << "\n\nНекоректный ввод! Попробуйте ещё раз.\n"
					<< "Введите ID потока, который хотите остановить: ";
			}
			else
			{
				break;
			}
		}

		for (size_t i = 0; i < size; i++)
		{
			if (arr[i] == stopId)
			{
				arr[i] = 0;
			}
		}

		threadExited[stopId - 1] = true;
		threadSleep = threadExited;

		amountOfExitedThreads++;
		ul.unlock();
		cv.notify_all();
	}

	for (size_t i = 0; i < countOfThreads; i++)
	{
		threads[i].detach();
	}

	std::cout << "\n\nКонечный массив:\n";
	for (auto i : arr)
	{
		std::cout << i << " ";
	}

	return 0;
}