#include "sim4.h"

WORD getInstruction(WORD curPC, WORD *instructionMemory);

void extract_instructionFields(WORD instruction, InstructionFields *fieldsOut){
    int shift;
    int mask;
    mask = instruction &  0x3F;         // funct mask shift and extract     (5 - 0) bits
    fieldsOut->funct = mask;

    shift =  instruction >> 6;          // shamt mask shift and extract     (10 - 6) bits
    mask = shift & 0x1F;
    fieldsOut->shamt = mask;

    shift =  instruction >> 11;         // rd mask shift and extract        (15 - 11) bits
    mask = shift & 0x1F;
    fieldsOut->rd = mask;

    mask = instruction & 0xFFFF;        // imm16 mask shift and extract     (15 - 0) bits
    fieldsOut->imm16 = mask;             
    fieldsOut->imm32 = signExtend16to32(mask);  // imm32 <- Sign Extend

    shift = instruction >> 16;          // rt mask shift and extract        (20 - 16) bits
    mask = shift & 0x1F;
    fieldsOut->rt = mask;

    shift = instruction >> 21;          // rs mask shift and extract        (25 - 21) bits
    mask = shift & 0x1F;
    fieldsOut->rs = mask;
    
    mask = instruction & 0x3FFFFFF;     // address mask shift and extract   (25 - 0) bits
    fieldsOut->address = mask;
    
    shift = instruction >> 26;          // opcode mask shift and extract    (31 - 26) bits
    mask = shift & 0x3F;
    fieldsOut->opcode = mask;


}

int  fill_CPUControl(InstructionFields *fields, CPUControl *controlOut){
    // printf("%d",fields->funct, fields->opcode);
    if (fields->opcode == 0){                   // If opcode == 0:
        controlOut->ALUsrc = 0;
        controlOut->regDst = 1;
        controlOut->regWrite = 1;
        controlOut->memToReg = 0;
        controlOut->memWrite = 0;
        controlOut->memRead = 0;
        controlOut->branch = 0;
        controlOut->jump = 0;
        if(fields->funct == 32){                // if funct = 32 then add
            (*controlOut).ALU.op = 2;
            (*controlOut).ALU.bNegate = 0;
        }
        else if(fields->funct == 33){           // if funct = 33 then addu
            (*controlOut).ALU.op = 2;
            (*controlOut).ALU.bNegate = 0;
        }
        else if(fields->funct == 34){           // elseif funct = 34 then sub
            (*controlOut).ALU.op = 2;
            (*controlOut).ALU.bNegate = 1;
        }
        else if(fields->funct == 35){           // elseif funct = 35 then subu
            (*controlOut).ALU.op = 2;
            (*controlOut).ALU.bNegate = 1;
        }
        else if(fields->funct == 36){           // elseif funct = 36 then and
            (*controlOut).ALU.op = 0;
            (*controlOut).ALU.bNegate = 0;
        }
        else if(fields->funct == 37){           // elseif funct = 37 then or
            (*controlOut).ALU.op = 1;
            (*controlOut).ALU.bNegate = 0;
        }
        else if(fields->funct == 38){           // elseif funct = 38 then xor
            (*controlOut).ALU.op = 4;
            (*controlOut).ALU.bNegate = 0;
        }
        else if(fields->funct == 42){           // elseif funct = 42 then slt
            (*controlOut).ALU.op = 3;
            (*controlOut).ALU.bNegate = 1;
        }
        else{
            return 0;
        }
    }
    else if(fields->opcode == 2){               // else if opcode == 2 then jump
        controlOut->ALUsrc = 0;
        controlOut->regDst = 0;
        controlOut->regWrite = 0;
        controlOut->memToReg = 0;
        controlOut->memWrite = 0;
        controlOut->memRead = 0;
        controlOut->branch = 0;
        controlOut->jump = 1;
        (*controlOut).ALU.op = 2;
        (*controlOut).ALU.bNegate = 0;
    }
    else if(fields->opcode == 4){               // else if opcode == 4 then beq
        controlOut->ALUsrc = 0;
        controlOut->regDst = 0;
        controlOut->regWrite = 0;
        controlOut->memToReg = 0;
        controlOut->memWrite = 0;
        controlOut->memRead = 0;
        controlOut->branch = 1;
        controlOut->jump = 0;
        (*controlOut).ALU.op = 1;
        (*controlOut).ALU.bNegate = 0;
    }
    else if(fields->opcode == 8){               // else if opcode == 8 then addi
        controlOut->ALUsrc = 1;
        controlOut->regDst = 0;
        controlOut->regWrite = 1;
        controlOut->memToReg = 0;
        controlOut->memWrite = 0;
        controlOut->memRead = 0;
        controlOut->branch = 0;
        controlOut->jump = 0;
        (*controlOut).ALU.op = 2;
        (*controlOut).ALU.bNegate = 0;
    }
    else if(fields->opcode == 9){               // else if opcode == 9 then addiu
        controlOut->ALUsrc = 1;
        controlOut->regDst = 0;
        controlOut->regWrite = 1;
        controlOut->memToReg = 0;
        controlOut->memWrite = 0;
        controlOut->memRead = 0;
        controlOut->branch = 0;
        controlOut->jump = 0;
        (*controlOut).ALU.op = 2;
        (*controlOut).ALU.bNegate = 0;
    }
    else if(fields->opcode == 10){               // else if opcode == 10 then slti
        controlOut->ALUsrc = 1;
        controlOut->regDst = 0;
        controlOut->regWrite = 1;
        controlOut->memToReg = 0;
        controlOut->memWrite = 0;
        controlOut->memRead = 0;
        controlOut->branch = 0;
        controlOut->jump = 0;
        (*controlOut).ALU.op = 3;
        (*controlOut).ALU.bNegate = 1;
    }
    else if(fields->opcode == 35){               // else if opcode == 35 then lw
        controlOut->ALUsrc = 1;
        controlOut->regDst = 0;
        controlOut->regWrite = 1;
        controlOut->memToReg = 1;
        controlOut->memWrite = 0;
        controlOut->memRead = 1;
        controlOut->branch = 0;
        controlOut->jump = 0;
        (*controlOut).ALU.op = 0;
        (*controlOut).ALU.bNegate = 0;
    }
    else if(fields->opcode == 43){               // else if opcode == 43 then sw
        controlOut->ALUsrc = 1;
        controlOut->regDst = 0;
        controlOut->regWrite = 0;
        controlOut->memToReg = 0;
        controlOut->memWrite = 1;
        controlOut->memRead = 0;
        controlOut->branch = 0;
        controlOut->jump = 0;
        (*controlOut).ALU.op = 0;
        (*controlOut).ALU.bNegate = 0;
    }
    else{
        return 0;
    }
    return 1;
}

WORD getALUinput1(CPUControl *controlIn,
                  InstructionFields *fieldsIn,
                  WORD rsVal, WORD rtVal, WORD reg32, WORD reg33,
                  WORD oldPC);

WORD getALUinput2(CPUControl *controlIn,
                  InstructionFields *fieldsIn,
                  WORD rsVal, WORD rtVal, WORD reg32, WORD reg33,
                  WORD oldPC);

void execute_ALU(CPUControl *controlIn,
                 WORD input1, WORD input2,
                 ALUResult  *aluResultOut);
void execute_MEM(CPUControl *controlIn,
                 ALUResult  *aluResultIn,
                 WORD        rsVal, WORD rtVal,
                 WORD       *memory,
                 MemResult  *resultOut);

WORD getNextPC(InstructionFields *fields, CPUControl *controlIn, int aluZero,
               WORD rsVal, WORD rtVal,
               WORD oldPC);

void execute_updateRegs(InstructionFields *fields, CPUControl *controlIn,
                        ALUResult  *aluResultIn, MemResult *memResultIn,
                        WORD       *regs);
