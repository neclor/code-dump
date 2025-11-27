using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace LittleComputer3;

internal class Lc3 {

	public enum OpCode : UInt16 {
		Add = 0b0001,
		And = 0b0101,
		Br = 0b0000,
		Jmp = 0b1100,
		Jsr = 0b0100,
		// Jsrr = 0b1100, Same as Jsr
		Ld = 0b0010,
		Ldi = 0b1010,
		Ldr = 0b0110,
		Lea = 0b1110,
		Not = 0b1001,
		// Ret = 0b1100, Same as Jmp
		Rti = 0b1000,
		St = 0b0011,
		Sti = 0b1011,
		Str = 0b0111,
		Trap = 0b1111,
		Unused = 0b1101,
	}

	public enum TrapCode : UInt16 {
		Getc = 0x20,
		Out = 0x21,
		Puts = 0x22,
		In = 0x23,
		Putsp = 0x24,
		Halt = 0x25,
	}

	public enum DeviceRegisterAddres : UInt16 {
		Kbsr = 0xFE00,
		Kbdr = 0xFE02,
		Dsr = 0xFE04,
		Ddr = 0xFE06,
		Mcr = 0xFFFE,
	}

	public enum ConditionFlag : UInt16 {
		P = 0b001,
		Z = 0b010,
		N = 0b100,
	};

	public const UInt16 MemorySize = UInt16.MaxValue;
	public const UInt16 RegisterCount = 8;
	public const UInt16 PcStart = 0x3000;

	public UInt16[] Memory { get; } = new UInt16[MemorySize];
	public UInt16[] Registers { get; } = new UInt16[RegisterCount];
	public UInt16 Pc { get; set; } = PcStart;
	public UInt16 Cc { get; set; } = 0;

	public void Run() {
		while (true) {
			Step();
		}
	}

	public void Step() {
		Execute(Memory[Pc]);
		Pc++;
	}

	public void Execute(UInt16 instruction) {
		OpCode opCode = (OpCode)ExtractBits(instruction, 4, 12);

		switch (opCode) {
			case OpCode.Add:
				Add(instruction);
				break;
			case OpCode.And:
				And(instruction);
				break;
			case OpCode.Br:
				Br(instruction);
				break;
			default:
				throw new NotImplementedException($"Opcode {opCode} not implemented.");
		}
	}

	public void SetCc(UInt16 value) {

		if (value == 0) {
			Cc = (UInt16)ConditionFlag.Z;
		} else if ((value >> 15) == 1) {
			Cc = (UInt16)ConditionFlag.N;
		} else {
			Cc = (UInt16)ConditionFlag.P;
		}


	}

	public void Add(UInt16 instruction) {
		UInt16 sr1 = ExtractR1(instruction);

		UInt16 b = 0;

		if (ExtractBits(instruction, 1, 5) == 0) {

			b = ExtractR2(instruction);

		} else {

		}


			Registers[ExtractR0(instruction)] = sr1 + ExtractBits(instruction, 1, 5) == 0 ? 1 : 1;


		let dr: usize = get_r0!(instr);
		let a: u16 = self.reg[get_r1!(instr)];
		let b: u16 = if get_imm5_flag!(instr) { sign_extend!(instr & 0x1F, 5)} else { self.reg[get_r2!(instr)]}
		;
		let value: u16 = a + b;

		self.reg[dr] = value;
		self.set_cc(value);





	}

	public void And(UInt16 instruction) {

		\


	private static UInt16 ExtractR0(UInt16 instruction) => ExtractBits(instruction, 3, 9);
	private static UInt16 ExtractR1(UInt16 instruction) => ExtractBits(instruction, 3, 6);
	private static UInt16 ExtractR2(UInt16 instruction) => ExtractBits(instruction, 3);

	private static UInt16 ExtractBits(UInt16 instruction, UInt16 count, UInt16 startBit = 0) => (UInt16)((instruction >> startBit) & ((1 << count) - 1));
}
