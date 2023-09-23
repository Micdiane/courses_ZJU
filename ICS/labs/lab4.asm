;;initialize interrupt
.ORIG x0200
LD R6,OS_SP     ;Get OS_Sp
LD R0,USER_PSR  ;Get UserRsr
ADD R6,R6,#-1
STR R0,R6,#0    ;Store UserPsr First
LD R0,USER_PC   ;Get tem PC
ADD R6,R6,#-1
STR R0,R6,#0    ;PUSH tem PC
LD R1,TRAP_ENABLE   
LD R2,OS_KBSR       
STR R1,R2,#0    ;TRAP_ENABLE->KBSR
LD R1,FLAPPY        
LD R2,TRAP_FLAPPY   ;Store interrupt routine start address
STR R1,R2,#0        
RTI
TRAP_FLAPPY .FILL x0180 ;keyboard interrupt start
FLAPPY .FILL x2000      ;interrupt routine
TRAP_ENABLE .FILL x4000
OS_KBSR .FILL xFE00     
OS_SP .FILL x3000       
USER_PSR .FILL x8002    
USER_PC .FILL x3000     
.END

;interrupt routine Check input 
;if input is a-z then change KBDR
.ORIG x2000
ADD R6,R6,#-1
STR R0,R6,#0
ADD R6,R6,#-1
STR R1,R6,#0
ADD R6,R6,#-1
STR R2,R6,#0
ADD R6,R6,#-1
STR R3,R6,#0
ADD R6,R6,#-1
STR R4,R6,#0

LDI R0,OS_KBDR
LD R2,NEG_A     
ADD R2,R0,R2    
BRn NO_CHAR 
LD R2,NEG_Z
ADD R2,R0,R2
BRp NO_DIGIT    ;Check 'a'<=R2<='z'
ADD R1,R0,#0    ;R1 = KBDR
STR R1,R6,#0    ;R1->R6
BR  NO_DIGIT    ;Return

NO_CHAR LD R2,NEG_0
        ADD R0,R0,R2
        BRn NO_DIGIT
        ADD R0,R0,#-9
        BRp NO_DIGIT    ;check '0'<=R0<='9'
        ADD R6,R6,#1
        LDR R2,R6,#0
        BRz ZERO
        ADD R0,R0,#1
        ZERO ADD R0,R0,#9
        ADD R0,R0,R2
        LD R1,NEG_MAX_HEIGHT
        ADD R1,R0,R1
        BRn NORMAL
        LD R2,MAX_HEIGHT
        STR R2,R6,#0
        ADD R6,R6,#-1
        BRnzp NO_DIGIT
        NORMAL ADD R2,R0,#0
               STR R2,R6,#0
               ADD R6,R6,#-1
NO_DIGIT LDR R4,R6,#0
         ADD R6,R6,#1
         LDR R3,R6,#0
         ADD R6,R6,#1
         LDR R2,R6,#0
         ADD R6,R6,#1
         LDR R1,R6,#0
         ADD R6,R6,#1
         LDR R0,R6,#0
         ADD R6,R6,#1
RTI
NEG_A .FILL xFF9F       ;-'a'
NEG_Z .FILL xFF86       ;-'z'
NEG_0 .FILL xFFD0       ;-'0'
OS_KBDR .FILL xFE02
NEG_MAX_HEIGHT .FILL xFFEF
MAX_HEIGHT .FILL x0011
.END

;;main program
.ORIG x3000
LD R1,ASCII_A       ;x=a
AND R2,R2,#0
ADD R2,R2,#5        ;y=5

L2 LD R3,ASCII_DOT  ;R3 = '.'
   ADD R4,R2,#0     ;
   JSR PRINT

   ADD R3,R1,#0
   AND R4,R4,#0
   ADD R4,R4,#3
   JSR PRINT

   LD R3,TOTAL
   ADD R4,R2,#0
   ADD R4,R4,#3
   NOT R4,R4
   ADD R4,R4,#1
   ADD R4,R4,R3
   LD R3,ASCII_DOT
   JSR PRINT
   
   JSR SHIFT
   BRnzp L2

BRnzp DONE

PRINT ST R0,SaveR0
      ADD R0,R3,#0
      ADD R4,R4,#0
      BRz PRINT_DONE
      L1 OUT
         ADD R4,R4,#-1
         BRp L1
      LD R0,SaveR0
      PRINT_DONE RET

SHIFT ;;ST R0,SaveR0
      ADD R2,R2,#0
      BRz NO_DOWN
      ADD R2,R2,#-1
NO_DOWN 
      LD R0,ASCII_NEWLINE
      OUT                   ;putchar R0                   
      LD R4,COUNT
      L3 ADD R4,R4,#-1
         BRzp L3
      ;;LD R0,SaveR0
      RET
      
DONE HALT
ASCII_DOT .FILL x002E
ASCII_A .FILL x0061
ASCII_NEWLINE .FILL x000A
TOTAL .FILL x0014
SaveR0 .FILL x0000
COUNT .FILL x7FFF
.END