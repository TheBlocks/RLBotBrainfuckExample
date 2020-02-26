# RLBotBrainfuckExample
An example Rocket League bot written in brainfuck.

## Changing the bot

- Bot behavior is controlled by `src/brainfuck/bot.bf`
- The inputs that the bot gets is controlled by the Python code in the `inputs` option in `src/agent.cfg`

## Brainfuck implementation details

This bot uses a relatively standard implementation of brainfuck. Here are the details:
- There is a 30,000 cell tape, where each cell is initialised to zero
- Each cell has a value range of an 8-bit `char` (-128 to 127)
- Cell overflow/underflow wraps the cell to the opposite end of the cell range
- Moving the pointer to the left (`<`) of cell 0 limits the pointer to cell 0
- Moving the pointer to the right (`>`) of cell 29,999 limits the pointer to cell 29,999
- The `#` symbol can be used for debug printing

## Troubleshooting

### Printing debug values
You can place the pound/hash symbol (`#`) in your brainfuck code to print the current cell, its value, and the value for the following for 19 cells.

### %1 is not a valid Win32 application
If you get this error, `src/python/interpreter.dll` hasn't been compiled with the same architecture as your Python environment. To fix this, compile the interpreter project (project provided in `CMakeLists.txt`) with the right architecture.