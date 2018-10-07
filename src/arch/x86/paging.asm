global load_page_directory

section .text
    load_page_directory:
        pop eax
        mov cr3, eax
 
        ; Set paging + protection bits of cr0
        mov eax, cr0
        or eax, 0x80000001
        mov cr0, eax

        ; Enable PSE (4MiB pages)
        mov eax, cr4
        or eax, 0x00000010
        mov cr4, eax