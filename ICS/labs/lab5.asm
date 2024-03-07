.ORIG x3000
JSR GETMAPIJ
LD R0,MAP ; r0 = &map
AND R5,R5,#0
LD  R6,STACK 
LDR R1,R0,#0; R1 = N
BRZ WHILE1DONE
LDR R2,R0,#1; R2 = M
BRZ WHILE1DONE
NOT R1,R1
ADD R1,R1,#1 ; GET -N-1
ST  R1,NEGN
NOT R2,R2
ADD R2,R2,#1 ; GET -M-1
ST  R2,NEGM
AND R3,R3,#0 ; 

WHILE1
    LD  R1,NEGN
    ADD R1,R3,R1 ;; FOR I=0;I-N<0; I++
    BRZ WHILE1DONE
    AND R4,R4,#0
    WHILE2
        LD  R2,NEGM  
        ADD R2,R4,R2 ;; FOR J =0 ; J-M<0 ; J++
        BRZ WHILE2DONE
        ST  R3,SAVER3
        ST  R4,SAVER4
        ST  R5,SAVER5
        ST  R1,SAVER1
        ST  R2,SAVER2
        ADD R1,R3,#0
        ADD R2,R4,#0
        JSR DFS
        
        LD  R1,SAVER1
        LD  R2,SAVER2
        LD  R3,SAVER3
        LD  R4,SAVER4
        LD  R4,SAVER5
        ADD R1,R0,#0; 
        ADD R2,R5,#0; 
        JSR GETMAX
        ADD R5,R1,#0;r5 = max(r5,dfs)
        ADD R4,R4,#1 
        BR WHILE2
    WHILE2DONE
    ADD R3,R3,#1;
BR WHILE1
WHILE1DONE
ADD R2,R5,#0
HALT
NEGN    .BLKW   1
NEGM    .BLKW   1
MAP .FILL X4000
MEMO .FILL x5000
STACK .FILL X8000
SAVER1  .BLKW   1
SAVER2  .BLKW   1
SAVER3  .BLKW   1
SAVER4  .BLKW   1
SAVER5  .BLKW   1
GETMAX ;; R0 = M1 > M2 ? M1 : M2
ST  R1,M1
ST  R2,M2
NOT R1,R1
ADD R1,R1,#1
ADD R1,R2,R1 ;R1-R2
BRP MAXR2
LD R1,M1 
RET
MAXR2
LD R1,M2
RET
M1 .BLKW 1
M2 .BLKW 1

DFS
ADD R6,R6,#-1
STR R1,R6,#0
ADD R6,R6,#-1
STR R2,R6,#0
ADD R6,R6,#-1
STR R7,R6,#0
JSR GETMEMOIJ ; R0 = MEMO[R1][R2]
ST  R0,MEMOOLD
ADD R4,R0,#0    ;R4 = &memo[row][column]
LDR R0,R0,#0
BRZ DFSTHEN
RET ;IF(memo[row][column] != 0) return memo[row][column];
DFSTHEN
ADD R0,R0,#1
STR R0,R4,#0 ;++memo[row][column]; 
AND R3,R3,#0    ;R3 =0
ADD R3,R3,#3    ;int i = 3; i >= 0; i--
DFSFOR
BRN DFSLOOPDONE
    LD  R5,DIR
    ADD R5,R3,R5
    ADD R5,R3,R5 ; &DIR[R3][0]
    LDR R5,R5,#0 ; DIR[R3][0]
    ADD R5,R1,R5 ; R5(NEWROW) = DIR[R3][0] + ROW
    BRN CONTINUE ;newRow < 0 CONTINUE
    LD  R0,NEGN
    ADD R0,R0,R5
    BRNP CONTINUE ;newRow >= rows
    
    LD  R7,DIR
    ADD R7,R3,R7
    ADD R7,R3,R7 ; 
    ADD R4,R4,#1 ; &DIR[R3][1]
    LDR R7,R7,#0 ; DIR[R3][1]
    ADD R7,R2,R7 ; R7(NEWCOL) = DIR[R4][1] + COL
    BRN CONTINUE ; NEWCOL <0 CONTINUE
    LD  R0,NEGM
    ADD R0,R0,R7
    BRNP CONTINUE ;newColumn >= columns
    
    JSR GETMAPIJ  ; matrix[row][column]
    ST  R0,OLDPOS
    ADD R1,R5,#0
    ADD R2,R7,#0
    JSR GETMAPIJ
    ST  R0,NEWPOS
    LDI R4,NEWPOS ; R4=matrix[newRow][newColumn]
    LDI R5,OLDPOS ; R5=matrix[row][column]
    NOT R5,R5
    ADD R5,R5,#1
    ADD R4,R5,R4    
    BRNZ    CONTINUE
    LDI R4,MEMOOLD
    JSR DFS
    ADD R5,R0,#0
    ADD R1,R4,#0;memo[row][column]
    ADD R2,R5,#1;dfs(matrix, newRow, newColumn, memo) + 1
    JSR GETMAX
    LD  R4,MEMOOLD
    STR R0,R4,#0;memo[row][column] = max(memo[row][column], dfs(matrix, newRow, newColumn, memo) + 1);
    CONTINUE
    ADD R3,R3,#-1
BR DFSFOR
DFSLOOPDONE

LDI R0,MEMOOLD
LDR R7,R6,#0
ADD R6,R6,#1
LDR R2,R6,#0
ADD R6,R6,#1
LDR R1,R6,#0
ADD R6,R6,#1
RET
MAPA    .fill x4002
DIR .FILL   -1
    .FILL   0
    .FILL   1
    .FILL   0
    .FILL   0
    .FILL   -1
    .FILL   0
    .FILL   1 
NEWPOS  .BLKW   1
OLDPOS  .BLKW   1
MEMOOLD .BLKW   1
GETMAPij ;r0=Matrix[i][j] r1 = i ,r2 = j ,A + I*M + J GET
LD  R0,MAPA
ADD R0,R0,R2 ; R0 = A+J
LDI R2,COL1 ; R2 = M
LOOPMAPIJ
BRZ DONEMAPIJ
ADD R0,R1,R0 ; R0+=I FOR m TIMES
ADD R2,R2,#-1
BR  LOOPMAPIJ
DONEMAPIJ
RET
COL1 .FILL X4001

MEMOA   .FILL x5000
GETMEMOij ;r0=Memo[i][j] r1 = i ,r2 = j ,A + I*M + J
LD  R0,MEMOA
ADD R0,R0,R2 ; R0 = A+J
LDI R2,COL1 ; R2 = M
LOOPMEMOIJ
BRZ DONEMEMOIJ
ADD R0,R1,R0 ; R0+=I FOR m TIMES
ADD R2,R2,#-1
BR  LOOPMEMOIJ
DONEMEMOIJ
RET

.END

.ORIG x4000
.FILL #3 ; N
.FILL #4 ; M
.FILL #89 ; the map
.FILL #88
.FILL #86
.FILL #83
.FILL #79
.FILL #73
.FILL #90
.FILL #80
.FILL #60
.FILL #69
.FILL #73
.FILL #77
.END

.orig x5000
.BLKW 2500
.end
