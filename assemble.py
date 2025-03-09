# THIS FILE TURNS THE program.asm FILE INTO EXECUTABLE BYTES

import sys
import struct

# MIPS instruction encodings (subset)
OPCODES = {
    'addi': 0x08, 'lw': 0x23, 'sw': 0x2B, 'beq': 0x04, 'bne': 0x05,
    'add': 0x00, 'sub': 0x00, 'and': 0x00, 'or': 0x00, 'slt': 0x00, 'j': 0x02
}

FUNCTS = {
    'add': 0x20, 'sub': 0x22, 'and': 0x24, 'or': 0x25, 'slt': 0x2A
}

REGISTERS = {f'${i}': i for i in range(32)}  # $0-$31
REGISTERS.update({f'${name}{i}': (i + 8 if name == 't' else i + 16)
               for name in ['t', 's'] for i in range(10)})

def parse_instruction(line, labels, current_addr):
    line = line.split('#')[0].strip()
    if not line:
        return None, None

    # Hhandle labels (e.g. "loop:")
    if ':' in line:
        label_part, instr_part = line.split(':', 1)
        label = label_part.strip()
        labels[label] = current_addr
        line = instr_part.strip()

    parts = line.replace(',', ' ').split()
    if not parts:
        return None, None

    mnemonic = parts[0].lower()

    # r type (add, sub, and, or, slt)
    if mnemonic in ['add', 'sub', 'and', 'or', 'slt']:
        rd, rs, rt = parts[1], parts[2], parts[3]
        return (
            (OPCODES[mnemonic] << 26) |
            (REGISTERS[rs] << 21) |
            (REGISTERS[rt] << 16) |
            (REGISTERS[rd] << 11) |
            FUNCTS[mnemonic]
        ), None

    # i type (addi, lw, sw, beq, bne)
    elif mnemonic in ['addi', 'lw', 'sw', 'beq', 'bne']:
        if mnemonic in ['lw', 'sw']:
            rt = parts[1]
            offset_rs = parts[2].split('(')
            imm = int(offset_rs[0])
            rs = offset_rs[1].strip(')')
        elif mnemonic in ['beq', 'bne']:
            # format: beq/bne rs, rt, label
            rs = parts[1]
            rt = parts[2]
            label = parts[3]
            # calculate offset (label_addr - pc_next) / 4
            pc_next = current_addr + 4
            imm = (labels[label] - pc_next) // 4 if label in labels else int(label)
        else:
            # addi rt, rs, imm
            rt = parts[1]
            rs = parts[2]
            imm = int(parts[3])

        return (
            (OPCODES[mnemonic] << 26) |
            (REGISTERS[rs] << 21) |
            (REGISTERS[rt] << 16) |
            (imm & 0xFFFF)
        ), None

    # j type 
    elif mnemonic == 'j':
        target = labels[parts[1]] if parts[1] in labels else int(parts[1])
        return (OPCODES['j'] << 26) | (target & 0x03FFFFFF), None

    else:
        raise ValueError(f"Unsupported instruction: {mnemonic}")

def assemble(input_file, output_file):
    with open(input_file, 'r') as f:
        lines = f.readlines()

    labels = {}
    current_addr = 0x00400000  # start of text segment

    # first pass: Collect labels
    for line in lines:
        line = line.split('#')[0].strip()
        if ':' in line:
            label_part = line.split(':', 1)[0].strip()
            labels[label_part] = current_addr
        elif line:
            current_addr += 4  # increment address for each valid instruction

    # second pass: generate binary
    current_addr = 0x00400000
    binary = []
    for line in lines:
        instr, _ = parse_instruction(line, labels, current_addr)
        if instr is not None:
            binary.append(instr)
            current_addr += 4

    with open(output_file, 'wb') as f:
        for instr in binary:
            f.write(struct.pack('>I', instr))  # big-endian

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.asm output.bin")
        sys.exit(1)
    assemble(sys.argv[1], sys.argv[2])