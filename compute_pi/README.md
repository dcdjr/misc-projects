# Numerical Approximations of π and e

This program approximates two constants:

- **π** using the Ramanujan series  
- **e** using the Taylor series expansion of \( e^x \) at \( x = 1 \)

I implemented the supporting math operations myself (power, factorial, and a Newton-based square root) to understand how these calculations work under the hood.

## How to Run
Compile and run:

```bash
g++ -O2 main.cpp -o approx
./approx
