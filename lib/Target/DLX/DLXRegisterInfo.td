class GPReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "DLX";
}


foreach i = 0-28 in {
def R#i : GPReg<i, "r"#i >;}

def SP : GPReg<29, "fp">;
def FP : GPReg<30, "sp">;
def RA : GPReg<31, "ra">;


def GPRegs : RegisterClass<"DLX", [i32], 32,
(add R0, R1, R2, R3, R4, R5, R6, R7, R8,
R9, R10, R11, R12, R13, R14, R15, R16, R17, R18, R19,
R20, R21, R22, R23, R24, R25, R26, R27, R28)>;
