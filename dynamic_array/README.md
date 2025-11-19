# Dynamic Array in C

This project is a simple dynamic array implementation written in C. I built it to understand how resizable arrays work under the hood without relying on standard library abstractions.

## Features
- Manual memory management  
- Automatic resizing when capacity is reached  
- `push`, `pop`, `get`, and `set` operations  
- Internal growth strategy using doubling  
- Basic bounds checking  

## How to Compile
```bash
gcc main.c -o dynamic_array
./dynamic_array