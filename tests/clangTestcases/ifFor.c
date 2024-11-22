int main() {
    int a = 0, b = 1;
    // 短路求值
    int a1, a2, a3,
        b1, b2, b3,
        c1, c2, c3;
    if (a1 && a2 && a3 || b1 && b2 && b3 || c1 && c2 && c3) {
        ;
    }

    if (a + 1 == b || a) {
        a = 1;
    }

    if (a > b && a == b) {
        ;
    }
    else {
        // else stmt
        a = 2;
    }

    // break stmt
    for (;;) {
        break;
    }

    // continue stmt with condition and update stmt
    int i = 0;
    for (i = 0; i < 4; i=i+1) {
        a = a + 1;
        if (a > a-1) {
            continue;
        }
    }

    return a-6;
}