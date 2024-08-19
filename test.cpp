#include <vector>
#include <cstdio>

int main() {
	std::vector<int> arr;
	for (int i = 0 ; i < 14; i++) {
		arr.push_back(i);
	}
	for (int i = 0; i < 14; i++) {
		printf("%d ", arr[i]);
	}
}
