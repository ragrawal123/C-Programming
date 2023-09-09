/* Name, hw4.c, CS 24000, Spring 2023
 * Last updated Dec 8, 2022
 */

/* Add any includes here */
#include "hw4.h"
#include <math.h>
#include <stdio.h>

/* Define your functions here */

/*
 * This function adds the components of two different complex_t components,
 * x and y, from two different arguments together to get a new complex_t
 * number that is then returned. We are basically adding components of
 * complex numbers together.
 */

complex_t add_complex(complex_t arg1, complex_t arg2) {
  complex_t return_value = {0, 0};
  double x = arg1.x + arg2.x;
  double y = arg1.y + arg2.y;
  return_value.x = x;
  return_value.y = y;
  return return_value;
} /* add_complex() */

/*
 * This function returns the negative values of both the x and y component
 * of the complex_t argument passed in. It then assigns that to a complex_t
 * number that is then returned.
 */

complex_t neg_complex(complex_t arg) {
  arg.x = -1.0 * arg.x;
  arg.y = -1.0 * arg.y;
  return arg;
} /* neg_complex() */

/*
 * This function subtracts the components of two different complex_t components,
 * x and y, from two different arguments together to get a new complex_t
 * number that is then returned. We are basically adding components of
 * complex numbers together.
 */

complex_t sub_complex(complex_t arg1, complex_t arg2) {
  return add_complex(arg1, neg_complex(arg2));
} /* sub_complex() */

/*
 * This function takes the dot product of the two complex arguments by
 * multiplying the x-components together and then multipling the y
 * components together of the different complex_t arguments and then
 * add the values together for the dot product.
 */

double dot_complex(complex_t arg1, complex_t arg2) {
  double x = arg1.x * arg2.x;
  double y = arg1.y * arg2.y;
  return (x + y);
} /* dot_complex() */

/*
 * This function gets the reciprocal of the argument passed in and then
 * returns the new argument made. This is done by calculating the
 * magnitude of the complex arguments and then finding the two
 * separate components of the complex_t argument.
 */

complex_t inv_complex(complex_t arg) {
  double magnitude = (arg.x * arg.x) + (arg.y * arg.y);
  arg.x = arg.x / magnitude;
  arg.y = -1 * (arg.y / magnitude);
  return arg;
} /* inv_complex() */

/*
 * This function multiplies two complex numbers together separately
 * for the x and y components. It does two different formulas for the x and y
 * component and then returns the final complex_t argument.
 */

complex_t mul_complex(complex_t arg1, complex_t arg2) {
  complex_t final_arg = {0, 0};
  final_arg.x = (arg1.x * arg2.x) - (arg1.y * arg2.y);
  final_arg.y = (arg1.x * arg2.y) + (arg1.y * arg2.x);

  return final_arg;
} /* mul_complex() */

/*
 * This function multiplies two complex numbers together separately
 * for the x and y components. It does two different formulas for the x and y
 * component and then returns the final complex_t argument.
 */

complex_t div_complex(complex_t arg1, complex_t arg2) {
  return mul_complex(arg1, inv_complex(arg2));
} /* div_complex() */

/*
 * This function returns the compex_t argument number created by doing
 * e^(arg) which is a complex number. This is done by using a formula created
 * from Euler's Identity and solving for our corresponding x and y components.
 */

complex_t exp_complex(complex_t arg) {
  double x_exponent_arg = exp(arg.x) * cos(arg.y);
  double y_exponent_arg = exp(arg.x) * sin(arg.y);
  arg.x = x_exponent_arg;
  arg.y = y_exponent_arg;
  return arg;
} /* exp_complex() */

/*
 * This function computes the mandelbrot for the argument passed in given
 * a complex_t Z. This is done by checking conditions and using a formuala
 * to do the work.
 */

int mandelbrot(complex_t arg) {
  complex_t z = {0.0, 0.0};
  int num_calcs = 0;
  while (dot_complex(z, z) < 4.0) {
    z = add_complex((mul_complex(z, z)), arg);
    num_calcs++;
    if (num_calcs > MAX_MANDELBROT) {
      return MAX_MANDELBROT;
    }
  }
  return num_calcs;
} /* mandelbrot() */


/* Remember, you don't need a main function!
 * It is provided by hw4_main.c or hw4_test.o
 */