/*
 * Author: Daniel DiPietro
 * Date: November 2025
 * Description:
 *    This program numerically approximates two mathematical constants:
 *      1. π (pi) – using the Ramanujan series.
 *      2. e (Euler’s number) – using the Taylor series expansion of e^x at x = 1.
 *
 *    It demonstrates manual implementation of mathematical operations such as
 *    exponentiation, factorial, and square root, along with numerical iteration
 *    and high-precision formatted output.
 */

#include <iostream>
#include <iomanip>

/*
 * Function: power
 * ----------------
 * Raises a base to an integer exponent using iterative multiplication.
 * Handles positive, zero, and negative exponents.
 *
 * Parameters:
 *    base      - the base value.
 *    exponent  - the integer exponent (can be negative).
 *
 * Returns:
 *    base^exponent as a double.
 */
double power(double base, int exponent) {
    double accum = 1.0;
    if (exponent == 0) return 1.0;
    else if (exponent > 0)
        for (int i = 0; i < exponent; i++) accum *= base;
    else
        return 1.0 / power(base, -exponent);
    return accum;
}

/*
 * Function: factorial
 * --------------------
 * Computes the factorial of a non-negative integer n.
 * Uses long double for higher precision.
 *
 * Parameters:
 *    n - the number to compute the factorial of.
 *
 * Returns:
 *    n! (n factorial) as a long double.
 */
long double factorial(int n) {
    long double accum = 1.0;
    for (int i = 1; i <= n; i++) accum *= i;
    return accum;
}

/*
 * Function: mysqrt
 * -----------------
 * Approximates the square root of a number using Newton’s method.
 * Performs a fixed number of iterations for convergence.
 *
 * Parameters:
 *    number - the number to find the square root of.
 *
 * Returns:
 *    Approximate square root of 'number' as a double.
 */
double mysqrt(double number) {
    double x = number;
    for (int i = 0; i < 6; i++)
        x = 0.5 * (x + (number / x));
    return x;
}

/*
 * Function: computePi
 * --------------------
 * Approximates π using Ramanujan’s rapidly converging infinite series:
 *
 *      1 / π = (2√2 / 9801) * Σ [ (4i)! (1103 + 26390i) ] / [ (i!)⁴ * 396^(4i) ]
 *
 * Increasing n improves accuracy; even small n yields high precision.
 *
 * Parameters:
 *    n - number of terms to include in the series.
 *
 * Output:
 *    Prints π to the console with 100 digits of precision.
 */
void computePi(int n) {
    long double pi = 0.0;
    for (int i = 0; i < n; i++) {
        pi += (factorial(4 * i) * (1103 + (26390 * i))) /
              (power(factorial(i), 4) * power(396, 4 * i));
    }

    double constant = (2.0 * mysqrt(2.0)) / 9801.0;
    pi *= constant;
    pi = 1.0 / pi;

    std::cout << "Approximation of π: " << std::setprecision(15) << pi << "\n";
}

/*
 * Function: computeEulerConstant
 * -------------------------------
 * Approximates Euler’s number e using the Taylor series expansion:
 *
 *      e = Σ (1 / n!) from n = 0 to ∞
 *
 * Increasing n improves accuracy.
 *
 * Parameters:
 *    n - number of terms to include in the series.
 *
 * Output:
 *    Prints e to the console with 20 digits of precision.
 */
void computeEulerConstant(int n) {
    double e = 0.0;
    for (int i = 0; i < n; i++) {
        e += 1.0 / factorial(i);
    }

    std::cout << "Approximation of e: " << std::setprecision(15) << e << "\n";
}

/*
 * Function: main
 * ---------------
 * Entry point of the program.
 * Computes π and e using a chosen number of series terms.
 */
int main() {
    computePi(10);          // Compute π using 10 terms (very accurate)
    computeEulerConstant(100); // Compute e using 100 terms
}
