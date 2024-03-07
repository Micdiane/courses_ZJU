.ORIG x3000

START   LEA R0, MAP      ; load address of map into R0
        LD R1, N        ; load N into R1
        ADD R1, R1, #-1 ; subtract 1 from N (to get last valid row index)
        LD R3, M        ; load M into R3
        ADD R3, R3, #-1 ; subtract 1 from M (to get last valid column index)
        AND R2, R2, #0  ; clear R2 (used to store longest distance found so far)
        ADD R4, R0, #0  ; set R4 to start of map (used for recursive calls)
        JSR FIND_LONGEST_PATH ; call recursive function
        HALT

; recursive function for finding longest path from a given point
FIND_LONGEST_PATH
        ST R7, SP        ; save return address
        ADD SP, SP, #-1 ; allocate space for local variables
        ST R0, 0(SP)     ; save address of map
        ST R1, 1(SP)     ; save row index of current point
        ST R2, 2(SP)     ; save column index of current point
        ST R3, 3(SP)     ; save last valid row index
        ST R4, 4(SP)     ; save start of map
        ST R5, 5(SP)     ; save current longest distance
        AND R5, R5, #0   ; clear R5 (used to store longest distance found so far from current point)

        ; check neighboring points
        LD R0, 0(SP)     ; load address of map
        LD R1, 1(SP)     ; load row index
        LD R2, 2(SP)     ; load column index
        LD R3, 3(SP)     ; load last valid row index
        ADD R6, R1, #-1  ; set R6 to row index of north neighbor
        BRz SKIP_NORTH    ; skip if already at top row
        ADD R6, R0, R6   ; set R6 to address of north neighbor
        ADD R6, R6, R2   ; set R6 to address of north neighbor column
        LDR R7, R6, #0   ; load altitude of north neighbor
        LDR R8, R0, R1   ; load altitude of current point
        ADD R1, R1, #-1  ; decrement row index
        BRn SKIP_NORTH    ; skip if north neighbor has higher altitude
        JSR FIND_LONGEST_PATH ; recursively find longest path from north neighbor
        ADD R5, R5, #1   ; add 1 to longest distance found so far
        ADD R5, R5, R7   ; add altitude of north neighbor
        BRnz PUBLISH_LONGEST_PATH ; publish longest path if greater than current longest path

SKIP_NORTH
        ADD R1, R1, #1   ; restore row index

        ADD R6, R2, #-1  ; set R6 to column index of west neighbor
        BRz SKIP_WEST     ; skip if already at leftmost column
        ADD R6, R0, R1   ; set R6 to address of west neighbor row
        ADD R6, R6, R6   ; multiply row index by 2 (since each row takes up 2 memory locations)
        ADD R6, R6, R6   ; multiply by 4 (since each altitude takes up 4 bits)
        ADD R6, R6, R2   ; set R6 to address of west neighbor column
        LDR R7, R6, #0   ; load altitude of west neighbor
        LDR R8, R0, R1   ; load altitude of current point
        ADD R2, R2, #-1  ; decrement column index
        BRn SKIP_WEST     ; skip if west neighbor has higher altitude
        JSR FIND_LONGEST_PATH ; recursively find longest path from west neighbor
        ADD R5, R5, #1   ; add 1 to longest distance foundso far
        ADD R5, R5, R7   ; add altitude of west neighbor
        BRnz PUBLISH_LONGEST_PATH ; publish longest path if greater than current longest path

SKIP_WEST
        ADD R2, R2, #1   ; restore column index

        ADD R6, R1, #1   ; set R6 to row index of south neighbor
        ADD R6, R6, R6   ; multiply by 2 (since each row takes up 2 memory locations)
        ADD R6, R6, R6   ; multiply by 4 (since each altitude takes up 4 bits)
        ADD R6, R6, R2   ; set R6 to address of south neighbor column
        BRp SKIP_SOUTH    ; skip if already at bottom row
        ADD R6, R6, R1   ; set R6 to address of south neighbor row
        LDR R7, R6, #0   ; load altitude of south neighbor
        LDR R8, R0, R1   ; load altitude of current point
        ADD R1, R1, #1   ; increment row index
        BRn SKIP_SOUTH    ; skip if south neighbor has higher altitude
        JSR FIND_LONGEST_PATH ; recursively find longest path from south neighbor
        ADD R5, R5, #1   ; add 1 to longest distance found so far
        ADD R5, R5, R7   ; add altitude of south neighbor
        BRnz PUBLISH_LONGEST_PATH ; publish longest path if greater than current longest path

SKIP_SOUTH
        ADD R1, R1, #-1  ; restore row index

        ADD R6, R2, #1   ; set R6 to column index of east neighbor
        ADD R6, R6, R6   ; multiply by 2 (since each row takes up 2 memory locations)
        ADD R6, R6, R6   ; multiply by 4 (since each altitude takes up 4 bits)
        ADD R6, R6, R1   ; set R6 to address of east neighbor row
        BRp SKIP_EAST     ; skip if already at rightmost column
        ADD R6, R6, R2   ; set R6 to address of east neighbor column
        LDR R7, R6, #0   ; load altitude of east neighbor
        LDR R8, R0, R1   ; load altitude of current point
        ADD R2, R2, #1   ; increment column index
        BRn SKIP_EAST     ; skip if east neighbor has higher altitude
        JSR FIND_LONGEST_PATH ; recursively find longest path from east neighbor
        ADD R5, R5, #1   ; add 1 to longest distance found so far
        ADD R5, R5, R7   ; add altitude of east neighbor
        BRnz PUBLISH_LONGEST_PATH ; publish longest path if greater than current longest path

SKIP_EAST
        ADD R2, R2, #-1  ; restore column index

        BRA END_RECURSION

; publish longest path found so far
PUBLISH_LONGEST_PATH
        LD R2, 5(SP)     ; load current longest distance from stack
        LD R5, 2(SP)     ; load longest distance found so far
        ADD R2, R2, R5   ; add current longest distance to longest distance found so far
        ST R2, 2(SP)     ; save new longest distance found so far

END_RECURSION
        LD R7, SP+1      ; restore return address
        LD R0, SP        ; restore local variables
        LD R1, 1(SP)
        LD R2, 2(SP)
        LD R3, 3(SP)
        LD R4, 4(SP)
        LD R5, 5(SP)
        ADD SP, SP, #1   ; deallocate space for local variables
        JMP R7           ; return to calling function

; data section
MAP     .FILL #N
        .FILL #M
        .FILL #89
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
        .BLKW 1000      ; reserve space for rest of map

N       .FILL #3
M       .FILL #4

.END