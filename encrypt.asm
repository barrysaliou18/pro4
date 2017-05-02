;Brian Jorgenson

;Setup
	.ORIG x3000
	AND R1, R1, #0	; Clear some registers
	AND R2, R2, #0
	AND R3, R3, #0
	AND R4, R4, #0
	AND R5, R5, #0
	AND R6, R6, #0
    
    
;
; Ask user for 20 char message
; Temp registers: R4, R3, R2
; R4 will be the input message character pointer
; R3 will hold the input message length counter
; R2 will hold the negative ASCII for enter (xA)
;
	LEA R0, MES	; Ask user for message
	PUTS
	LD R4, INPUT	; Set R4 at the start of the message memory
	LD R3, MAXMES	; Set R3 to max message length (#20)
AGAIN	GETC
	OUT 
	JSR CHECKCHAR	; Check for enter character
	STR R0, R4, #0	; Store character at message pointer
	ADD R4, R4, #1	; Increment pointer
	ADD R3, R3, #-1	; Decrement characters that are left
	BRp AGAIN	; If positive there is still space for more characters
DONE	AND R0, R0, #0	; Put a terminating character after the message
	STR R0, R4, #0	
	BR ENCRYPT	; Goto ENCRYPT

; Data used in ASKMESSAGE
ENTERVAL .FILL xFFF6	; Negative value of ASCII enter
MES	.STRINGZ "Enter message (20 char limit): "
MAXMES	.FILL #20	; Max message length
INPUT	.FILL x3102	; Location of message start   

;
; Check for enter character
; Temp registers: R2
; R2 will hold the negative value of ASCII enter/carriage return
;
CHECKCHAR
	LD R2, ENTERVAL	; Set R2 to negative ASCII enter 
	ADD R2, R0, R2	; Check for enter character
	BRnp NOTENTER	; If zero it's an enter character
	BR DONE		; Goto DONE
NOTENTER RET		; Character was not an enter character, get more characters

;
; Encrypt the message
; Temp registers: R4, R0
; R4 will be the message input/output pointer
; R0 will hold the data to be encrypted
;
ENCRYPT
    ADD R5, R5, #2  ; Set R5 to have the constant key
    NOT R5, R5      ; 2's comp on key to make it negative
    ADD R5, R5, #1
	LD R4, INPUT	; Set pointer to start of message input
EAGAIN	LDR R0, R4, #0	; Load character to R0 from memory input pointer
	JSR ENDOFM	; Check if end of message
	ADD R0, R0, R5	; Add (subtract) the key
	STR R0, R4, #0	; Save to memory output pointer
	ADD R4, R4, #1	; Increment pointers
	BR EAGAIN	; loop
    
;
; End of message?
;
ENDOFM
	ADD R0, R0, #0	; Check if end of message
	BRnp NOTEND	; If not zero, not end of message
	STR R0, R3, #0	; Store terminating character at output pointer
	BR DISPLAY	; Goto DISPLAY, done with encryption/decryption
NOTEND	RET

;
; Display encrypted message
;
DISPLAY
    LEA R0, PRESSKEY; output "press any key..." message
    PUTS
    GETC            ; wait for input (any)
	LD R0, INPUT	; Load output location to R0
	PUTS            ; print encrypted message

	HALT
PRESSKEY .STRINGZ "Press any key to continue: "

	.END
    