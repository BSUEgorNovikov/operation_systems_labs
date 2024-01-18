#pragma once
#include <vector>

class markerToTest
{

public:
	int count_marked_elements(std::vector<int> arr,int value) {
		int amount_of_asked_element = 0;

		for (int i = 0; i < arr.size(); i++) {
			if (arr[i] == value)
				amount_of_asked_element++;
		}

		return amount_of_asked_element;
	}

	std::vector<int> thread_delete_result(std::vector<int> arr, int value) {
		for (int i = 0; i < arr.size(); i++) {
			if (arr[i] == value)
				arr[i] = 0;
		}

		return arr;
	}
};