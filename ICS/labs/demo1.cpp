#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define bit(num,id) ((num >> id) % 2)
#define ADD 1
#define AND 5
#define BR  0
#define JMP 12
#define JSR 4
#define LD  2
#define LDI 10
#define LDR 6
#define LEA 14
#define NOT 9
#define RET 12
#define RTI 8
#define ST  3
#define STI 11
#define STR 7
#define TRAP 15
#define RESERVED 13
typedef unsigned short block;
FILE* fp;
// 指令操作码

// 指令掩码
enum Mask {
	DR_MASK = 0x0E00,
	SR1_MASK = 0x01C0,
	SR2_MASK = 0x0007,
	IMM5_MASK = 0x001F,
	OFFSET9_MASK = 0x01FF,
	OFFSET11_MASK = 0x07FF,
	OFFSET6_MASK = 0x003F,
	PSR_MASK = 0x8002
};

// 内存大小
#define MEM_SIZE 65024

// 内存和寄存器
block mem[MEM_SIZE];
block reg[8];

// 程序计数器、程序状态寄存器、指令寄存器、初始地址、条件码
block PC, PSR, IR, ORIG, CC;

// 函数声明
void boot();
block stob(char *p);
void readin(char *p, block ORIG);
void execute();
block get_masked(block num, block mask);
block get_dr(block num);
block get_sr1(block num);
block get_sr2(block num);
block get_imm5(block num);
block get_offset9(block num);
block get_offset11(block num);
block get_offset6(block num);
void setcc(int num);
void br();
void add();
void ld();
void st();
void jsr();
void and_();
void ldr();
void str();
void not_();
void ldi();
void sti();
void jmp();
void lea();

// 启动程序
void boot() {
	for (block i = 0; i < MEM_SIZE; i++)
		mem[i] = 0x7777;
	for (block i = 0; i < 8; i++)
		reg[i] = 0x7777;
	PC = ORIG;
	PSR = PSR_MASK;
	IR = 0x7777;
}

// 二进制字符串转换为十六进制数
block stob(char *p) {
	block sum = 0;
	for (block i = 0; i < 16; i++)
		sum += (p[i] - '0') * pow(2, 15 - i);
	return sum;
}

// 从标准输入读入指令到内存
void readin(char *p, block ORIG) {
	block i = ORIG;
	while (~scanf("%s", p))
	{
		mem[i++] = stob(p);
		
	}
}

// 获取目的寄存器
block get_dr(block num) {
	return (num & DR_MASK) >> 9;
}

// 获取源寄存器1
block get_sr1(block num) {
	return (num & SR1_MASK) >> 6;
}

// 获取源寄存器2
block get_sr2(block num) {
	return num & SR2_MASK;
}

// 获取立即数
block get_imm5(block num) {
	block imm5 = num & IMM5_MASK;
	if (imm5&0x10)
		imm5 |= 0xFFE0;
	return imm5;
}

// 获取偏移量9
block get_offset9(block num) {
	block offset9 = num & OFFSET9_MASK;
	if (bit(offset9,8))
		offset9 |= 0xFE00;
	return offset9;
}

// 获取偏移量11
block get_offset11(block num) {
	block offset11 = num & OFFSET11_MASK;
	if (bit(offset11,10))
		offset11 |= 0xF800;
	return offset11;
}

// 获取偏移量6
block get_offset6(block num) {
	block offset6 = num & OFFSET6_MASK;
	if (bit(offset6,5))
		offset6 |= 0xFFC0;
	return offset6;
}

// 设置条件码
void setcc() {
	int num = reg[get_dr(IR)];
	if (bit(num, 15))
	{
		PSR = 0x8004;
	}
	else if (num)
	{
		PSR = 0x8001;
	}
	else
	{
		PSR = 0x8002;
	}
}
// 模拟CPU执行
block BEN;
void execute() {
	
	while(1)
	{
		IR = mem[PC++];
		//		printf("%x\n",IR);
		int OP = IR >> 12;
		switch (OP) {
		case ADD:
			
			if(!bit(IR,5)) // ADD with reg
			{
				reg[get_dr(IR)] = reg[get_sr1(IR)] + reg[get_sr2(IR)];
			}	
			else{ // ADd with imm5
				reg[get_dr(IR)] = reg[get_sr1(IR)] +  get_imm5(IR);
			}
			setcc();
			break;
		case AND:
			
			if(!bit(IR,5)) // ADD with reg
			{
				reg[get_dr(IR)] = reg[get_sr1(IR)] & reg[get_sr2(IR)];
			}	
			else{ // ADd with imm5
				reg[get_dr(IR)] = reg[get_sr1(IR)] & get_imm5(IR);
			}
			setcc();
			break;
		case BR:
			
			BEN = (bit(IR, 11) & bit(PSR, 2)) + (bit(IR, 10) & bit(PSR, 1)) + (bit(IR, 9) & bit(PSR, 0));
			if(BEN)
				PC = PC + get_offset9(IR);
			break;
		case JMP:
			
			PC = reg[get_sr1(IR)];
			break;
		case JSR:
			
			if(bit(IR,11)){ // JSR
				
				reg[7] = PC;
				PC = PC + get_offset11(IR);
			}else{ // jsrr
				
				reg[7] = PC;
				PC = reg[get_sr1(IR)];
			}
			break;
		case LD:
			reg[get_dr(IR)] = mem[(block)(PC + get_offset9(IR))];
			setcc();
			break;
		case LDI:
			reg[get_dr(IR)] = mem[mem[(block)(PC + get_offset9(IR))]];
			setcc();
			break;
		case LDR:
			reg[get_dr(IR)] = mem[(block)(reg[get_sr1(IR)]+get_offset6(IR))];
			setcc();
			break;
		case LEA :
			reg[get_dr(IR)] = PC + get_offset9(IR);
			break;
		case NOT:
			reg[get_dr(IR)] = ~reg[get_sr1(IR)];
			setcc();
			break;
		case RTI:
			break;
		case ST:
			mem[(block)(PC + get_offset9(IR))] = reg[get_dr(IR)]; 
			break;
		case STI:
			mem[mem[(block)(PC + get_offset9(IR))]] = reg[get_dr(IR)]; 
			break;
		case STR:
			mem[(block)(reg[get_sr1(IR)]+get_offset6(IR))] = reg[get_dr(IR)] ;
			break;
		case TRAP:
			goto OUT;
			break;
		case RESERVED:
			break;
		}
	}
	OUT:
		return;
}

void check(){
	fp = fopen("in.txt","r");
	char p[17];
	fscanf(fp,"%s", p);
	ORIG = stob(p);
	
	boot();
	block i = ORIG;
	while (~fscanf(fp,"%s", p))
	{
		mem[i++] = stob(p);
		
	}
	execute();
	for (block i = 0; i < 8; i++)
	{
		printf("R%u = x%04hX\n", i, reg[i]);
	}
}
// 主函数
int main() {
	//	check();
	//	return 0;
	char p[17];
	scanf("%s", p);
	ORIG = stob(p);
	
	boot();
	readin(p, ORIG);
	execute();
	for (block i = 0; i < 8; i++)
	{
		printf("R%u = x%04hX\n", i, reg[i]);
	}
	return 0;
}
