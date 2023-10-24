5.37 change LDI to STI

5.39

6.24

7.32 7.34

optional: 5.58 6.26 (no extra point) （选做，不计分）

# 5.37

Using the overall data path in Figure 5.18, identify the elements that
implement the STI instruction of Figure 5.8.

Instruction Fetch and  Decode,then PC + SEXT imm[8:0] is computed then chosen by MARMUX ,next the Address will be sent to MAR , MEM fetch Data to MDR,result that the MEM fetch will be sent to MAR as the address to place DR's. So DR's data will pass SR1OUT ALUK=passA  Gate ALu then to MDR.Finally , DR's data written in now MAR's address ,Condition Code will be set.STI finish

PC,MAR,MEM,MDR,ID,SEXT,ADDR1MUX ADDR2MUX,adder of them ,MARMUX,REGFILE ,ALU, LOFIC NZP ,FINITE STATE MACHINE ,Gate ALU  Gate MARMUX GateMDR,Gate PC 

![image-20230718110145661](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230718110145661.png)

![image-20230718110505117](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230718110505117.png)

![image-20230718110903417](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230718110903417.png)

![image-20230718110937296](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230718110937296.png)

# 5.39

Using the overall data path in Figure 5.18, identify the elements that
implement the LEA instruction of Figure 5.6.

Inst Fetch and then Decode ,PC calculate ,write New PC in Reg,Finish

PC,IR,SEXT[8:0] ADRR1MUX ADDR2MUX the adder of them ,MARMUX,REG FILE,GateMARMUX,Gate PC 

![image-20230718111104989](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230718111104989.png)



# 6.24

![image-20230718112243179](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230718112243179.png)

LDR DR,BaseR,offset

R0 was changed after that,and R0 = MEM[x4201]

x4201 = 4200 + 1 

So it use R1 as the baseR with offset 1

LDR R0,R1,#1

0110 000 001 000001

# 7.32

Symbol table

| symbol | Address |
| ------ | ------- |
| SKIP   | x8009   |
| A      | x800A   |
| B      | x8011   |
| BANNER | x801E   |
| C      | x801F   |

x8006	0010 0010 0000 0011

x8007	0000 0100 0000 0001

x8008	0011 0000 0000 1000



B .FILL #5 just store the value 5 in MEM before Program execute 

and  ST R0,B is a instruction , should after its execution then B is changed



# 7.34

![image-20230718114803848](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230718114803848.png)

while(A-B!=) A++ B--;



a.NOT R2,R0

b.ADD R2,R2,#1

c.BRz ,DONE

d.ADD R0,R0,#1

![image-20230718120300845](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230718120300845.png)









