int f1(int a, char b) {

    return a+b;
}

void f2(int a[], char b[], int na, int nb) {
    int i = 0;
//    for (i=0; i<na; i=i+1) {
//        printf("a[i] = %d\n", a[i]);
//    }
//    for (i=0; i<nb; i=i+1) {
//        printf("b[i] = '%c'\n", b[i]);
//    }
}

char f3() {
    return 'y';
}

int main() {
    int a[4] = {1, 2, 3, 4};
    char b[4] = {'a', 'b', 'c', 'd'};
    int in = 4;
    char ch = 'a';

    // funcCall
    f1(a[0], in);
    f1(0, ch + 1);

    f2(a, b, in, 4);

    // use return value;
    ch = f3();
    in = f1(in, 0);

    return 0;
}