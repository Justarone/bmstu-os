.386P

; описание структуры дескриптора сегмента
descr   struc
    limit   dw  0
    base_l  dw  0
    base_m  db  0
    attr_1  db  0
    attr_2  db  0
    base_h  db  0
descr   ends

; сегмент данных
; use16 - использование 16-битных адресов по умолчанию. запускаем из-под DOS.
data    segment use16
    
    gdt_null descr<0,0,0,0,0,0>
    gdt_data descr<data_size-1,0,0,92h,0,0>
    gdt_code descr<code_size-1,0,0,98h,0,0>
    gdt_stack descr<255,0,0,92h,0,0>
    gdt_screen descr<4095,8000h,0Bh,92h,0,0>
    gdt_data32 descr<0FFFFh,0,0,92h,11001111b,0>
    gdt_size=$-gdt_null

    pdescr  df  0

    rm_alert  db  'Alert from real mode!'
    rm_alert_len=$-rm_alert
    pm_alert  db  'Alert from protected mode!'
    pm_alert_len=$-pm_alert

    param=1Eh
    col=12
    row=11

    data_size=$-gdt_null
data    ends


text    segment 'code' use16
    assume  cs:text, ds:data
    main    proc
        mov ax, data
        mov ds, ax
        mov ax, 0B800h
        mov es, ax

        mov di, row * 160 + col * 2
        mov bx, offset rm_alert
        mov cx, rm_alert_len
        mov ah, param

        print1: 
            mov al, byte ptr [bx]
            inc bx
            stosw
        loop    print1

        xor eax, eax
        mov ax, data
        shl eax, 4
        mov ebp, eax
        mov bx, offset gdt_data
        mov (descr ptr [bx]).base_l, ax
        shr eax, 16
        mov (descr ptr [bx]).base_m, al

        xor eax, eax
        mov ax, cs
        shl eax, 4
        mov bx, offset gdt_code
        mov (descr ptr [bx]).base_l, ax
        shr eax, 16
        mov (descr ptr [bx]).base_m, al

        xor eax, eax
        mov ax, ss
        shl eax, 4
        mov bx, offset gdt_stack
        mov (descr ptr [bx]).base_l, ax
        shr eax, 16
        mov (descr ptr [bx]).base_m, al

        mov dword ptr pdescr + 2, ebp
        mov word ptr pdescr, gdt_size - 1
        lgdt    pdescr

        cli

        ; сам переход в защищенный режим
        mov eax, cr0
        or  eax, 1
        mov cr0, eax

        ; искусственно сконструированная команда дальнего перехода для смены CS:IP
        db  0EAh
        dw  offset continue
        dw  16

    continue:
        ; заносим в ds селектор сегмента данных
        mov ax, 8
        mov ds, ax

        ; заносим в ss селектор сегмента стека
        mov ax, 24
        mov ss, ax

        ; заносим в es селектор сегмента видеопамяти
        mov ax, 32
        mov es, ax

        ; заносим в gs селектор сегмента размера 4ГБ???
        mov ax, 40
        mov gs, ax

        mov di, (row + 1) * 160 + col * 2
        mov bx, offset pm_alert
        mov cx, pm_alert_len
        mov ah, param

        print2:
            mov al, byte ptr [bx]
            inc bx
            stosw
        loop    print2


        ;mov gdt_data.limit, 0FFFFh
        ;mov gdt_code.limit, 0FFFFh
        ;mov gdt_stack.limit, 0FFFFh
        ;mov gdt_screen.limit, 0FFFFh

        ;push ds
        ;pop  ds
        ;push es
        ;pop  es
        ;push ss
        ;pop  ss

        db  0EAh
        dw  offset go
        dw  16

    go:
        mov eax, cr0
        and eax, 0FFFFFFFEh
        mov cr0, eax
        db  0EAh
        dw  offset return
        dw  text

    return:
        mov ax, data
        mov ds, ax
        mov ax, stk
        mov ss, ax

        sti

        mov di, (row + 2) * 160 + col * 2
        mov bx, offset rm_alert
        mov cx, rm_alert_len
        mov ah, param

        print3:
            mov al, byte ptr [bx]
            inc bx
            stosw
        loop    print3

        mov ax, 4C00h
        int 21h

    main    endp
    code_size=$-main
text    ends

stk segment stack 'stack'
    db  256 dup('^')
stk ends
end main
