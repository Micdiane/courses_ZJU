3rd Edition

1.2 1.4 1.10 1.16 1.18

2.4 2.8 2.17 2.20 2.34



# 1.2

 Can a higher-level programming language instruct a computer to
compute more than a lower-level programming language?

answer:

No, because all computer languages ,no matter higher-level or lower-level , all will be finally converted to the same language - the machine code . And the different may be the code's length or the implementation's differ. But There NO difference in functional uses, or say the things that lower-programming language can Compute is the same as that of higher-level programming language.





# 1.4

Name one characteristic of natural languages that prevents them from
being used as programming languages

answer:

Ambiguity, that is, there may be multiple ways to understand the same sentence, which cannot be run on a deterministic machine.

for example :

Lucy insisted on a talk with the manager herself;

1.It is the **manager herself** that Lucy wants to talk to

2.It is the **Lucy herself** that wants to talk to the manager 



# 1.10

 Name three characteristics of algorithms. Briefly explain each of
these three characteristics.

answer:

 **definiteness** to describe the notion that each step is precisely
stated.

 **effective computability** to describe the notion that each step
canbecarriedoutbyacomputer..

**finiteness** to describe the notion that the procedure termi-
nates.

# 1.16

 Name at least three things specified by an ISA:

answer:

1.**Instruction Set**: The ISA defines the set of insructions that a computer can execute.

2.**Data Types**: The ISA defines the data types that the computer can operate on

3.**Addressing Modes**:The ISA specifies the addresssing modes that the computer supports calculates the memory address of operands.

# 1.18

How many ISAs are normally implemented by a single
microarchitecture? Conversely, how many microarchitectures could exist
for a single ISA?

answer:

a single microarchitecture is typically designed to implement **a specific ISA**,

**multiple** microarchitectures can exist for a single ISA



# 2.4 

Given n bits, how many unsigned integers can be represented with the n
bits? What is the range of these integers?

answer:

for each bit it can be 0 or 1,and that's for n bit ,so we can have $2^n$ different unsigned integers.

and We count the from zero ,so the range is $0\ to\ 2^n -1$



# 2.8 

a. What is the largest positive number one can represent in an eight-bit
2’s complement code? Write your result in binary and decimal.
b. What is the greatest magnitude negative number one can represent in
an eight-bit 2’s complement code? Write your result in binary and
decimal.
c. What is the largest positive number one can represent in n-bit 2’s
complement code?
d. What is the greatest magnitude negative number one can represent in
n-bit 2’s complement code?

answer:

a: 

Binary :	0111 1111 

Decimal:	+127

b:

Binary:	1000 0000

Decimal: -128

c:

Decimal: 	$2^{n-1} -1$

First bit should be zero and the following n-1 bits are all one.

d:

Decimal:  $-2^{n-1}$

First bit should be one and the following n-1 bits are all zero.

# 2.17 

Add the following 2’s complement binary numbers. Also express the
answer in decimal.
a. 01 + 1011
b. 11 + 01010101
c. 0101 + 110
d. 01 + 10



answer:

a:

0001

1011

**1100**

Decimal expression: 1-5=-4

b:

11111111

01010101

**01010100** 

Decimal expression: -1+85=84

c:

0101

1110

**0011**

Decimal expression:5-2=3

d:

01

10

**11**

Decimal expression:1-2=-1

# 2.20 

The following binary numbers are four-bit 2’s complement binary
numbers. Which of the following operations generate overflow? Justify
your answer by translating the operands and results into decimal.
a. 1100 + 0011 d. 1000 − 0001
b. 1100 + 0100 e. 0111 + 1001
c. 0111 + 0001

answer:

a:

1100

0011

**1111**

No overflow 

Decimal expression: -4+3=-1

b:

1100

0100

**0000**

NO overflow

Decimal expression:-4+4=0 

c:

0111

0001

**1000**

has overflow 

Decimal expression:7+1=8  which is out of range (for 4 bits 2's complements code the largest integer is 7)but it is -8

d:

1000-0001

=

1000+1111

1000

1111

**0111**

has overflow

Decimal expression:-8-1=-9 which is out of range (for 4 bits 2's complements code the largest interger is -8)but it is 7

e:

0111 

1001

**0000**

No overflow

Decimal expression:7-7=0





# 2.34

Compute the following: 

a. NOT(1011) OR NOT(1100) 

b. NOT(1000 AND (1100 OR 0101))

c. NOT(NOT(1101)) 

d. (0110 OR 0000) AND 1111



answer:

a:

NOT(1011) = 0100

NOT(1100) = 0011

0100 OR 

0011=

**0111**

b:

1100 OR 

0101=

1101



1000 AND 

1101=

1000



NOT(1000)=

**0111**



c:

NOT(NOT(1101)) = 1101

double NOT is just itself

NOT(1101) = 0010

NOT(0010) = **1101**



d:

0110 OR 

0000 =

0110



0110 AND 

1111 =

**0110**















