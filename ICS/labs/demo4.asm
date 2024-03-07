;;initialize interrupt
.ORIG x0200
LD R6,OS_SP     ;Get OS_Sp
LD R0,USER_PSR  ;Get UserRsr
ADD R6,R6,#-1
STR R0,R6,#0    ;PUSH tem PSR to OS stack
LD R0,USER_PC   ;Get tem PC
ADD R6,R6,#-1
STR R0,R6,#0    ;PUSH tem PC to OSstack
LD R1,EN
LD R2,KBSR
STR R1,R2,#0    ;enable the keyboard interrupt
LD R1,FLAPPY        
LD R2,TRAP_FLAPPY   ;Store interrupt routine start address
STR R1,R2,#0        
RTI
TRAP_FLAPPY .FILL x0180 ;keyboard interrupt start
EN .FILL x4000
FLAPPY .FILL x2000      ;interrupt routine
KBSR .FILL xFE00     
OS_SP .FILL x3000       
USER_PSR .FILL x8002    
USER_PC .FILL x3000     
.END

;interrupt routine Check input 
;if input is a-z then change KBDR
.ORIG x2000
ADD R6,R6,#-1
STR R3,R6,#0
ADD R6,R6,#-1
STR R4,R6,#0

LEA R0,STRING
; PUTS
LDI R0,KBDR
; AND R4,R4,#0
; LD  R3,KBSR1
; STR R4,R3,#0
LD R3,NEG_A
ADD R3,R0,R3
BRn CHECKDIGIT
LD R3,NEG_Z
ADD R3,R0,R3
BRp FINISH
ADD R1,R0,#0
BRnzp FINISH

CHECKDIGIT
    LD  R3,NEG_0
    ADD R0,R0,R3    ;op-'0'
    BRn FINISH
    ADD R0,R0,#-9
    BRp FINISH   ;if (op>='0' && op <='9')
    ADD R0,R0,#10 ; 
    ADD R2,R0,R2 ; y += op - '0'
    LD  R4,NEG_SEVENTEEN;TODO{y= (y-17>0?17:y);}
    ADD R4,R2,R4 ; R4 = y-17 
    BRp CEILy
    BR  FINISH
    CEILy 
    LD R2,SEVENTEEN ; y = 17
    BR  FINISH

FINISH
LDR R4,R6,#0
ADD R6,R6,#1
LDR R3,R6,#0
ADD R6,R6,#1

RTI
STRING .STRINGZ "GET"
NEG_A .FILL xFF9F
NEG_Z .FILL xFF86
NEG_0 .FILL xFFD0
KBDR  .FILL xFE02
NEG_SEVENTEEN .FILL xFFEF
SEVENTEEN .FILL x0012
KBSR1 .FILL xFE00  
.END


;;main program
.ORIG x3000
LD R1,A       ;x=a
AND R2,R2,#0
ADD R2,R2,#5        ;y=5

WHILE
   LD R0,DOT  ;R0 = '.'
   LD R3,TOTAL      ;R3(cnt) = 17
   ADD R4,R2,#0     ;R4(temy) = y
   LOOP1
        BRZ LOOP1OUT   
        OUT          ;PUTCHAR('.')
        ADD R3,R3,#-1;CNT  --
        ADD R4,R4,#-1;TEMY --
        BR LOOP1
   LOOP1OUT
   
   AND R4,R4,#0;
   ADD R0,R1,#0;
   ADD R4,R4,#3;
   LOOP2
        BRZ LOOP2OUT   
        OUT          ;PUTCHAR(R1)
        ADD R4,R4,#-1; 3 times
        BR LOOP2
    LOOP2OUT
    
    LD  R0,DOT
    ADD R3,R3,#0;   ;set CC
    LOOP3
        BRZ LOOP3OUT
        OUT
        ADD R3,R3,#-1;
        BR LOOP3
    LOOP3OUT
    LD R0,NEWLINE
    OUT             ;printf('\n')
    JSR DELAY       ;Sleep
    ADD R2,R2,#0    
    BRnz NOSUB
    ADD R2,R2,#-1   ;y--
    NOSUB
    BR WHILE
BR DONE

DONE HALT

DELAY 
ST R5, SAVER5
LD R5, COUNT
LOOP ADD R5, R5, #-1
BRnp LOOP
LD R5,SAVER5
RET
SAVER5 .BLKW #1

DOT .FILL x002E
A .FILL x0061
NEWLINE .FILL x000A
TOTAL .FILL x0011
SaveR0 .FILL x0000
COUNT .FILL x4000
.END