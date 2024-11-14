#include <stdio.h>

int getchar(){
    char c;
    scanf("%c",&c);
    return (int)c;
}
int getint(){
    int t;
    scanf("%d",&t);
    while(getchar()!='\n');
    return t;
}

int main() {
    int a;
    int arr[4] = {0, 1, 2, 3};
    char c;
    char crr[4] = {'0', '1', '2', '\0'};

    a = getint();
    arr[arr[3]] = getint();
    c = getchar();
    crr[2] = getchar();

    printf("%d %d %c %c", a, arr[3], c, crr[2]);

    return 0;
}