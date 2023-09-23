# 1.Algorithm

The main program is three loops, printing a total of 17. And 3 bird characters x (a-z), if no interruption occurs, then the height y-- at the end of each loop.
Interrupt program is to modify the preset character x or height y according to the user's keyboard input, here only need to take the KBDR data and judge the type,then modify the corresponding x y 。
The preparation of the interrupter requires enabling KBSR[14] and placing the start address of the interrupter at x0180

```
	char op; op = getchar(); // INTERRUPT PART
		if(op>='a' && op <='z'){ 
		x=op;
		} // 
		else if(op>='0' && op <='9'){
		y+=op-'0'; 
		y= (y>17?17:y);
		}
		cnt = 17;
		int temy = y;
	while(ture){			 // PRINT PART
		while(temy--){ 
			printf(".");
			cnt--;}
		for(int i = 0;i<3;i++) 
			printf("%c",x);
		while(cnt--){
			printf(".");
		}
		printf("\n");
		Delay()
		if(y) y--;}
```

# 2.Essential parts of code with sufficient comments

```LC3
;;initialize interrupt
.ORIG x0200
... PUSH PSR AND PC
LD R1,EN
LD R2,KBSR
STR R1,R2,#0    ;enable the keyboard interrupt
LD R1,MAIN       
LD R2,MAIN_IN_TABLE   ;Store interrupt routine start address
STR R1,R2,#0        
RTI
MAIN_IN_TABLE .FILL x0180 ;keyboard interrupt start by 01 + 80 , 80 is INTV for keyboard
EN .FILL   x4000          ;Set KBSR[14] = 1
MAIN .FILL x2000      ;interrupt routine
KBSR .FILL xFE00     
...
.END

;interrupt routine Check input 
;if input is a-z then change KBDR
.ORIG x2000
...SOME Stack PUSH
LDI R0,KBDR
LD R3,NEG_A
ADD R3,R0,R3
BRn CHECKDIGIT
LD R3,NEG_Z
ADD R3,R0,R3
BRp FINISH		; CHECK input R0 is from a-z
; ADD R1,R0,#0
STR R0,R6,#0
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
FINISH	... SOME STACK POP
RTI
NEG_A .FILL xFF9F
NEG_Z .FILL xFF86
NEG_0 .FILL xFFD0
KBDR  .FILL xFE02
NEG_SEVENTEEN .FILL xFFEF
SEVENTEEN .FILL x0012
.END
.ORIG x3000 ; PRINT PART
LD R1,A       ;x=a
AND R2,R2,#0
ADD R2,R2,#5        ;y=5
WHILE
   LD R0,DOT  ;R0 = '.'
   LD R3,TOTAL      ;R3(cnt) = 17
   ADD R4,R2,#0     ;R4(temy) = y
   LOOP1 BRZ LOOP1OUT   
        OUT          ;PUTCHAR('.')
        ADD R3,R3,#-1;CNT  --
        ADD R4,R4,#-1;TEMY --
        BR LOOP1
   LOOP1OUT AND R4,R4,#0;
   ADD R0,R1,#0;
   ADD R4,R4,#3;
   LOOP2 BRZ LOOP2OUT   
        OUT          ;PUTCHAR(R1)
        ADD R4,R4,#-1; 3 times
        BR LOOP2
    LOOP2OUT
    LD  R0,DOT
    ADD R3,R3,#0;   ;set CC
    LOOP3 BRZ LOOP3OUT
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
... 
DELAY  ... OMIT
SAVER5 .BLKW #1
DOT .FILL x002E
A .FILL x0061
NEWLINE .FILL x000A
TOTAL .FILL x0011
SaveR0 .FILL x0000
COUNT .FILL x4000
.END
```

# 3.Questions

## 1.How To Check the Priority of Keyboard

You just need to set an endpoint anywhere in the keyboard interrupt function and look at its PSR after the interrupt occurs, and you can know the priority through the PSR

## 2.How does keyboard Interrupt works

Firstly, KBSR[14]=1 is designed at booting, which means that the keyboard has the right to interrupt. Then, when the user enters characters at the time of program execution, KBSR[15]=1 will be created, which means that the user has the right and has the demand. Then the system will sort according to the priority of the current task and find that the priority of the keyboard interrupt is higher. So select and send the INT signal, the system receives the INT signal and uses INTV to look up the table to get the start address of the interrupt subroutine, jumps to the interrupt subroutine and executes