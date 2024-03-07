3.53 3.61

4.1 4.3 4.8 4.10

# 3.53

![image-20230715102242636](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230715102242636.png)

first We take clk = 0 , after a time,and Master get input ,Slave maintain old value

then We take clk posedge 0->1,and master resist input ,Slave Get MASTER's value

then clk = 1 ,after a time ,and SLAVE'output get combine logic's computed, then wait Master'WE  to be 1

So We say this master slave flip flop is posedge,or say that ,only when 0->1 occurs, this change

|      | cycle0 | cycle1 | cycle2 | cycle3 | cycle4 | cycle5 | cycle6 | cycle7 |
| ---- | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
| D2   | 0      | 1      | 1      | 1      | 1      | 0      | 0      | 0      |
| D1   | 0      | 1      | 1      | 0      | 0      | 1      | 1      | 0      |
| D0   | 0      | 1      | 0      | 1      | 0      | 1      | 0      | 1      |

0 7 6 5 4 3 2 1 0 7 ...

3 bit asynchronous backward counter

# 3.61

| S1   | S0   | X    | Z    | S1'  | S0'  |
| ---- | ---- | ---- | ---- | ---- | ---- |
| 0    | 0    | 0    | 1    | 0    | 0    |
| 0    | 0    | 1    | 1    | 0    | 1    |
| 0    | 1    | 0    | 0    | 1    | 0    |
| 0    | 1    | 1    | 0    | 0    | 0    |
| 1    | 0    | 0    | 0    | 0    | 1    |
| 1    | 0    | 1    | 0    | 1    | 0    |
| 1    | 1    | 0    | 0    | 0    | 0    |
| 1    | 1    | 1    | 0    | 0    | 0    |

$Z=\bar S1 \bar S0$

$S1 = \bar S1 S0 \bar X + S1\bar S0X$

 $S0  =\bar S1 \bar S0 X  + S1 \bar S0 \bar X$

![image-20230715105952304](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230715105952304.png)

# 4.1

memory, a processing unit, input, output, and a control unit



**memory:** Store Data，Using MAR's address fetch data to MDR .The
information stored in the memory location is like the letters contained in the post
office box. As time goes by, what is contained in the post office box at any particular moment can change. But the box number remains the same. 

**a processing unit:** The actual processing of information in the computer is carried out by the
processing unit

 input, Receive information From outside

**output,** display or present information To outside

**a control unit:**The control unit is like the conductor of an orchestra; it is in charge of making
all the other parts of the computer play together

# 4.3

since the contents of this register is, in some sense, “pointing” to the next instruction to be processed. Curiously, Intel does in fact call that register the instruction pointer, but the simple elegance of that name has not caught on. 

program counter doesn't really count the total number of Instructions,instead,it's the instruction's address So it's misleading.

instruction pointer can fit the real function of PC that Containing the  instruction's address

# 4.8

 225 opcodes: 256=2^8,8bit

120 regs: 128=2^7,7bit

32-8-7-7-7 = 3,most 3bit

# 4.10

![image-20230715112538004](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230715112538004.png)

![image-20230715112528859](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230715112528859.png)

![image-20230715112523414](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230715112523414.png)

|      | Fetch inst       | Decode | Evaluate Addr | Fetch Data | Store Result |
| ---- | ---------------- | ------ | ------------- | ---------- | ------------ |
| PC   | 0001 ;0110 ;1100 |        |               |            | 1100         |
| IR   | 0001 ;0110 ;1100 |        |               |            |              |
| MAR  | 0001 ;0110 ;1100 |        |               | 0110       |              |
| MDR  | 0001 ;0110 ;1100 |        |               | 0110       |              |

ADD don't change PC or use MEM 

JMP Read BaseR then change PC 

LDR use baseR + offset to compute MEMaddr ,then use it to fetch data from MEM,pass it to DR