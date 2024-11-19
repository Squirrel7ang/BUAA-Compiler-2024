const int gb = 1;
const char gb1 = gb;
int gb2 = gb1;
const int gb3 = gb1;
char gb4 = gb1;
char gb5 = gb1 + gb1;



int main() {
  char ch = 'a';
  ch = ch + '\x01';
  int a = 1;
  int b;
  b = 1;
  b = a + a;
  a = b + 1;
  b = gb + 2;
  return a;
}