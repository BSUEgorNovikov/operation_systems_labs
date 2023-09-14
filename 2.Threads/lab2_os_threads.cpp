#include <iostream>
#include <thread>
#include <Windows.h>

void GetMinAndMax(double* arr, int size, std::pair<double, double> &min_max)
{
	std::cout << "\tПоток min_max был запущен.\n";

	double min = arr[0];
	double max = arr[0];

	for (int i = 1; i < size; i++)
	{
		Sleep(7);
		if (arr[i] > max)
			max = arr[i];

		Sleep(7);
		if (arr[i] < min)
			min = arr[i];
	}

	min_max.first = min;
	min_max.second = max;

	std::cout << "Минимальный элемент: " << min << "\nМаксимальный элемент: " << max << "\n\tПоток min_max был завершён.\n";
}

void GetAverage(double* arr, int size, double& result)
{
	std::cout << "\tПоток average был запущен.\n";

	for (int i = 0; i < size; i++)
	{
		result += arr[i];
		Sleep(12);
	}
	result /= size;

	std::cout << "Среднее арифметическое значение элементов массива: " << result << "\n\tПоток average был завершён.\n";
}

int main()
{
	setlocale(LC_ALL, "Ru");

	std::cout << "\tПоток main был запущен.\n";

	int n;
	std::cout << "Введите размерность массива: ";
	std::cin >> n;

	double* arr = new double[n];
	for (int i = 0; i < n; i++)
	{
		std::cout << "Введите элемент №" << i + 1 << ": ";
		std::cin >> arr[i];
	}

	std::pair<double, double> min_and_max;
	std::thread min_max(GetMinAndMax, arr, n, std::ref(min_and_max));

	double average_result = 0;
	std::thread average(GetAverage, arr, n, std::ref(average_result));

	min_max.join();
	average.join();

	WaitForSingleObject(min_max.native_handle(), INFINITE);
	WaitForSingleObject(average.native_handle(), INFINITE);

	for (int i = 0; i < n; i++)
	{
		if (arr[i] == min_and_max.first || arr[i] == min_and_max.second)
			arr[i] = average_result;
	}

	std::cout << "Изменённый массив: ";
	for (int i = 0; i < n; i++)
	{
		std::cout << arr[i] << " ";
	}

	std::cout << "\n\tПоток main был завершён.\n";
	return 0;
}