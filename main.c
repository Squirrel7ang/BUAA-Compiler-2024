int add(int a, int b) {
	return a+b;
}

int sub(int a, int b) {
	return a-b;
}

int equal(int a, int b) {
	return a==b;
}

int main() {
	// +-*/
	int a = 10, b = -1;
	add(a, a);
	
	int c = a - b;
	int d = a * b;
	int e = a / (b + 4);
	int f = a % (b + 4);

	// branch
	if (a < b) {
		sub(b, a);
	}
	else if (a > b || a == b) {
		sub(a, b);
	}
	if (a == b) {
		equal(a, b);
	}

	// while loop
	int i = 0;
	int len = 10;
	while (i < 10) {
		i++;	
	}

	// for loop
	i = 0;
	for (int i = 0; i < 20; i += 2) {
		i -= 1;
	}

	// 1d array
	int arr1[5];
	for (int i = 0; i < 5; i++) {
		// write arr;
		arr1[i] = i;
	}

	for (int j = 0; j < 5; j++) {
		// read arr
		int k = arr1[i] + 1;
	}

	// 2d array
	int arr2[2][3];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			arr2[i][j] = i*10 + j;
		}
	}


	return 0;
}
