#ifndef INTERPRETER_LIBRARY_H
#define INTERPRETER_LIBRARY_H

typedef struct instruction instruction;
typedef struct length_instruction_array length_instruction_array;
typedef struct length_char_array length_char_array;
__declspec(dllexport) length_instruction_array get_instructions(char *source);
__declspec(dllexport) length_char_array run_code(instruction *instructions, unsigned short total_instructions, const char *inputs, unsigned short inputs_len);

#endif