int add(int a, int b) {
	return a+b;
}

int sub(int a, int b) {
	return a-b;
}

int equal(int a, int b) {
	return !(a-b);
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

	return 0;
}
