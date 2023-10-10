#include <stdio.h>
#include <stdlib.h>

struct _retire_info {
  int months;
  double contribution;
  double rate_of_return;
};
typedef struct _retire_info retire_info;
double increase_wealth(int age, double balance, retire_info info) {
  for (int i = 0; i < info.months; i++) {
    printf("Age %3d month %2d you have $%.2f\n", (age + i) / 12, (age + i) % 12, balance);
    balance = balance + balance * info.rate_of_return + info.contribution;
  }
  return balance;
}

void retirement(int startAge, double initial, retire_info working, retire_info retired) {
  double balance = increase_wealth(startAge, initial, working);
  increase_wealth(startAge + working.months, balance, retired);
}

int main() {
  retire_info working = {489, 1000, 0.045 / 12};
  retire_info retired = {384, -4000, 0.01 / 12};
  int age = 327;
  double initial = 21345;
  retirement(age, initial, working, retired);
  return 0;
}
