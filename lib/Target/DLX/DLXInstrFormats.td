class Format<bits<4> val> {
  bits<4> Value = val;
}

def Pseudo    : Format<0>;
def FrmR      : Format<1>;
def FrmI      : Format<2>;
def FrmJ      : Format<3>;
def FrmFR     : Format<4>;
def FrmFI     : Format<5>;
def FrmOther  : Format<6>; // Instruction w/ a custom format

class DLXInst<dag outs, dag ins, string asmstr, list<dag> pattern,
               InstrItinClass itin, Format f>: Instruction
{
  field bits<32> Inst;
  Format Form = f;

  let Namespace = "DLX";

  let Size = 4;

  bits<6> Opcode = 0;

  // Top 6 bits are the 'opcode' field
  let Inst{31-26} = Opcode;

  let OutOperandList = outs;
  let InOperandList  = ins;

  let AsmString   = asmstr;
  let Pattern     = pattern;
  let Itinerary   = itin;

  //
  // Attributes specific to DLX instructions...
  //
  bits<4> FormBits     = Form.Value;
  bit isCTI            = 0; // Any form of Control Transfer Instruction.
                            // Required for DLXR6
  bit hasForbiddenSlot = 0; // Instruction has a forbidden slot.
  bit IsPCRelativeLoad = 0; // Load instruction with implicit source register
                            // ($pc) and with explicit offset and destination
                            // register
  bit hasFCCRegOperand = 0; // Instruction uses $fcc<X> register and is
                            // present in DLX-I to DLX-III.

  // TSFlags layout should be kept in sync with MCTargetDesc/DLXBaseInfo.h.
  let TSFlags{3-0}   = FormBits;
  let TSFlags{4}     = isCTI;
  let TSFlags{5}     = hasForbiddenSlot;
  let TSFlags{6}     = IsPCRelativeLoad;
  let TSFlags{7}     = hasFCCRegOperand;

  let DecoderNamespace = "DLX";

  field bits<32> SoftFail = 0;
}

// DLX32/64 Instruction Format
class InstSE<dag outs, dag ins, string asmstr, list<dag> pattern,
             InstrItinClass itin, Format f, string opstr = ""> :
  DLXInst<outs, ins, asmstr, pattern, itin, f> {
  string BaseOpcode = opstr;
  string Arch;
}

// DLX Pseudo Instructions Format
class DLXPseudo<dag outs, dag ins, list<dag> pattern,
                 InstrItinClass itin = IIPseudo> :
  DLXInst<outs, ins, "", pattern, itin, Pseudo> {
  let isCodeGenOnly = 1;
  let isPseudo = 1;
}

// DLX32/64 Pseudo Instruction Format
class PseudoSE<dag outs, dag ins, list<dag> pattern,
               InstrItinClass itin = IIPseudo> :
  DLXPseudo<outs, ins, pattern, itin> {
}

// Pseudo-instructions for alternate assembly syntax (never used by codegen).
// These are aliases that require C++ handling to convert to the target
// instruction, while InstAliases can be handled directly by tblgen.
class DLXAsmPseudoInst<dag outs, dag ins, string asmstr>:
  DLXInst<outs, ins, asmstr, [], IIPseudo, Pseudo> {
  let isPseudo = 1;
  let Pattern = [];
}

class StdArch {
  string Arch = "se";
}

//===----------------------------------------------------------------------===//
// Format R instruction class in DLX : <|opcode|rs|rt|rd|shamt|funct|>
//===----------------------------------------------------------------------===//

class FR<bits<6> op, bits<6> _funct, dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
  InstSE<outs, ins, asmstr, pattern, itin, FrmR>
{
  bits<5>  rd;
  bits<5>  rs;
  bits<5>  rt;
  bits<5>  shamt;
  bits<6>  funct;

  let Opcode = op;
  let funct  = _funct;

  let Inst{25-21} = rs;
  let Inst{20-16} = rt;
  let Inst{15-11} = rd;
  let Inst{10-6}  = shamt;
  let Inst{5-0}   = funct;
}

//===----------------------------------------------------------------------===//
// Format I instruction class in DLX : <|opcode|rs|rt|immediate|>
//===----------------------------------------------------------------------===//

class FI<bits<6> op, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin>: InstSE<outs, ins, asmstr, pattern, itin, FrmI>
{
  bits<5>  rt;
  bits<5>  rs;
  bits<16> imm16;

  let Opcode = op;

  let Inst{25-21} = rs;
  let Inst{20-16} = rt;
  let Inst{15-0}  = imm16;
}

class BranchBase<bits<6> op, dag outs, dag ins, string asmstr,
                  list<dag> pattern, InstrItinClass itin>:
  InstSE<outs, ins, asmstr, pattern, itin, FrmI>
{
  bits<5>  rs;
  bits<5>  rt;
  bits<16> imm16;

  let Opcode = op;

  let Inst{25-21} = rs;
  let Inst{20-16} = rt;
  let Inst{15-0}  = imm16;
}

//===----------------------------------------------------------------------===//
// Format J instruction class in DLX : <|opcode|address|>
//===----------------------------------------------------------------------===//

class FJ<bits<6> op> : StdArch
{
  bits<26> target;

  bits<32> Inst;

  let Inst{31-26} = op;
  let Inst{25-0}  = target;
}
