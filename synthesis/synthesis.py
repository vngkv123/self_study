import z3
import sys, time
import ctypes
import capstone

# For Binary Disassemble
# Generate x86 code
# Instruction Parsing?
class Opcode(object):
    def __init__(self):
        self.prefix = None
        self.mnem = None
        self.operand1 = None
        self.operand2 = None
        self.operand3 = None


class BinAnalyzer(object):
    def __init__(self):
        pass



# Generate BasicBlocks and make CFG
class Differ(object):
    def __init__(self):
        pass

    def CodeGen(self):
        pass

    def Analyze(self):
        pass


# Do we really need IR Synthesize?
# Do we really need Hypotheses?
class IntermediateRepresentation(object):
    def __init__(self):
        pass

    def x86_to_IR(self):
        pass

    def SSAGen(self):
        pass


# For Verification Purpose
# For Emulation?
class VM(object):
    def __init__(self):
        self.context_before = {}
        self.context_after = {}
        self.page = None
        self.greg = []
        self.sreg = []
        self.freg = []
        self.memory = None

    def SetRWX(self, size)
        ptr = ctypes.pythonapi.valloc(size)
        ctypes.pythonapi.mprotect(ptr, size, 1|2|4) # 1|2|4 is read|write|execute
        self.page = ptr

    # For IR Semantic Verification
    # Can we set Signal Handler for this?
    # IDEA -> Do Fuzz in here, and get Result from VM
    def JIT(self):
        pass


def main():
    print("[-] Hello :)")

if __name__ == "__main__":
    main()

