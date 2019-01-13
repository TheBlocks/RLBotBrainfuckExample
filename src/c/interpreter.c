#include "interpreter.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

const int CELLS_SIZE = 30000;

typedef enum
{
    StartBracket,
    EndBracket,
    AbsoluteTeleport,
    RelativeTeleport,
    Add,
    Input,
    Output,
    Pass,
    DebugPrint
} InsType;

struct instruction
{
    InsType type;
    short value;
};

typedef struct length_instruction_array
{
    unsigned short n;
    instruction *array;
} length_instruction_array;

struct length_char_array
{
    unsigned short n;
    char *array;
};

__declspec(dllexport) length_instruction_array get_instructions(char *source)
{
    // Remove comments
    char *pr = source, *pw = source;
    char commands[] = "+-<>.,[]#";
    while (*pr)
    {
        *pw = *pr++;
        bool ignore_char = true;
        for (int i = 0; i < strlen(commands); i++)
        {
            if (*pw == commands[i])
            {
                ignore_char = false;
                break;
            }
        }
        if (!ignore_char)
            ++pw;
    }
    *pw = '\0';

    unsigned short source_len = (unsigned short) strlen(source);
    instruction *instructions = (instruction *) malloc(sizeof(instruction) * source_len);

    // Convert to intermediate representation, match brackets, optimise + - < >
    unsigned short start_bracket_stack[15000];
    unsigned short start_bracket_num = 0;
    short last_add = -1;
    short last_point = -1;

    for (unsigned short i = 0; i < source_len; i++)
    {
        switch (source[i])
        {
            case '-':
            case '+':
                last_point = -1;

                char add_num = 1;
                if (source[i] == '-')
                    add_num = -1;
                if (last_add == -1)
                {
                    last_add = i;
                    instructions[i].type = Add;
                    instructions[i].value = add_num;
                }
                else
                {
                    instructions[last_add].value += add_num;
                    instructions[i].type = Pass;
                }
                // TODO: Add AbsoluteTeleport right after the first add_num. It should not go through all these Passes.
                break;
            case '>':
            case '<':
            {
                last_add = -1;

                short point_num = 1;
                if (source[i] == '<')
                    point_num = -1;
                if (last_point == -1)
                {
                    last_point = i;
                    instructions[i].type = RelativeTeleport;
                    instructions[i].value = point_num;
                }
                else
                {
                    instructions[last_point].value += point_num;
                    instructions[i].type = Pass;
                }
                break;
            }
            case '.':
                last_point = -1;
                last_add = -1;
                instructions[i].type = Output;
                break;
            case ',':
                last_point = -1;
                last_add = -1;
                instructions[i].type = Input;
                break;
            case '[':
                last_point = -1;
                last_add = -1;
                start_bracket_stack[++start_bracket_num] = i;
                break;
            case ']':
                last_point = -1;
                last_add = -1;
                unsigned short start_bracket = start_bracket_stack[start_bracket_num];
                instructions[start_bracket].type = StartBracket;
                instructions[start_bracket].value = i;
                instructions[i].type = EndBracket;
                instructions[i].value = start_bracket;
                --start_bracket_num;
                break;
            case '#':
                instructions[i].type = DebugPrint;
                break;
            case '\000':
                break;
            default:
                printf("Found unexpected character %c\nSource: %s\nSource length:%i", source[i], source, source_len);
                break;
        }
    }

    length_instruction_array ret;
    ret.n = source_len;
    ret.array = instructions;
    return ret;
}

__declspec(dllexport) length_char_array
run_code(instruction *instructions, unsigned short total_instructions, const char *inputs, unsigned short inputs_len)
{
    // Run brainfuck code
    char *cells = (char *) malloc(sizeof(char) * CELLS_SIZE);
    char *outputs = (char *) malloc(sizeof(char) * CELLS_SIZE);
    for (int i = 0; i < CELLS_SIZE; i++)
    {
        cells[i] = 0;
    }
    for (int i = 0; i < CELLS_SIZE; i++)
    {
        outputs[i] = 0;
    }
    short instruction_pos = 0;
    short cell_pos = 0;
    unsigned short inputs_pos = 0;
    unsigned short outputs_pos = 0;

    while (true)
    {
        switch (instructions[instruction_pos].type)
        {
            case RelativeTeleport:
                // Wrap value from 0 to CELLS_SIZE
                cell_pos += instructions[instruction_pos].value;
                cell_pos = (unsigned short) min(max(cell_pos, 0), CELLS_SIZE);
                break;
            case Add:
                cells[cell_pos] += instructions[instruction_pos].value;
                break;
            case Output:
                outputs[outputs_pos] = cells[cell_pos];
                ++outputs_pos;
                break;
            case Input:
                if (inputs_pos >= inputs_len)
                {
                    // Give it a 0 if we ran out of supplied inputs
                    printf("%s", "WARNING: Requested input but ran out of supplied inputs! Giving 0 as an input.\n");
                    cells[cell_pos] = 0;
                }
                else
                {
                    cells[cell_pos] = inputs[inputs_pos];
                    ++inputs_pos;
                }
                break;
            case StartBracket:
                if (cells[cell_pos] == 0)
                    instruction_pos = instructions[instruction_pos].value;
                break;
            case EndBracket:
                if (cells[cell_pos] != 0)
                    instruction_pos = instructions[instruction_pos].value;
                break;
            case AbsoluteTeleport:
                instruction_pos = instructions[instruction_pos].value;
                break;
            case Pass:
                break;
            case DebugPrint:
                // Prints the cell and its value for 20 cells, starting from the current cell position.
                puts("\nDebug\n");
                puts("cell: value");
                for (int i = cell_pos; i < cell_pos + 20; i++)
                {
                    printf("%i: %i\n", i, cells[i]);
                }
                puts("\nEnd debug\n");
                break;
            default:
                printf("\nBad value reached: %i, instruction_pos: %i\n", instructions[instruction_pos].type, instruction_pos);
                length_char_array ret;
                ret.n = 0;
                ret.array = cells;
                return ret;
        }

        instruction_pos++;
        if (instruction_pos == total_instructions)
        {
            length_char_array ret;
            ret.n = CELLS_SIZE;
            ret.array = outputs;
            return ret;
        }
    }
}