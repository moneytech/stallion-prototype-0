global gdt

section .data
    gdt_start:

    gdt_null:
        dd 0
        dd 0

    gdt_cs:
    	dw 0xFFFF    ; Limit
    	dw 0x0000    ; Base
    	db 0x0000	 ; Base 23:16
    	db 10011011b
    	db 11011111b
    	db 0x0000

    gdt_ds:
    	dw 0xFFFF    ; Limit
    	dw 0x0000    ; Base
    	db 0x0000	 ; Base 23:16
    	db 10010011b
    	db 11011111b
    	db 0x0000

    gdt_end:

    gdt:
        dw gdt_end - gdt_start -1
        dd gdt