// global declaration
int gbIntArrNonInit[4], gbIntArrInit[4] = {0, 1, 2, 3};
char gbCharArrNonInit[4], gbCharArrInit[4] = {'0', '1', 16, 126};
const int gbConstIntArr[4] = {4, 5, 6};
const char gbConstCharArr[4] = "321";
int gbIntNonInit;
int gbIntInit = 1;
char gbCharNonInit;
char gbCharInit = 'a';
const int gbConstInt = 1;
const char gbConstChar = 'a';


int main() {
    // local declaration
    int intArrNonInit[6];
    char charArrNonInit[5];
    int intArrInit[4] = {0, 1, 2, 3};
    char charArrInit[4] = {'0', '1', '2', '3'};

    // modification
    intArrNonInit[0] = 1;
    intArrNonInit[intArrNonInit[0]] = 2;
    intArrNonInit[2] = 3;
    intArrNonInit[3] = 4;

    gbCharArrNonInit[0] = '0';
    gbCharArrNonInit[1] = '1';
    gbCharArrNonInit[2] = '2';
    gbCharArrNonInit[3] = '3';

    // read Out
    intArrNonInit[0] = gbConstIntArr[0];
    intArrNonInit[1] = intArrNonInit[1];
    intArrNonInit[2] = charArrInit[2] - '0';
    intArrNonInit[3] = gbConstCharArr[0];
    return 0;
}