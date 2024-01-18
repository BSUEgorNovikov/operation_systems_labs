#include "pch.h"
#include "CppUnitTest.h"
#include "../lab3_os_handles/markerToTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace markerUnitTest
{
	TEST_CLASS(markerUnitTest)
	{
	public:
		
		TEST_METHOD(Test_count1)
		{
			markerToTest mtt;
			std::vector<int> arr{ 1, 2, 2, 0, 2, 5 };
			int result = mtt.count_marked_elements(arr, 2);
			Assert::IsTrue(result == 3);
		}

		TEST_METHOD(Test_count2)
		{
			markerToTest mtt;
			std::vector<int> arr{ 1, 1, 1, 1, 1, 1};
			int result = mtt.count_marked_elements(arr, 1);
			Assert::IsTrue(result == 6);
		}

		TEST_METHOD(Test_count3)
		{
			markerToTest mtt;
			std::vector<int> arr{ 1, 2, 2, 0, 2, 5 };
			int result = mtt.count_marked_elements(arr, 999);
			Assert::IsTrue(result == 0);
		}

		TEST_METHOD(Test_deletion1)
		{
			markerToTest mtt;
			std::vector<int> arr{ 1, 2, 3, 5, 6, 5, 2 };
			std::vector<int> res = mtt.thread_delete_result(arr, 5);
			std::vector<int> expeted{ 1,2,3,0,6,0,2 };
			Assert::IsTrue(res == expeted);
		}

		TEST_METHOD(Test_deletion2)
		{
			markerToTest mtt;
			std::vector<int> arr{ 9, 9, 9, 9, 9, 9, 9 };
			std::vector<int> res = mtt.thread_delete_result(arr, 9);
			std::vector<int> expeted{ 0,0,0,0,0,0,0 };
			Assert::IsTrue(res == expeted);
		}

		TEST_METHOD(Test_deletion3)
		{
			markerToTest mtt;
			std::vector<int> arr{ 1, 2, 3, 5, 6, 5, 2 };
			std::vector<int> res = mtt.thread_delete_result(arr, 99);
			std::vector<int> expeted{ 1, 2, 3, 5, 6, 5, 2 };
			Assert::IsTrue(res == expeted);
		}
	};
}
