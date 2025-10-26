#include <vcl.h>
#include <tchar.h>

#include <algorithm> // For std::sort
#include <iostream>
#include <vector>

struct MyObject {
    int value;
    std::string name;

    // Overload the less-than operator for default sorting by 'value'
    bool operator<(const MyObject& other) const {
        return value < other.value;
    }
};

struct MyObject2 {
	double value;
	int freq;
	// Overload the less-than operator for default sorting by 'value'
	bool operator<(const MyObject2& other) const {
		return value > other.value;
	}
};


int _tmain(int argc, _TCHAR* argv[]) {
	std::vector<MyObject> objects = {{3, "C"}, {1, "A"}, {2, "B"}};

	// Sort using the overloaded operator<
	std::sort(objects.begin(), objects.end());

	for (const auto& obj : objects) {
		std::cout << obj.value << " " << obj.name << std::endl;
	}
	//
	std::cout << std::endl;
	struct MyObject objects2[4] = {{-4, "D"}, {3, "C"}, {1, "A"}, {2, "B"}};
	// Sort using the overloaded operator<
	std::sort(objects2, objects2 + 4);

	for (const auto& obj : objects2) {
		std::cout << obj.value << " " << obj.name << std::endl;
	}

	//
	std::cout << std::endl;
	struct MyObject *objects3 = new struct MyObject [5];
	objects3[0] = {-4, "D"};
	objects3[1] = {4, "A"};
	objects3[2] = {1, "B"};
	objects3[3] = {2, "C"};
	objects3[4] = {-3, "E"};
	// Sort using the overloaded operator<
	std::sort(objects3, objects3 + 5);

	for (auto i = 0; i < 5; i++) {
		std::cout << objects3[i].value << " " << objects3[i].name << std::endl;
	}
	delete[] objects3;

	//
	std::cout << std::endl;
	std::vector<MyObject2> objects4 = {{3.16, 6}, {1.18, 2}, {2.64, 87}};
	// Sort using the overloaded operator<
	//std::sort(objects4.begin(), objects4.end(), std::greater<double>());
    std::sort(objects4.begin(), objects4.end());

	for (const auto& obj : objects4) {
		std::cout << obj.value << " " << obj.freq << std::endl;
	}



	return 0;
}