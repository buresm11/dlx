include "DLXInstrFormats.td"


def ADDrr  : FR <2, 0b000000,
         (outs GPRegs:$dst), (ins GPRegs:$src1, GPRegs:$src2),
         "add $dst, $src1, $src2",
        [(set i32:$dst, (add i32:$src1, i32:$src2))],IIC_ALU>;

def SUBrr  : FR <2, 0b000001,
         (outs GPRegs:$dst), (ins GPRegs:$src1, GPRegs:$src2),
         "sub $dst, $src1, $src2",
         [(set i32:$dst, (sub i32:$src1, i32:$src2))],IIC_ALU>;
