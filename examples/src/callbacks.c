#include <stdio.h>

float plus     (float a, float b) { return a+b; }
float minus    (float a, float b) { return a-b; }
float multiply (float a, float b) { return a*b; }
float divide   (float a, float b) { return a/b; }

typedef enum foo {add, sub, mul, div} op_t;

float operation(float a, float b, op_t op) {
  float result = 0.0;

  switch(op) {
  case add : result = plus (a, b); break;
  case sub : result = minus (a, b); break;
  case mul : result = multiply (a, b); break;
  case div : result = divide (a, b);
  }
  return result;
}

float calculate(float a, float b, float (*callback)(float, float)) {
  return callback(a, b);
}

int main(void) {
  printf("Switch:   2*5 = %f\n", operation(2.0,5.0, mul));

  printf("Callback: 2+5 = %f\n", calculate(2, 5, plus));
}
