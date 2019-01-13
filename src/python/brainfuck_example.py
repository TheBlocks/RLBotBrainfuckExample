import ctypes
from typing import List

from rlbot.agents.base_agent import BaseAgent, SimpleControllerState, BOT_CONFIG_AGENT_HEADER
from rlbot.parsing.custom_config import ConfigHeader, ConfigObject
from rlbot.utils.structures.game_data_struct import GameTickPacket

from structures import Instruction, LengthInstructionArray, LengthCharArray, get_instructions, run_code


class BrainfuckExample(BaseAgent):
    def __init__(self, name, team, index):
        super().__init__(name, team, index)

        with open("src/brainfuck/bot.bf", "r") as f:
            source: str = f.read()

        ins_ret: LengthInstructionArray = get_instructions(ctypes.create_string_buffer(bytes(source, "ASCII")))
        self.instructions_len: ctypes.c_ushort = ins_ret.n
        self.instructions: ctypes.POINTER(Instruction) = ins_ret.array
        self.inputs: str = ""

    def get_output(self, packet: GameTickPacket) -> SimpleControllerState:
        packet_cells: List[int] = eval(self.inputs, {}, {"packet": packet, "self": self})
        packet_cells_ctypes = (ctypes.c_byte * len(packet_cells))(*packet_cells)

        code_ret: LengthCharArray = run_code(self.instructions, self.instructions_len,
                                             ctypes.c_char_p(ctypes.addressof(packet_cells_ctypes)), 6)
        outputs: ctypes.POINTER(ctypes.c_byte) = code_ret.array

        controller: SimpleControllerState = SimpleControllerState()
        controller.steer = outputs[0]
        controller.throttle = outputs[1]
        controller.pitch = outputs[2]
        controller.yaw = outputs[3]
        controller.roll = outputs[4]
        controller.jump = outputs[5]
        controller.boost = outputs[6]
        controller.handbrake = outputs[7]

        return controller

    def load_config(self, config_header: ConfigHeader) -> None:
        self.inputs = config_header["inputs"].value

    @staticmethod
    def create_agent_configurations(config: ConfigObject) -> None:
        config_header: ConfigHeader = config.get_header(BOT_CONFIG_AGENT_HEADER)
        config_header.add_value("inputs", str, default="[]",
                                description="The inputs that the bot should receive \
                                             whenever it gives the input command (,)")
