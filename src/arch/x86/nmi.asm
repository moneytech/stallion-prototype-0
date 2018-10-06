global disable_nmi
global enable_nmi

section .text
    enable_nmi:
        ; outb(0x70, inb(0x70) & 0x7F);
        ; First, inb(0x70)
        in al, 0x070
        and al, 0x7f
        out 0x70, al
        ret

    disable_nmi:
        ; outb(0x70, inb(0x70) | 0x80);
        in al, 0x70
        or al, 0x80
        out 0x70, al
        ret