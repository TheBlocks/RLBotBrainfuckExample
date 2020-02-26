import ctypes
import pathlib


class Instruction(ctypes.Structure):
    _fields_ = [
        ('type', ctypes.c_int),
        ('value', ctypes.c_short)
    ]


class LengthInstructionArray(ctypes.Structure):
    _fields_ = [
        ('n', ctypes.c_ushort),
        ('array', ctypes.POINTER(Instruction))
    ]


class LengthCharArray(ctypes.Structure):
    _fields_ = [
        ('n', ctypes.c_ushort),
        ('array', ctypes.POINTER(ctypes.c_byte))
    ]


interpreter_path = pathlib.Path(__file__).parent.absolute() / "interpreter.dll"
dll = ctypes.WinDLL(str(interpreter_path))

get_instructions = dll.get_instructions
get_instructions.argtypes = [ctypes.c_char_p]
get_instructions.restype = LengthInstructionArray

run_code = dll.run_code
run_code.argtypes = [ctypes.POINTER(Instruction), ctypes.c_ushort, ctypes.c_char_p, ctypes.c_ushort]
run_code.restype = LengthCharArray
