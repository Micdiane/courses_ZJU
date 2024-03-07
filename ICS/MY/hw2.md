2.40 2.48

3.6 3.20 3.30 3.36 3.40 3.50

# 2.40

Write the decimal equivalents for these IEEE floating point
numbers.
a. 0 10000000 00000000000000000000000
b. 1 10000011 00010000000000000000000
c. 0 11111111 00000000000000000000000
d. 1 10000000 10010000000000000000000

answer:

a：+ 1.0* 2^1 = 2

b:  - 1.0001*2^4 = -17

c: Positive Infinity because all exponent bit is one and all fractions is zero

d: - 1.1001 *2^1 = -3.125

# 2.48

Convert the following decimal numbers to hexadecimal representations
of 2’s complement numbers.
a. 256
b. 111
c. 123,456,789
d. −44

answer:

a: 

binary: 0001 0000 0000

**0x100**

b: 

binary:0110 1111

**0x6F**

c: 

binary:0111 0101 1011 1100 1101 0001 0101

**0x75BCD15**

d:

binary of 44 :0010 1100

binary of -44:1101 0100

**0xD4**

# 3.6

![image-20230713184805390](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230713184805390.png)

| A    | B    | C    | D    | Z    |
| ---- | ---- | ---- | ---- | ---- |
| 0    | 0    | 1    | 1    | 0    |
| 0    | 1    | 1    | 0    | 0    |
| 1    | 0    | 0    | 1    | 0    |
| 1    | 1    | 0    | 0    | 1    |

C = $\overline{A}$

D = $\overline{B}$

Z = $\overline{C+D}=\overline{\bar{A}+\bar{B}}=AB$



# 3.20

How many output lines will a 16-input multiplexer have? How many
select lines will this multiplexer have?

output line: 1

select lines: $log_216=4$

# 3.30

a. Figure 3.42 shows a logic circuit that appears in many of today’s
processors. Each of the boxes is a full-adder circuit. What does the
value on the wire X do? That is, what is the difference in the output
of this circuit if X = 0 vs. if X = 1?
b. Construct a logic diagram that implements an adder/subtractor. That
is, the logic circuit will compute A + B or A − B depending on
the value of X. Hint: Use the logic diagram of Figure 3.42 as a
building block.

![image-20230713190053464](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230713190053464.png)

a:

X is the select line of each 2to1 mux,it determines A + B or A +C

when X = 0  , then B3 B2 B1 B0 are chosen to be added

when X = 1 , then  C3 C2 C1 C0 are chosen to be added

![image-20230713194232223](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230713194232223.png)

When X=0 then choose Bi ，and carry = 0 ,it is A+B 

When X=1 then choose $\bar{B_i}$, and carry = 1,means A + $\bar{B}$ +1 = A-B

# 3.36

A comparator circuit has two 1-bit inputs A and B and three 1-bit outputs
G (greater), E (Equal), and L (less than). Refer to Figures 3.43 and 3.44
for this problem.
G is 1 if A > B E is 1 if A = B L is 1 if A < B
0 otherwise 0 otherwise 0 otherwise

![image-20230713192947133](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230713192947133.png)

![image-20230713193000143](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230713193000143.png)

a:

| A    | B    | G    | E    | L    |
| ---- | ---- | ---- | ---- | ---- |
| 0    | 0    | 0    | 1    | 0    |
| 0    | 1    | 0    | 0    | 1    |
| 1    | 0    | 1    | 0    | 0    |
| 1    | 1    | 0    | 1    | 0    |

b:

for AB ,

G = 10 

E = 00 + 11

L = 01

![image-20230714103621566](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230714103621566.png)

C：

![image-20230714111140019](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230714111140019.png)

when all pair{Ai,Bi} is equal ,the output EQUAL will be 1 

# 3.40

For the memory shown in Figure 3.45:
a. What is the address space?
b. What is the addressability?
c. What is the data at address 2?

![image-20230713193145969](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230713193145969.png)

address space : 2 address line ,then 2^2 = 4 

addressability: 4, since it is D[3:0] 

data of address 2 , We find A=10 and the D = 0001

# 3.50

Prove that the NAND gate, by itself, is logically complete (see
Section 3.3.5) by constructing a logic circuit that performs the AND
function, a logic circuit that performs the NOT function, and a logic
circuit that performs the OR function. Use only NAND gates in these
three logic circuits.

NOT ： Using NAND(X,X)

![image-20230714112636627](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230714112636627.png)

AND  : Using NOT NAND(X,Y) = NAND[NAND(X,Y) , NAND(X,Y)]

![image-20230714112624908](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230714112624908.png)

OR : Using Demorgan's law，it is NOT [AND(NOT A , NOT B)]

$\overline{AB}=\bar A+\bar B $

$\overline{\bar A \bar B}=A+B $

![image-20230714112822829](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230714112822829.png)

