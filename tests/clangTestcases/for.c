int main() {
    int a = 0;
    for (;a<4;a=a+1) {
        ;
    }
    a = 0;
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
}