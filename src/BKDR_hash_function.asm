
section .text

;------------------------------------------------
;   BKDR hash-function 
;   Seed value = 31
;------------------------------------------------
; entry:    RBX = str addr
; exit:		RAX = hash
; destroys:	RAX
;------------------------------------------------

global GetBkdrHashAsm

GetBkdrHashAsm:     ; proc
                    
                    mov rax, 0      ; Hash value    
                    mov r8,  31
                    xor rcx, rcx

                    .loop:
                        mul r8
                        
                        mov cl, [rdi]         ; hash += *str_addr
                        add rax, rcx          
                        
                        inc rdi                 ; str_addr++ 

                        cmp byte [rdi], 0       ; if( *str_addr == '\0' ) break;   
                        jne .loop                

                    ret 
                    ; endp

;------------------------------------------------
