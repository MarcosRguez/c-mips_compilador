bool EsPrimo(int numero) {
  if (numero == 2 || numero == 3) {
    return true;
  }
  if (numero <= 1 || numero % 2 == 0 || numero % 3 == 0) {
    return false;
  }
  for (int i{5}; i * i <= numero; i += 6) {
    if (numero % i == 0 || numero % (i + 2) == 0) {
      return false;
    }
  }
  return true;
}

int main() {
  EsPrimo(6);
  return 0;
}