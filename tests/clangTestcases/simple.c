int gb;

int main() {
  char ch = 'a';
  ch = ch + '\x01';
  int a = 1;
  int b;
  b = 1;
  b = a + a;
  a = b + 1;
  gb = a;
  b = gb + 2;
  return a;
}