global enable_a20

; TODO: More robust implementation of A20 Line stuff

section .text
    enable_a20:
        in al, 0x92
        test al, 2
        jnz .after
        or al, 2
        and al, 0xFE
        out 0x92, al
        .after:
            ret