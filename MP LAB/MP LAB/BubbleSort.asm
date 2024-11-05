MOV BX,0200H
; Let's insert the 8-bit unsigned 10 numbers we want from 0700:0200.
MOV BYTE PTR [bx],55h
MOV BYTE PTR [bx+1],32H
MOV BYTE PTR [bx+2],54H
MOV BYTE PTR [bx+3],11H
MOV BYTE PTR [bx+4],47H
MOV BYTE PTR [bx+5],33H
MOV BYTE PTR [bx+6],75H
MOV BYTE PTR [bx+7],01H
MOV BYTE PTR [bx+8],5FH
MOV BYTE PTR [bx+9],23H

org 100h             ;offset addresses of commands

MOV BX,0200H
MOV SI,0             ;The register to be used in the inner loop iteration
MOV DI,0             ;The register to be used in outer loop iteration

innerloop:

MOV DL,[BX+SI]       ;register to hold the smallest number
MOV AL,[BX+SI+1]     ;Gets the corresponding byte at base address 0700:0200 (indexed with SI+1)

CMP AL,DL            ;If the value of the al register is greater than or equal to dl,
JAE @IF              ;if al >= dl (cf=0) go to if block
JB @ELSE             ;if al < dl (cf=1) go to else block

@IF:                 ;if block
MOV [BX+SI],DL       ;If al is greater than dl, there is no need to swap 
MOV [BX+SI+1],AL     ; because they are already sorted among themselves.
INC SI               ;For iteration, increase the register by 1 (si=si+1)

CMP SI,9             ;Is si equal to 9?
JNZ innerloop        ;(zf=0) enter the inner loop

LOOP outerloop       ;(zf=1) enter the outerloop

@ELSE:               ;else block
MOV [BX+SI],AL       ;values are swapped in the same regions in memory
MOV [BX+SI+1],DL
MOV CL,AL            ;swap for registers
MOV AL,DL
MOV DL,CL
INC SI               ;increment register by 1 for iteration(si=si+1)

CMP SI,9             ;Is si equal to 9?
JNZ innerloop        ;(zf=0) enter the inner loop

LOOP outerloop       ;(zf=1) enter the outerloop

outerloop:            
INC DI               ;
MOV SI,0             ;si is reset to be able to reorder
CMP DI,9             ;has the loop reached the end of its step (di=9)
JNZ innerloop        ;If di is not equal to 9, enter the inner loop and sort again

; Code to print the sorted numbers
PRINT_SORTED_NUMBERS:
MOV CX,10            ; We'll print 10 numbers
MOV SI,0             ; Start from the first number in the array
PRINT_LOOP:
MOV AL,[BX+SI]       ; Load the number from the sorted array

; Convert the byte in AL to two ASCII characters representing hex digits

MOV AH,AL            ; Move AL to AH to isolate high nibble
SHR AH,4             ; Shift right 4 bits to get the high nibble
CALL PRINT_HEX       ; Print the high nibble

MOV AL,[BX+SI]       ; Load the number again to print the low nibble
AND AL,0Fh           ; Mask to isolate the low nibble
CALL PRINT_HEX       ; Print the low nibble

MOV AH,02h           ; DOS interrupt to print character
MOV DL,' '           ; Print a space between numbers
INT 21h

INC SI               ; Move to the next number in the array
LOOP PRINT_LOOP

HLT                  ; STOP the program

; Subroutine to print a hexadecimal digit
PRINT_HEX:
ADD AL,30h           ; Convert to ASCII assuming it's 0-9
CMP AL,39h
JBE PRINT_DONE       ; If it's less than or equal to '9', it's a digit

ADD AL,7             ; Convert to 'A'-'F' for hex digits
PRINT_DONE:
MOV AH,02h           ; DOS interrupt to print character
MOV DL,AL            ; Load the ASCII character to print
INT 21h
RET                  ; Return from subroutine
