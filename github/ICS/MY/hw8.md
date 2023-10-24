# 9.16 

a. How many trap service routines can be implemented in the LC-3?
Why?
b. Why must a RTI instruction be used to return from a TRAP routine?
Why won’t a BR (Unconditional Branch) instruction work instead?
c. How many accesses to memory are made during the processing of a
TRAP instruction? Assume the TRAP is already in the IR.



a.256 service routines can be specified. Since TRAP vector has 8 bit ,2^8 = 256

b. The RTI instruction pops the top two values on the system stack into the PC and PSR. Since the PC contains the address
following the address of the TRAP instruction,control returns to the user program at the correct address. 

And BR may not has the range to resume PC ,and even if BR can resume PC by Label ,it can't resume PSR, which contain the previous State, it must be resumed but BR can't do that.

c. if PSR[15]==1 ,then need to switch Stack Pointer, but it don't need Mem

then PUSH temp and PC into system stack,it's 2 Mem access

then it will fetch Mem[trapvec]'s address of sub service routine. it's 1 Mem access

So it has 3 Mem access, push temp and PC into system Stack，and get Mem[trapvec] -> PC 

![image-20230722164907124](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230722164907124.png)

![image-20230722170138725](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230722170138725.png)

# 9.17 

Refer to Figure 9.14, the HALT service routine.
a. What starts the clock after the machine is HALTed? Hint: How can
the HALT service routine return after bit [15] of the Master Control
Register is cleared?
b. Which instruction actually halts the machine?
c. What is the first instruction executed when the machine is started
again?
d. Where will the RTI of the HALT routine return to?



a. clock is stopped .none of instruction can be use. So only to start it by external ways, To set  MCR[15]=1 and resume clock

b. STI R0,MCR ,it Stop the clock and halt the machine.

c. LD R1,SaveR1

d.to the PC where call HALT plus one. （PC+1,PC is where Call Halt）

![image-20230722171728966](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230722171728966.png)![image-20230722171738868](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230722171738868.png)