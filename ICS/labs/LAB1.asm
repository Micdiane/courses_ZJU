.ORIG x3000
AND R2,R2,#0
LDI R0,WORD
LD  R3,MASK
NOT R0,R0

LOOP 
AND R1,R0,R3
BRZ YES
ADD R3,R3,R3
BRP LOOP

AND R1,R0,R3
BRNP NO

YES ADD R2,R2,#1
    TRAP x25
    
NO  TRAP X25

WORD .FILL X3100
MASK .FILL X0007

.END

C000