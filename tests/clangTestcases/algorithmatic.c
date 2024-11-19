int f(int a) {
    return a;
}

int main() {
    int a = 0;
    int b = a + 1;
    int arr[1] = {12};
    int z;
    z = a+b;
    z = a-b;
    z = a*b;
    z = a/b;
    z = a%b;
    z = -a;
    z = !a;
    z = +z;
    z = a&&b;
    z = a||b;
    z = (a!=b);
    z = (a==b);
    z = (a< b);
    z = (a<=b);
    z = (a> b);
    z = (a>=b);
    char zx;
    zx = a;
    a = zx;


    a = (a - b*a) + 2 * (a+1) / b;
    b = (-f(-b) + a + arr[0] - 4) / (a + 2);
    int c = (-f(-b) + a + arr[0] - 4) / (a + 2); // exp in decl

    char c1 = 'a', c2 = 'b';

    // type casting
    c1 = 'a' + a;
    a = c1 - 'a';
    c1 = --a;
    c1 = arr[0] + 'a';
    arr[0] = arr[0] + 'a';
    c2 = arr[0];

    // condition
    int ret = 1;
    if (a + b <= b + a && a != f(a) || b > b-1) {
        ret = 0;
    }

    return ret;
}