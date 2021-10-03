
dist/x86/kernel.elf:     file format elf32-i386


Disassembly of section .text:

c0101000 <_boot>:
extern _kernel_start
extern _kernel_end
extern _kernel_main
global _boot
_boot:
    mov ecx, boot_page_directory - ADDROFF
c0101000:	b9 00 30 10 00       	mov    $0x103000,%ecx
    mov cr3, ecx
c0101005:	0f 22 d9             	mov    %ecx,%cr3

    mov ecx, cr4
c0101008:	0f 20 e1             	mov    %cr4,%ecx
    or ecx, 0x00000010
c010100b:	83 c9 10             	or     $0x10,%ecx
    mov cr4, ecx
c010100e:	0f 22 e1             	mov    %ecx,%cr4

    mov ecx, cr0
c0101011:	0f 20 c1             	mov    %cr0,%ecx
    or ecx, 0x80000000
c0101014:	81 c9 00 00 00 80    	or     $0x80000000,%ecx
    mov cr0, ecx
c010101a:	0f 22 c1             	mov    %ecx,%cr0
    lea ecx, [_boot_paged]
c010101d:	8d 0d 25 10 10 c0    	lea    0xc0101025,%ecx
    jmp ecx
c0101023:	ff e1                	jmp    *%ecx

c0101025 <_boot_paged>:

section .text
_boot_paged:

    mov dword[boot_page_directory], 0
c0101025:	c7 05 00 30 10 c0 00 	movl   $0x0,0xc0103000
c010102c:	00 00 00 

    mov ecx, cr3
c010102f:	0f 20 d9             	mov    %cr3,%ecx
    mov cr3, ecx
c0101032:	0f 22 d9             	mov    %ecx,%cr3

    mov esp, stack_top
c0101035:	bc 8c 98 10 c0       	mov    $0xc010988c,%esp
    call _kernel_main
c010103a:	e8 c3 09 00 00       	call   c0101a02 <_kernel_main>


    cli
c010103f:	fa                   	cli    
    hlt
c0101040:	f4                   	hlt    
    jmp $
c0101041:	eb fe                	jmp    c0101041 <_boot_paged+0x1c>
c0101043:	66 90                	xchg   %ax,%ax
c0101045:	66 90                	xchg   %ax,%ax
c0101047:	66 90                	xchg   %ax,%ax
c0101049:	66 90                	xchg   %ax,%ax
c010104b:	66 90                	xchg   %ax,%ax
c010104d:	66 90                	xchg   %ax,%ax
c010104f:	90                   	nop

c0101050 <__irq0>:


%macro DECLIRQ 2
global __irq %+ %1
__irq %+ %1:
    cli
c0101050:	fa                   	cli    
    %if !(%2)
    push byte 0
c0101051:	6a 00                	push   $0x0
    %endif
    push byte %1+32
c0101053:	6a 20                	push   $0x20
    IRQCOMMON

%endmacro

%macro IRQCOMMON 0
pusha
c0101055:	60                   	pusha  
push ds
c0101056:	1e                   	push   %ds
push es
c0101057:	06                   	push   %es
push fs
c0101058:	0f a0                	push   %fs
push gs
c010105a:	0f a8                	push   %gs
mov ax, 0x10
c010105c:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101060:	8e d8                	mov    %eax,%ds
mov es, ax
c0101062:	8e c0                	mov    %eax,%es
mov fs, ax
c0101064:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101066:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101068:	89 e0                	mov    %esp,%eax
push eax
c010106a:	50                   	push   %eax
mov eax, __irq_handler
c010106b:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c0101070:	ff d0                	call   *%eax
pop eax
c0101072:	58                   	pop    %eax
pop gs
c0101073:	0f a9                	pop    %gs
pop fs
c0101075:	0f a1                	pop    %fs
pop es
c0101077:	07                   	pop    %es
pop ds
c0101078:	1f                   	pop    %ds
popa
c0101079:	61                   	popa   
add esp, 8 
c010107a:	83 c4 08             	add    $0x8,%esp
iret
c010107d:	cf                   	iret   

c010107e <__irq1>:
    cli
c010107e:	fa                   	cli    
    push byte 0
c010107f:	6a 00                	push   $0x0
    push byte %1+32
c0101081:	6a 21                	push   $0x21
pusha
c0101083:	60                   	pusha  
push ds
c0101084:	1e                   	push   %ds
push es
c0101085:	06                   	push   %es
push fs
c0101086:	0f a0                	push   %fs
push gs
c0101088:	0f a8                	push   %gs
mov ax, 0x10
c010108a:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c010108e:	8e d8                	mov    %eax,%ds
mov es, ax
c0101090:	8e c0                	mov    %eax,%es
mov fs, ax
c0101092:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101094:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101096:	89 e0                	mov    %esp,%eax
push eax
c0101098:	50                   	push   %eax
mov eax, __irq_handler
c0101099:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c010109e:	ff d0                	call   *%eax
pop eax
c01010a0:	58                   	pop    %eax
pop gs
c01010a1:	0f a9                	pop    %gs
pop fs
c01010a3:	0f a1                	pop    %fs
pop es
c01010a5:	07                   	pop    %es
pop ds
c01010a6:	1f                   	pop    %ds
popa
c01010a7:	61                   	popa   
add esp, 8 
c01010a8:	83 c4 08             	add    $0x8,%esp
iret
c01010ab:	cf                   	iret   

c01010ac <__irq2>:
    cli
c01010ac:	fa                   	cli    
    push byte 0
c01010ad:	6a 00                	push   $0x0
    push byte %1+32
c01010af:	6a 22                	push   $0x22
pusha
c01010b1:	60                   	pusha  
push ds
c01010b2:	1e                   	push   %ds
push es
c01010b3:	06                   	push   %es
push fs
c01010b4:	0f a0                	push   %fs
push gs
c01010b6:	0f a8                	push   %gs
mov ax, 0x10
c01010b8:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01010bc:	8e d8                	mov    %eax,%ds
mov es, ax
c01010be:	8e c0                	mov    %eax,%es
mov fs, ax
c01010c0:	8e e0                	mov    %eax,%fs
mov gs, ax
c01010c2:	8e e8                	mov    %eax,%gs
mov eax, esp
c01010c4:	89 e0                	mov    %esp,%eax
push eax
c01010c6:	50                   	push   %eax
mov eax, __irq_handler
c01010c7:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c01010cc:	ff d0                	call   *%eax
pop eax
c01010ce:	58                   	pop    %eax
pop gs
c01010cf:	0f a9                	pop    %gs
pop fs
c01010d1:	0f a1                	pop    %fs
pop es
c01010d3:	07                   	pop    %es
pop ds
c01010d4:	1f                   	pop    %ds
popa
c01010d5:	61                   	popa   
add esp, 8 
c01010d6:	83 c4 08             	add    $0x8,%esp
iret
c01010d9:	cf                   	iret   

c01010da <__irq3>:
    cli
c01010da:	fa                   	cli    
    push byte 0
c01010db:	6a 00                	push   $0x0
    push byte %1+32
c01010dd:	6a 23                	push   $0x23
pusha
c01010df:	60                   	pusha  
push ds
c01010e0:	1e                   	push   %ds
push es
c01010e1:	06                   	push   %es
push fs
c01010e2:	0f a0                	push   %fs
push gs
c01010e4:	0f a8                	push   %gs
mov ax, 0x10
c01010e6:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01010ea:	8e d8                	mov    %eax,%ds
mov es, ax
c01010ec:	8e c0                	mov    %eax,%es
mov fs, ax
c01010ee:	8e e0                	mov    %eax,%fs
mov gs, ax
c01010f0:	8e e8                	mov    %eax,%gs
mov eax, esp
c01010f2:	89 e0                	mov    %esp,%eax
push eax
c01010f4:	50                   	push   %eax
mov eax, __irq_handler
c01010f5:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c01010fa:	ff d0                	call   *%eax
pop eax
c01010fc:	58                   	pop    %eax
pop gs
c01010fd:	0f a9                	pop    %gs
pop fs
c01010ff:	0f a1                	pop    %fs
pop es
c0101101:	07                   	pop    %es
pop ds
c0101102:	1f                   	pop    %ds
popa
c0101103:	61                   	popa   
add esp, 8 
c0101104:	83 c4 08             	add    $0x8,%esp
iret
c0101107:	cf                   	iret   

c0101108 <__irq4>:
    cli
c0101108:	fa                   	cli    
    push byte 0
c0101109:	6a 00                	push   $0x0
    push byte %1+32
c010110b:	6a 24                	push   $0x24
pusha
c010110d:	60                   	pusha  
push ds
c010110e:	1e                   	push   %ds
push es
c010110f:	06                   	push   %es
push fs
c0101110:	0f a0                	push   %fs
push gs
c0101112:	0f a8                	push   %gs
mov ax, 0x10
c0101114:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101118:	8e d8                	mov    %eax,%ds
mov es, ax
c010111a:	8e c0                	mov    %eax,%es
mov fs, ax
c010111c:	8e e0                	mov    %eax,%fs
mov gs, ax
c010111e:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101120:	89 e0                	mov    %esp,%eax
push eax
c0101122:	50                   	push   %eax
mov eax, __irq_handler
c0101123:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c0101128:	ff d0                	call   *%eax
pop eax
c010112a:	58                   	pop    %eax
pop gs
c010112b:	0f a9                	pop    %gs
pop fs
c010112d:	0f a1                	pop    %fs
pop es
c010112f:	07                   	pop    %es
pop ds
c0101130:	1f                   	pop    %ds
popa
c0101131:	61                   	popa   
add esp, 8 
c0101132:	83 c4 08             	add    $0x8,%esp
iret
c0101135:	cf                   	iret   

c0101136 <__irq5>:
    cli
c0101136:	fa                   	cli    
    push byte 0
c0101137:	6a 00                	push   $0x0
    push byte %1+32
c0101139:	6a 25                	push   $0x25
pusha
c010113b:	60                   	pusha  
push ds
c010113c:	1e                   	push   %ds
push es
c010113d:	06                   	push   %es
push fs
c010113e:	0f a0                	push   %fs
push gs
c0101140:	0f a8                	push   %gs
mov ax, 0x10
c0101142:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101146:	8e d8                	mov    %eax,%ds
mov es, ax
c0101148:	8e c0                	mov    %eax,%es
mov fs, ax
c010114a:	8e e0                	mov    %eax,%fs
mov gs, ax
c010114c:	8e e8                	mov    %eax,%gs
mov eax, esp
c010114e:	89 e0                	mov    %esp,%eax
push eax
c0101150:	50                   	push   %eax
mov eax, __irq_handler
c0101151:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c0101156:	ff d0                	call   *%eax
pop eax
c0101158:	58                   	pop    %eax
pop gs
c0101159:	0f a9                	pop    %gs
pop fs
c010115b:	0f a1                	pop    %fs
pop es
c010115d:	07                   	pop    %es
pop ds
c010115e:	1f                   	pop    %ds
popa
c010115f:	61                   	popa   
add esp, 8 
c0101160:	83 c4 08             	add    $0x8,%esp
iret
c0101163:	cf                   	iret   

c0101164 <__irq6>:
    cli
c0101164:	fa                   	cli    
    push byte 0
c0101165:	6a 00                	push   $0x0
    push byte %1+32
c0101167:	6a 26                	push   $0x26
pusha
c0101169:	60                   	pusha  
push ds
c010116a:	1e                   	push   %ds
push es
c010116b:	06                   	push   %es
push fs
c010116c:	0f a0                	push   %fs
push gs
c010116e:	0f a8                	push   %gs
mov ax, 0x10
c0101170:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101174:	8e d8                	mov    %eax,%ds
mov es, ax
c0101176:	8e c0                	mov    %eax,%es
mov fs, ax
c0101178:	8e e0                	mov    %eax,%fs
mov gs, ax
c010117a:	8e e8                	mov    %eax,%gs
mov eax, esp
c010117c:	89 e0                	mov    %esp,%eax
push eax
c010117e:	50                   	push   %eax
mov eax, __irq_handler
c010117f:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c0101184:	ff d0                	call   *%eax
pop eax
c0101186:	58                   	pop    %eax
pop gs
c0101187:	0f a9                	pop    %gs
pop fs
c0101189:	0f a1                	pop    %fs
pop es
c010118b:	07                   	pop    %es
pop ds
c010118c:	1f                   	pop    %ds
popa
c010118d:	61                   	popa   
add esp, 8 
c010118e:	83 c4 08             	add    $0x8,%esp
iret
c0101191:	cf                   	iret   

c0101192 <__irq7>:
    cli
c0101192:	fa                   	cli    
    push byte 0
c0101193:	6a 00                	push   $0x0
    push byte %1+32
c0101195:	6a 27                	push   $0x27
pusha
c0101197:	60                   	pusha  
push ds
c0101198:	1e                   	push   %ds
push es
c0101199:	06                   	push   %es
push fs
c010119a:	0f a0                	push   %fs
push gs
c010119c:	0f a8                	push   %gs
mov ax, 0x10
c010119e:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01011a2:	8e d8                	mov    %eax,%ds
mov es, ax
c01011a4:	8e c0                	mov    %eax,%es
mov fs, ax
c01011a6:	8e e0                	mov    %eax,%fs
mov gs, ax
c01011a8:	8e e8                	mov    %eax,%gs
mov eax, esp
c01011aa:	89 e0                	mov    %esp,%eax
push eax
c01011ac:	50                   	push   %eax
mov eax, __irq_handler
c01011ad:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c01011b2:	ff d0                	call   *%eax
pop eax
c01011b4:	58                   	pop    %eax
pop gs
c01011b5:	0f a9                	pop    %gs
pop fs
c01011b7:	0f a1                	pop    %fs
pop es
c01011b9:	07                   	pop    %es
pop ds
c01011ba:	1f                   	pop    %ds
popa
c01011bb:	61                   	popa   
add esp, 8 
c01011bc:	83 c4 08             	add    $0x8,%esp
iret
c01011bf:	cf                   	iret   

c01011c0 <__irq8>:
    cli
c01011c0:	fa                   	cli    
    push byte 0
c01011c1:	6a 00                	push   $0x0
    push byte %1+32
c01011c3:	6a 28                	push   $0x28
pusha
c01011c5:	60                   	pusha  
push ds
c01011c6:	1e                   	push   %ds
push es
c01011c7:	06                   	push   %es
push fs
c01011c8:	0f a0                	push   %fs
push gs
c01011ca:	0f a8                	push   %gs
mov ax, 0x10
c01011cc:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01011d0:	8e d8                	mov    %eax,%ds
mov es, ax
c01011d2:	8e c0                	mov    %eax,%es
mov fs, ax
c01011d4:	8e e0                	mov    %eax,%fs
mov gs, ax
c01011d6:	8e e8                	mov    %eax,%gs
mov eax, esp
c01011d8:	89 e0                	mov    %esp,%eax
push eax
c01011da:	50                   	push   %eax
mov eax, __irq_handler
c01011db:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c01011e0:	ff d0                	call   *%eax
pop eax
c01011e2:	58                   	pop    %eax
pop gs
c01011e3:	0f a9                	pop    %gs
pop fs
c01011e5:	0f a1                	pop    %fs
pop es
c01011e7:	07                   	pop    %es
pop ds
c01011e8:	1f                   	pop    %ds
popa
c01011e9:	61                   	popa   
add esp, 8 
c01011ea:	83 c4 08             	add    $0x8,%esp
iret
c01011ed:	cf                   	iret   

c01011ee <__irq9>:
    cli
c01011ee:	fa                   	cli    
    push byte 0
c01011ef:	6a 00                	push   $0x0
    push byte %1+32
c01011f1:	6a 29                	push   $0x29
pusha
c01011f3:	60                   	pusha  
push ds
c01011f4:	1e                   	push   %ds
push es
c01011f5:	06                   	push   %es
push fs
c01011f6:	0f a0                	push   %fs
push gs
c01011f8:	0f a8                	push   %gs
mov ax, 0x10
c01011fa:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01011fe:	8e d8                	mov    %eax,%ds
mov es, ax
c0101200:	8e c0                	mov    %eax,%es
mov fs, ax
c0101202:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101204:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101206:	89 e0                	mov    %esp,%eax
push eax
c0101208:	50                   	push   %eax
mov eax, __irq_handler
c0101209:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c010120e:	ff d0                	call   *%eax
pop eax
c0101210:	58                   	pop    %eax
pop gs
c0101211:	0f a9                	pop    %gs
pop fs
c0101213:	0f a1                	pop    %fs
pop es
c0101215:	07                   	pop    %es
pop ds
c0101216:	1f                   	pop    %ds
popa
c0101217:	61                   	popa   
add esp, 8 
c0101218:	83 c4 08             	add    $0x8,%esp
iret
c010121b:	cf                   	iret   

c010121c <__irq10>:
    cli
c010121c:	fa                   	cli    
    push byte 0
c010121d:	6a 00                	push   $0x0
    push byte %1+32
c010121f:	6a 2a                	push   $0x2a
pusha
c0101221:	60                   	pusha  
push ds
c0101222:	1e                   	push   %ds
push es
c0101223:	06                   	push   %es
push fs
c0101224:	0f a0                	push   %fs
push gs
c0101226:	0f a8                	push   %gs
mov ax, 0x10
c0101228:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c010122c:	8e d8                	mov    %eax,%ds
mov es, ax
c010122e:	8e c0                	mov    %eax,%es
mov fs, ax
c0101230:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101232:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101234:	89 e0                	mov    %esp,%eax
push eax
c0101236:	50                   	push   %eax
mov eax, __irq_handler
c0101237:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c010123c:	ff d0                	call   *%eax
pop eax
c010123e:	58                   	pop    %eax
pop gs
c010123f:	0f a9                	pop    %gs
pop fs
c0101241:	0f a1                	pop    %fs
pop es
c0101243:	07                   	pop    %es
pop ds
c0101244:	1f                   	pop    %ds
popa
c0101245:	61                   	popa   
add esp, 8 
c0101246:	83 c4 08             	add    $0x8,%esp
iret
c0101249:	cf                   	iret   

c010124a <__irq11>:
    cli
c010124a:	fa                   	cli    
    push byte 0
c010124b:	6a 00                	push   $0x0
    push byte %1+32
c010124d:	6a 2b                	push   $0x2b
pusha
c010124f:	60                   	pusha  
push ds
c0101250:	1e                   	push   %ds
push es
c0101251:	06                   	push   %es
push fs
c0101252:	0f a0                	push   %fs
push gs
c0101254:	0f a8                	push   %gs
mov ax, 0x10
c0101256:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c010125a:	8e d8                	mov    %eax,%ds
mov es, ax
c010125c:	8e c0                	mov    %eax,%es
mov fs, ax
c010125e:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101260:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101262:	89 e0                	mov    %esp,%eax
push eax
c0101264:	50                   	push   %eax
mov eax, __irq_handler
c0101265:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c010126a:	ff d0                	call   *%eax
pop eax
c010126c:	58                   	pop    %eax
pop gs
c010126d:	0f a9                	pop    %gs
pop fs
c010126f:	0f a1                	pop    %fs
pop es
c0101271:	07                   	pop    %es
pop ds
c0101272:	1f                   	pop    %ds
popa
c0101273:	61                   	popa   
add esp, 8 
c0101274:	83 c4 08             	add    $0x8,%esp
iret
c0101277:	cf                   	iret   

c0101278 <__irq12>:
    cli
c0101278:	fa                   	cli    
    push byte 0
c0101279:	6a 00                	push   $0x0
    push byte %1+32
c010127b:	6a 2c                	push   $0x2c
pusha
c010127d:	60                   	pusha  
push ds
c010127e:	1e                   	push   %ds
push es
c010127f:	06                   	push   %es
push fs
c0101280:	0f a0                	push   %fs
push gs
c0101282:	0f a8                	push   %gs
mov ax, 0x10
c0101284:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101288:	8e d8                	mov    %eax,%ds
mov es, ax
c010128a:	8e c0                	mov    %eax,%es
mov fs, ax
c010128c:	8e e0                	mov    %eax,%fs
mov gs, ax
c010128e:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101290:	89 e0                	mov    %esp,%eax
push eax
c0101292:	50                   	push   %eax
mov eax, __irq_handler
c0101293:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c0101298:	ff d0                	call   *%eax
pop eax
c010129a:	58                   	pop    %eax
pop gs
c010129b:	0f a9                	pop    %gs
pop fs
c010129d:	0f a1                	pop    %fs
pop es
c010129f:	07                   	pop    %es
pop ds
c01012a0:	1f                   	pop    %ds
popa
c01012a1:	61                   	popa   
add esp, 8 
c01012a2:	83 c4 08             	add    $0x8,%esp
iret
c01012a5:	cf                   	iret   

c01012a6 <__irq13>:
    cli
c01012a6:	fa                   	cli    
    push byte 0
c01012a7:	6a 00                	push   $0x0
    push byte %1+32
c01012a9:	6a 2d                	push   $0x2d
pusha
c01012ab:	60                   	pusha  
push ds
c01012ac:	1e                   	push   %ds
push es
c01012ad:	06                   	push   %es
push fs
c01012ae:	0f a0                	push   %fs
push gs
c01012b0:	0f a8                	push   %gs
mov ax, 0x10
c01012b2:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01012b6:	8e d8                	mov    %eax,%ds
mov es, ax
c01012b8:	8e c0                	mov    %eax,%es
mov fs, ax
c01012ba:	8e e0                	mov    %eax,%fs
mov gs, ax
c01012bc:	8e e8                	mov    %eax,%gs
mov eax, esp
c01012be:	89 e0                	mov    %esp,%eax
push eax
c01012c0:	50                   	push   %eax
mov eax, __irq_handler
c01012c1:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c01012c6:	ff d0                	call   *%eax
pop eax
c01012c8:	58                   	pop    %eax
pop gs
c01012c9:	0f a9                	pop    %gs
pop fs
c01012cb:	0f a1                	pop    %fs
pop es
c01012cd:	07                   	pop    %es
pop ds
c01012ce:	1f                   	pop    %ds
popa
c01012cf:	61                   	popa   
add esp, 8 
c01012d0:	83 c4 08             	add    $0x8,%esp
iret
c01012d3:	cf                   	iret   

c01012d4 <__irq14>:
    cli
c01012d4:	fa                   	cli    
    push byte 0
c01012d5:	6a 00                	push   $0x0
    push byte %1+32
c01012d7:	6a 2e                	push   $0x2e
pusha
c01012d9:	60                   	pusha  
push ds
c01012da:	1e                   	push   %ds
push es
c01012db:	06                   	push   %es
push fs
c01012dc:	0f a0                	push   %fs
push gs
c01012de:	0f a8                	push   %gs
mov ax, 0x10
c01012e0:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01012e4:	8e d8                	mov    %eax,%ds
mov es, ax
c01012e6:	8e c0                	mov    %eax,%es
mov fs, ax
c01012e8:	8e e0                	mov    %eax,%fs
mov gs, ax
c01012ea:	8e e8                	mov    %eax,%gs
mov eax, esp
c01012ec:	89 e0                	mov    %esp,%eax
push eax
c01012ee:	50                   	push   %eax
mov eax, __irq_handler
c01012ef:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c01012f4:	ff d0                	call   *%eax
pop eax
c01012f6:	58                   	pop    %eax
pop gs
c01012f7:	0f a9                	pop    %gs
pop fs
c01012f9:	0f a1                	pop    %fs
pop es
c01012fb:	07                   	pop    %es
pop ds
c01012fc:	1f                   	pop    %ds
popa
c01012fd:	61                   	popa   
add esp, 8 
c01012fe:	83 c4 08             	add    $0x8,%esp
iret
c0101301:	cf                   	iret   

c0101302 <__irq15>:
    cli
c0101302:	fa                   	cli    
    push byte 0
c0101303:	6a 00                	push   $0x0
    push byte %1+32
c0101305:	6a 2f                	push   $0x2f
pusha
c0101307:	60                   	pusha  
push ds
c0101308:	1e                   	push   %ds
push es
c0101309:	06                   	push   %es
push fs
c010130a:	0f a0                	push   %fs
push gs
c010130c:	0f a8                	push   %gs
mov ax, 0x10
c010130e:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101312:	8e d8                	mov    %eax,%ds
mov es, ax
c0101314:	8e c0                	mov    %eax,%es
mov fs, ax
c0101316:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101318:	8e e8                	mov    %eax,%gs
mov eax, esp
c010131a:	89 e0                	mov    %esp,%eax
push eax
c010131c:	50                   	push   %eax
mov eax, __irq_handler
c010131d:	b8 2b 1d 10 c0       	mov    $0xc0101d2b,%eax
call eax
c0101322:	ff d0                	call   *%eax
pop eax
c0101324:	58                   	pop    %eax
pop gs
c0101325:	0f a9                	pop    %gs
pop fs
c0101327:	0f a1                	pop    %fs
pop es
c0101329:	07                   	pop    %es
pop ds
c010132a:	1f                   	pop    %ds
popa
c010132b:	61                   	popa   
add esp, 8 
c010132c:	83 c4 08             	add    $0x8,%esp
iret
c010132f:	cf                   	iret   

c0101330 <__isr0>:
[bits 32]

%macro DECLISR 2
global __isr %+ %1
__isr %+ %1:
    cli
c0101330:	fa                   	cli    
    %if !(%2)
    push byte 0
c0101331:	6a 00                	push   $0x0
    %endif
    push byte %1
c0101333:	6a 00                	push   $0x0
    ISRCOMMON

%endmacro

%macro ISRCOMMON 0
pusha
c0101335:	60                   	pusha  
push ds
c0101336:	1e                   	push   %ds
push es
c0101337:	06                   	push   %es
push fs
c0101338:	0f a0                	push   %fs
push gs
c010133a:	0f a8                	push   %gs
mov ax, 0x10
c010133c:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101340:	8e d8                	mov    %eax,%ds
mov es, ax
c0101342:	8e c0                	mov    %eax,%es
mov fs, ax
c0101344:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101346:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101348:	89 e0                	mov    %esp,%eax
push eax
c010134a:	50                   	push   %eax
mov eax, __fault_handler
c010134b:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101350:	ff d0                	call   *%eax
pop eax
c0101352:	58                   	pop    %eax
pop gs
c0101353:	0f a9                	pop    %gs
pop fs
c0101355:	0f a1                	pop    %fs
pop es
c0101357:	07                   	pop    %es
pop ds
c0101358:	1f                   	pop    %ds
popa
c0101359:	61                   	popa   
add esp, 8 
c010135a:	83 c4 08             	add    $0x8,%esp
iret
c010135d:	cf                   	iret   

c010135e <__isr1>:
    cli
c010135e:	fa                   	cli    
    push byte 0
c010135f:	6a 00                	push   $0x0
    push byte %1
c0101361:	6a 01                	push   $0x1
pusha
c0101363:	60                   	pusha  
push ds
c0101364:	1e                   	push   %ds
push es
c0101365:	06                   	push   %es
push fs
c0101366:	0f a0                	push   %fs
push gs
c0101368:	0f a8                	push   %gs
mov ax, 0x10
c010136a:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c010136e:	8e d8                	mov    %eax,%ds
mov es, ax
c0101370:	8e c0                	mov    %eax,%es
mov fs, ax
c0101372:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101374:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101376:	89 e0                	mov    %esp,%eax
push eax
c0101378:	50                   	push   %eax
mov eax, __fault_handler
c0101379:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c010137e:	ff d0                	call   *%eax
pop eax
c0101380:	58                   	pop    %eax
pop gs
c0101381:	0f a9                	pop    %gs
pop fs
c0101383:	0f a1                	pop    %fs
pop es
c0101385:	07                   	pop    %es
pop ds
c0101386:	1f                   	pop    %ds
popa
c0101387:	61                   	popa   
add esp, 8 
c0101388:	83 c4 08             	add    $0x8,%esp
iret
c010138b:	cf                   	iret   

c010138c <__isr2>:
    cli
c010138c:	fa                   	cli    
    push byte 0
c010138d:	6a 00                	push   $0x0
    push byte %1
c010138f:	6a 02                	push   $0x2
pusha
c0101391:	60                   	pusha  
push ds
c0101392:	1e                   	push   %ds
push es
c0101393:	06                   	push   %es
push fs
c0101394:	0f a0                	push   %fs
push gs
c0101396:	0f a8                	push   %gs
mov ax, 0x10
c0101398:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c010139c:	8e d8                	mov    %eax,%ds
mov es, ax
c010139e:	8e c0                	mov    %eax,%es
mov fs, ax
c01013a0:	8e e0                	mov    %eax,%fs
mov gs, ax
c01013a2:	8e e8                	mov    %eax,%gs
mov eax, esp
c01013a4:	89 e0                	mov    %esp,%eax
push eax
c01013a6:	50                   	push   %eax
mov eax, __fault_handler
c01013a7:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c01013ac:	ff d0                	call   *%eax
pop eax
c01013ae:	58                   	pop    %eax
pop gs
c01013af:	0f a9                	pop    %gs
pop fs
c01013b1:	0f a1                	pop    %fs
pop es
c01013b3:	07                   	pop    %es
pop ds
c01013b4:	1f                   	pop    %ds
popa
c01013b5:	61                   	popa   
add esp, 8 
c01013b6:	83 c4 08             	add    $0x8,%esp
iret
c01013b9:	cf                   	iret   

c01013ba <__isr3>:
    cli
c01013ba:	fa                   	cli    
    push byte 0
c01013bb:	6a 00                	push   $0x0
    push byte %1
c01013bd:	6a 03                	push   $0x3
pusha
c01013bf:	60                   	pusha  
push ds
c01013c0:	1e                   	push   %ds
push es
c01013c1:	06                   	push   %es
push fs
c01013c2:	0f a0                	push   %fs
push gs
c01013c4:	0f a8                	push   %gs
mov ax, 0x10
c01013c6:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01013ca:	8e d8                	mov    %eax,%ds
mov es, ax
c01013cc:	8e c0                	mov    %eax,%es
mov fs, ax
c01013ce:	8e e0                	mov    %eax,%fs
mov gs, ax
c01013d0:	8e e8                	mov    %eax,%gs
mov eax, esp
c01013d2:	89 e0                	mov    %esp,%eax
push eax
c01013d4:	50                   	push   %eax
mov eax, __fault_handler
c01013d5:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c01013da:	ff d0                	call   *%eax
pop eax
c01013dc:	58                   	pop    %eax
pop gs
c01013dd:	0f a9                	pop    %gs
pop fs
c01013df:	0f a1                	pop    %fs
pop es
c01013e1:	07                   	pop    %es
pop ds
c01013e2:	1f                   	pop    %ds
popa
c01013e3:	61                   	popa   
add esp, 8 
c01013e4:	83 c4 08             	add    $0x8,%esp
iret
c01013e7:	cf                   	iret   

c01013e8 <__isr4>:
    cli
c01013e8:	fa                   	cli    
    push byte 0
c01013e9:	6a 00                	push   $0x0
    push byte %1
c01013eb:	6a 04                	push   $0x4
pusha
c01013ed:	60                   	pusha  
push ds
c01013ee:	1e                   	push   %ds
push es
c01013ef:	06                   	push   %es
push fs
c01013f0:	0f a0                	push   %fs
push gs
c01013f2:	0f a8                	push   %gs
mov ax, 0x10
c01013f4:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01013f8:	8e d8                	mov    %eax,%ds
mov es, ax
c01013fa:	8e c0                	mov    %eax,%es
mov fs, ax
c01013fc:	8e e0                	mov    %eax,%fs
mov gs, ax
c01013fe:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101400:	89 e0                	mov    %esp,%eax
push eax
c0101402:	50                   	push   %eax
mov eax, __fault_handler
c0101403:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101408:	ff d0                	call   *%eax
pop eax
c010140a:	58                   	pop    %eax
pop gs
c010140b:	0f a9                	pop    %gs
pop fs
c010140d:	0f a1                	pop    %fs
pop es
c010140f:	07                   	pop    %es
pop ds
c0101410:	1f                   	pop    %ds
popa
c0101411:	61                   	popa   
add esp, 8 
c0101412:	83 c4 08             	add    $0x8,%esp
iret
c0101415:	cf                   	iret   

c0101416 <__isr5>:
    cli
c0101416:	fa                   	cli    
    push byte 0
c0101417:	6a 00                	push   $0x0
    push byte %1
c0101419:	6a 05                	push   $0x5
pusha
c010141b:	60                   	pusha  
push ds
c010141c:	1e                   	push   %ds
push es
c010141d:	06                   	push   %es
push fs
c010141e:	0f a0                	push   %fs
push gs
c0101420:	0f a8                	push   %gs
mov ax, 0x10
c0101422:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101426:	8e d8                	mov    %eax,%ds
mov es, ax
c0101428:	8e c0                	mov    %eax,%es
mov fs, ax
c010142a:	8e e0                	mov    %eax,%fs
mov gs, ax
c010142c:	8e e8                	mov    %eax,%gs
mov eax, esp
c010142e:	89 e0                	mov    %esp,%eax
push eax
c0101430:	50                   	push   %eax
mov eax, __fault_handler
c0101431:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101436:	ff d0                	call   *%eax
pop eax
c0101438:	58                   	pop    %eax
pop gs
c0101439:	0f a9                	pop    %gs
pop fs
c010143b:	0f a1                	pop    %fs
pop es
c010143d:	07                   	pop    %es
pop ds
c010143e:	1f                   	pop    %ds
popa
c010143f:	61                   	popa   
add esp, 8 
c0101440:	83 c4 08             	add    $0x8,%esp
iret
c0101443:	cf                   	iret   

c0101444 <__isr6>:
    cli
c0101444:	fa                   	cli    
    push byte 0
c0101445:	6a 00                	push   $0x0
    push byte %1
c0101447:	6a 06                	push   $0x6
pusha
c0101449:	60                   	pusha  
push ds
c010144a:	1e                   	push   %ds
push es
c010144b:	06                   	push   %es
push fs
c010144c:	0f a0                	push   %fs
push gs
c010144e:	0f a8                	push   %gs
mov ax, 0x10
c0101450:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101454:	8e d8                	mov    %eax,%ds
mov es, ax
c0101456:	8e c0                	mov    %eax,%es
mov fs, ax
c0101458:	8e e0                	mov    %eax,%fs
mov gs, ax
c010145a:	8e e8                	mov    %eax,%gs
mov eax, esp
c010145c:	89 e0                	mov    %esp,%eax
push eax
c010145e:	50                   	push   %eax
mov eax, __fault_handler
c010145f:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101464:	ff d0                	call   *%eax
pop eax
c0101466:	58                   	pop    %eax
pop gs
c0101467:	0f a9                	pop    %gs
pop fs
c0101469:	0f a1                	pop    %fs
pop es
c010146b:	07                   	pop    %es
pop ds
c010146c:	1f                   	pop    %ds
popa
c010146d:	61                   	popa   
add esp, 8 
c010146e:	83 c4 08             	add    $0x8,%esp
iret
c0101471:	cf                   	iret   

c0101472 <__isr7>:
    cli
c0101472:	fa                   	cli    
    push byte 0
c0101473:	6a 00                	push   $0x0
    push byte %1
c0101475:	6a 07                	push   $0x7
pusha
c0101477:	60                   	pusha  
push ds
c0101478:	1e                   	push   %ds
push es
c0101479:	06                   	push   %es
push fs
c010147a:	0f a0                	push   %fs
push gs
c010147c:	0f a8                	push   %gs
mov ax, 0x10
c010147e:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101482:	8e d8                	mov    %eax,%ds
mov es, ax
c0101484:	8e c0                	mov    %eax,%es
mov fs, ax
c0101486:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101488:	8e e8                	mov    %eax,%gs
mov eax, esp
c010148a:	89 e0                	mov    %esp,%eax
push eax
c010148c:	50                   	push   %eax
mov eax, __fault_handler
c010148d:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101492:	ff d0                	call   *%eax
pop eax
c0101494:	58                   	pop    %eax
pop gs
c0101495:	0f a9                	pop    %gs
pop fs
c0101497:	0f a1                	pop    %fs
pop es
c0101499:	07                   	pop    %es
pop ds
c010149a:	1f                   	pop    %ds
popa
c010149b:	61                   	popa   
add esp, 8 
c010149c:	83 c4 08             	add    $0x8,%esp
iret
c010149f:	cf                   	iret   

c01014a0 <__isr8>:
    cli
c01014a0:	fa                   	cli    
    push byte %1
c01014a1:	6a 08                	push   $0x8
pusha
c01014a3:	60                   	pusha  
push ds
c01014a4:	1e                   	push   %ds
push es
c01014a5:	06                   	push   %es
push fs
c01014a6:	0f a0                	push   %fs
push gs
c01014a8:	0f a8                	push   %gs
mov ax, 0x10
c01014aa:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01014ae:	8e d8                	mov    %eax,%ds
mov es, ax
c01014b0:	8e c0                	mov    %eax,%es
mov fs, ax
c01014b2:	8e e0                	mov    %eax,%fs
mov gs, ax
c01014b4:	8e e8                	mov    %eax,%gs
mov eax, esp
c01014b6:	89 e0                	mov    %esp,%eax
push eax
c01014b8:	50                   	push   %eax
mov eax, __fault_handler
c01014b9:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c01014be:	ff d0                	call   *%eax
pop eax
c01014c0:	58                   	pop    %eax
pop gs
c01014c1:	0f a9                	pop    %gs
pop fs
c01014c3:	0f a1                	pop    %fs
pop es
c01014c5:	07                   	pop    %es
pop ds
c01014c6:	1f                   	pop    %ds
popa
c01014c7:	61                   	popa   
add esp, 8 
c01014c8:	83 c4 08             	add    $0x8,%esp
iret
c01014cb:	cf                   	iret   

c01014cc <__isr9>:
    cli
c01014cc:	fa                   	cli    
    push byte 0
c01014cd:	6a 00                	push   $0x0
    push byte %1
c01014cf:	6a 09                	push   $0x9
pusha
c01014d1:	60                   	pusha  
push ds
c01014d2:	1e                   	push   %ds
push es
c01014d3:	06                   	push   %es
push fs
c01014d4:	0f a0                	push   %fs
push gs
c01014d6:	0f a8                	push   %gs
mov ax, 0x10
c01014d8:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01014dc:	8e d8                	mov    %eax,%ds
mov es, ax
c01014de:	8e c0                	mov    %eax,%es
mov fs, ax
c01014e0:	8e e0                	mov    %eax,%fs
mov gs, ax
c01014e2:	8e e8                	mov    %eax,%gs
mov eax, esp
c01014e4:	89 e0                	mov    %esp,%eax
push eax
c01014e6:	50                   	push   %eax
mov eax, __fault_handler
c01014e7:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c01014ec:	ff d0                	call   *%eax
pop eax
c01014ee:	58                   	pop    %eax
pop gs
c01014ef:	0f a9                	pop    %gs
pop fs
c01014f1:	0f a1                	pop    %fs
pop es
c01014f3:	07                   	pop    %es
pop ds
c01014f4:	1f                   	pop    %ds
popa
c01014f5:	61                   	popa   
add esp, 8 
c01014f6:	83 c4 08             	add    $0x8,%esp
iret
c01014f9:	cf                   	iret   

c01014fa <__isr10>:
    cli
c01014fa:	fa                   	cli    
    push byte %1
c01014fb:	6a 0a                	push   $0xa
pusha
c01014fd:	60                   	pusha  
push ds
c01014fe:	1e                   	push   %ds
push es
c01014ff:	06                   	push   %es
push fs
c0101500:	0f a0                	push   %fs
push gs
c0101502:	0f a8                	push   %gs
mov ax, 0x10
c0101504:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101508:	8e d8                	mov    %eax,%ds
mov es, ax
c010150a:	8e c0                	mov    %eax,%es
mov fs, ax
c010150c:	8e e0                	mov    %eax,%fs
mov gs, ax
c010150e:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101510:	89 e0                	mov    %esp,%eax
push eax
c0101512:	50                   	push   %eax
mov eax, __fault_handler
c0101513:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101518:	ff d0                	call   *%eax
pop eax
c010151a:	58                   	pop    %eax
pop gs
c010151b:	0f a9                	pop    %gs
pop fs
c010151d:	0f a1                	pop    %fs
pop es
c010151f:	07                   	pop    %es
pop ds
c0101520:	1f                   	pop    %ds
popa
c0101521:	61                   	popa   
add esp, 8 
c0101522:	83 c4 08             	add    $0x8,%esp
iret
c0101525:	cf                   	iret   

c0101526 <__isr11>:
    cli
c0101526:	fa                   	cli    
    push byte %1
c0101527:	6a 0b                	push   $0xb
pusha
c0101529:	60                   	pusha  
push ds
c010152a:	1e                   	push   %ds
push es
c010152b:	06                   	push   %es
push fs
c010152c:	0f a0                	push   %fs
push gs
c010152e:	0f a8                	push   %gs
mov ax, 0x10
c0101530:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101534:	8e d8                	mov    %eax,%ds
mov es, ax
c0101536:	8e c0                	mov    %eax,%es
mov fs, ax
c0101538:	8e e0                	mov    %eax,%fs
mov gs, ax
c010153a:	8e e8                	mov    %eax,%gs
mov eax, esp
c010153c:	89 e0                	mov    %esp,%eax
push eax
c010153e:	50                   	push   %eax
mov eax, __fault_handler
c010153f:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101544:	ff d0                	call   *%eax
pop eax
c0101546:	58                   	pop    %eax
pop gs
c0101547:	0f a9                	pop    %gs
pop fs
c0101549:	0f a1                	pop    %fs
pop es
c010154b:	07                   	pop    %es
pop ds
c010154c:	1f                   	pop    %ds
popa
c010154d:	61                   	popa   
add esp, 8 
c010154e:	83 c4 08             	add    $0x8,%esp
iret
c0101551:	cf                   	iret   

c0101552 <__isr12>:
    cli
c0101552:	fa                   	cli    
    push byte %1
c0101553:	6a 0c                	push   $0xc
pusha
c0101555:	60                   	pusha  
push ds
c0101556:	1e                   	push   %ds
push es
c0101557:	06                   	push   %es
push fs
c0101558:	0f a0                	push   %fs
push gs
c010155a:	0f a8                	push   %gs
mov ax, 0x10
c010155c:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101560:	8e d8                	mov    %eax,%ds
mov es, ax
c0101562:	8e c0                	mov    %eax,%es
mov fs, ax
c0101564:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101566:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101568:	89 e0                	mov    %esp,%eax
push eax
c010156a:	50                   	push   %eax
mov eax, __fault_handler
c010156b:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101570:	ff d0                	call   *%eax
pop eax
c0101572:	58                   	pop    %eax
pop gs
c0101573:	0f a9                	pop    %gs
pop fs
c0101575:	0f a1                	pop    %fs
pop es
c0101577:	07                   	pop    %es
pop ds
c0101578:	1f                   	pop    %ds
popa
c0101579:	61                   	popa   
add esp, 8 
c010157a:	83 c4 08             	add    $0x8,%esp
iret
c010157d:	cf                   	iret   

c010157e <__isr13>:
    cli
c010157e:	fa                   	cli    
    push byte %1
c010157f:	6a 0d                	push   $0xd
pusha
c0101581:	60                   	pusha  
push ds
c0101582:	1e                   	push   %ds
push es
c0101583:	06                   	push   %es
push fs
c0101584:	0f a0                	push   %fs
push gs
c0101586:	0f a8                	push   %gs
mov ax, 0x10
c0101588:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c010158c:	8e d8                	mov    %eax,%ds
mov es, ax
c010158e:	8e c0                	mov    %eax,%es
mov fs, ax
c0101590:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101592:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101594:	89 e0                	mov    %esp,%eax
push eax
c0101596:	50                   	push   %eax
mov eax, __fault_handler
c0101597:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c010159c:	ff d0                	call   *%eax
pop eax
c010159e:	58                   	pop    %eax
pop gs
c010159f:	0f a9                	pop    %gs
pop fs
c01015a1:	0f a1                	pop    %fs
pop es
c01015a3:	07                   	pop    %es
pop ds
c01015a4:	1f                   	pop    %ds
popa
c01015a5:	61                   	popa   
add esp, 8 
c01015a6:	83 c4 08             	add    $0x8,%esp
iret
c01015a9:	cf                   	iret   

c01015aa <__isr14>:
    cli
c01015aa:	fa                   	cli    
    push byte %1
c01015ab:	6a 0e                	push   $0xe
pusha
c01015ad:	60                   	pusha  
push ds
c01015ae:	1e                   	push   %ds
push es
c01015af:	06                   	push   %es
push fs
c01015b0:	0f a0                	push   %fs
push gs
c01015b2:	0f a8                	push   %gs
mov ax, 0x10
c01015b4:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01015b8:	8e d8                	mov    %eax,%ds
mov es, ax
c01015ba:	8e c0                	mov    %eax,%es
mov fs, ax
c01015bc:	8e e0                	mov    %eax,%fs
mov gs, ax
c01015be:	8e e8                	mov    %eax,%gs
mov eax, esp
c01015c0:	89 e0                	mov    %esp,%eax
push eax
c01015c2:	50                   	push   %eax
mov eax, __fault_handler
c01015c3:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c01015c8:	ff d0                	call   *%eax
pop eax
c01015ca:	58                   	pop    %eax
pop gs
c01015cb:	0f a9                	pop    %gs
pop fs
c01015cd:	0f a1                	pop    %fs
pop es
c01015cf:	07                   	pop    %es
pop ds
c01015d0:	1f                   	pop    %ds
popa
c01015d1:	61                   	popa   
add esp, 8 
c01015d2:	83 c4 08             	add    $0x8,%esp
iret
c01015d5:	cf                   	iret   

c01015d6 <__isr15>:
    cli
c01015d6:	fa                   	cli    
    push byte 0
c01015d7:	6a 00                	push   $0x0
    push byte %1
c01015d9:	6a 0f                	push   $0xf
pusha
c01015db:	60                   	pusha  
push ds
c01015dc:	1e                   	push   %ds
push es
c01015dd:	06                   	push   %es
push fs
c01015de:	0f a0                	push   %fs
push gs
c01015e0:	0f a8                	push   %gs
mov ax, 0x10
c01015e2:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01015e6:	8e d8                	mov    %eax,%ds
mov es, ax
c01015e8:	8e c0                	mov    %eax,%es
mov fs, ax
c01015ea:	8e e0                	mov    %eax,%fs
mov gs, ax
c01015ec:	8e e8                	mov    %eax,%gs
mov eax, esp
c01015ee:	89 e0                	mov    %esp,%eax
push eax
c01015f0:	50                   	push   %eax
mov eax, __fault_handler
c01015f1:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c01015f6:	ff d0                	call   *%eax
pop eax
c01015f8:	58                   	pop    %eax
pop gs
c01015f9:	0f a9                	pop    %gs
pop fs
c01015fb:	0f a1                	pop    %fs
pop es
c01015fd:	07                   	pop    %es
pop ds
c01015fe:	1f                   	pop    %ds
popa
c01015ff:	61                   	popa   
add esp, 8 
c0101600:	83 c4 08             	add    $0x8,%esp
iret
c0101603:	cf                   	iret   

c0101604 <__isr16>:
    cli
c0101604:	fa                   	cli    
    push byte 0
c0101605:	6a 00                	push   $0x0
    push byte %1
c0101607:	6a 10                	push   $0x10
pusha
c0101609:	60                   	pusha  
push ds
c010160a:	1e                   	push   %ds
push es
c010160b:	06                   	push   %es
push fs
c010160c:	0f a0                	push   %fs
push gs
c010160e:	0f a8                	push   %gs
mov ax, 0x10
c0101610:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101614:	8e d8                	mov    %eax,%ds
mov es, ax
c0101616:	8e c0                	mov    %eax,%es
mov fs, ax
c0101618:	8e e0                	mov    %eax,%fs
mov gs, ax
c010161a:	8e e8                	mov    %eax,%gs
mov eax, esp
c010161c:	89 e0                	mov    %esp,%eax
push eax
c010161e:	50                   	push   %eax
mov eax, __fault_handler
c010161f:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101624:	ff d0                	call   *%eax
pop eax
c0101626:	58                   	pop    %eax
pop gs
c0101627:	0f a9                	pop    %gs
pop fs
c0101629:	0f a1                	pop    %fs
pop es
c010162b:	07                   	pop    %es
pop ds
c010162c:	1f                   	pop    %ds
popa
c010162d:	61                   	popa   
add esp, 8 
c010162e:	83 c4 08             	add    $0x8,%esp
iret
c0101631:	cf                   	iret   

c0101632 <__isr17>:
    cli
c0101632:	fa                   	cli    
    push byte 0
c0101633:	6a 00                	push   $0x0
    push byte %1
c0101635:	6a 11                	push   $0x11
pusha
c0101637:	60                   	pusha  
push ds
c0101638:	1e                   	push   %ds
push es
c0101639:	06                   	push   %es
push fs
c010163a:	0f a0                	push   %fs
push gs
c010163c:	0f a8                	push   %gs
mov ax, 0x10
c010163e:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101642:	8e d8                	mov    %eax,%ds
mov es, ax
c0101644:	8e c0                	mov    %eax,%es
mov fs, ax
c0101646:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101648:	8e e8                	mov    %eax,%gs
mov eax, esp
c010164a:	89 e0                	mov    %esp,%eax
push eax
c010164c:	50                   	push   %eax
mov eax, __fault_handler
c010164d:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101652:	ff d0                	call   *%eax
pop eax
c0101654:	58                   	pop    %eax
pop gs
c0101655:	0f a9                	pop    %gs
pop fs
c0101657:	0f a1                	pop    %fs
pop es
c0101659:	07                   	pop    %es
pop ds
c010165a:	1f                   	pop    %ds
popa
c010165b:	61                   	popa   
add esp, 8 
c010165c:	83 c4 08             	add    $0x8,%esp
iret
c010165f:	cf                   	iret   

c0101660 <__isr18>:
    cli
c0101660:	fa                   	cli    
    push byte 0
c0101661:	6a 00                	push   $0x0
    push byte %1
c0101663:	6a 12                	push   $0x12
pusha
c0101665:	60                   	pusha  
push ds
c0101666:	1e                   	push   %ds
push es
c0101667:	06                   	push   %es
push fs
c0101668:	0f a0                	push   %fs
push gs
c010166a:	0f a8                	push   %gs
mov ax, 0x10
c010166c:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101670:	8e d8                	mov    %eax,%ds
mov es, ax
c0101672:	8e c0                	mov    %eax,%es
mov fs, ax
c0101674:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101676:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101678:	89 e0                	mov    %esp,%eax
push eax
c010167a:	50                   	push   %eax
mov eax, __fault_handler
c010167b:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101680:	ff d0                	call   *%eax
pop eax
c0101682:	58                   	pop    %eax
pop gs
c0101683:	0f a9                	pop    %gs
pop fs
c0101685:	0f a1                	pop    %fs
pop es
c0101687:	07                   	pop    %es
pop ds
c0101688:	1f                   	pop    %ds
popa
c0101689:	61                   	popa   
add esp, 8 
c010168a:	83 c4 08             	add    $0x8,%esp
iret
c010168d:	cf                   	iret   

c010168e <__isr19>:
    cli
c010168e:	fa                   	cli    
    push byte 0
c010168f:	6a 00                	push   $0x0
    push byte %1
c0101691:	6a 13                	push   $0x13
pusha
c0101693:	60                   	pusha  
push ds
c0101694:	1e                   	push   %ds
push es
c0101695:	06                   	push   %es
push fs
c0101696:	0f a0                	push   %fs
push gs
c0101698:	0f a8                	push   %gs
mov ax, 0x10
c010169a:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c010169e:	8e d8                	mov    %eax,%ds
mov es, ax
c01016a0:	8e c0                	mov    %eax,%es
mov fs, ax
c01016a2:	8e e0                	mov    %eax,%fs
mov gs, ax
c01016a4:	8e e8                	mov    %eax,%gs
mov eax, esp
c01016a6:	89 e0                	mov    %esp,%eax
push eax
c01016a8:	50                   	push   %eax
mov eax, __fault_handler
c01016a9:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c01016ae:	ff d0                	call   *%eax
pop eax
c01016b0:	58                   	pop    %eax
pop gs
c01016b1:	0f a9                	pop    %gs
pop fs
c01016b3:	0f a1                	pop    %fs
pop es
c01016b5:	07                   	pop    %es
pop ds
c01016b6:	1f                   	pop    %ds
popa
c01016b7:	61                   	popa   
add esp, 8 
c01016b8:	83 c4 08             	add    $0x8,%esp
iret
c01016bb:	cf                   	iret   

c01016bc <__isr20>:
    cli
c01016bc:	fa                   	cli    
    push byte 0
c01016bd:	6a 00                	push   $0x0
    push byte %1
c01016bf:	6a 14                	push   $0x14
pusha
c01016c1:	60                   	pusha  
push ds
c01016c2:	1e                   	push   %ds
push es
c01016c3:	06                   	push   %es
push fs
c01016c4:	0f a0                	push   %fs
push gs
c01016c6:	0f a8                	push   %gs
mov ax, 0x10
c01016c8:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01016cc:	8e d8                	mov    %eax,%ds
mov es, ax
c01016ce:	8e c0                	mov    %eax,%es
mov fs, ax
c01016d0:	8e e0                	mov    %eax,%fs
mov gs, ax
c01016d2:	8e e8                	mov    %eax,%gs
mov eax, esp
c01016d4:	89 e0                	mov    %esp,%eax
push eax
c01016d6:	50                   	push   %eax
mov eax, __fault_handler
c01016d7:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c01016dc:	ff d0                	call   *%eax
pop eax
c01016de:	58                   	pop    %eax
pop gs
c01016df:	0f a9                	pop    %gs
pop fs
c01016e1:	0f a1                	pop    %fs
pop es
c01016e3:	07                   	pop    %es
pop ds
c01016e4:	1f                   	pop    %ds
popa
c01016e5:	61                   	popa   
add esp, 8 
c01016e6:	83 c4 08             	add    $0x8,%esp
iret
c01016e9:	cf                   	iret   

c01016ea <__isr21>:
    cli
c01016ea:	fa                   	cli    
    push byte 0
c01016eb:	6a 00                	push   $0x0
    push byte %1
c01016ed:	6a 15                	push   $0x15
pusha
c01016ef:	60                   	pusha  
push ds
c01016f0:	1e                   	push   %ds
push es
c01016f1:	06                   	push   %es
push fs
c01016f2:	0f a0                	push   %fs
push gs
c01016f4:	0f a8                	push   %gs
mov ax, 0x10
c01016f6:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01016fa:	8e d8                	mov    %eax,%ds
mov es, ax
c01016fc:	8e c0                	mov    %eax,%es
mov fs, ax
c01016fe:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101700:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101702:	89 e0                	mov    %esp,%eax
push eax
c0101704:	50                   	push   %eax
mov eax, __fault_handler
c0101705:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c010170a:	ff d0                	call   *%eax
pop eax
c010170c:	58                   	pop    %eax
pop gs
c010170d:	0f a9                	pop    %gs
pop fs
c010170f:	0f a1                	pop    %fs
pop es
c0101711:	07                   	pop    %es
pop ds
c0101712:	1f                   	pop    %ds
popa
c0101713:	61                   	popa   
add esp, 8 
c0101714:	83 c4 08             	add    $0x8,%esp
iret
c0101717:	cf                   	iret   

c0101718 <__isr22>:
    cli
c0101718:	fa                   	cli    
    push byte 0
c0101719:	6a 00                	push   $0x0
    push byte %1
c010171b:	6a 16                	push   $0x16
pusha
c010171d:	60                   	pusha  
push ds
c010171e:	1e                   	push   %ds
push es
c010171f:	06                   	push   %es
push fs
c0101720:	0f a0                	push   %fs
push gs
c0101722:	0f a8                	push   %gs
mov ax, 0x10
c0101724:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101728:	8e d8                	mov    %eax,%ds
mov es, ax
c010172a:	8e c0                	mov    %eax,%es
mov fs, ax
c010172c:	8e e0                	mov    %eax,%fs
mov gs, ax
c010172e:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101730:	89 e0                	mov    %esp,%eax
push eax
c0101732:	50                   	push   %eax
mov eax, __fault_handler
c0101733:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101738:	ff d0                	call   *%eax
pop eax
c010173a:	58                   	pop    %eax
pop gs
c010173b:	0f a9                	pop    %gs
pop fs
c010173d:	0f a1                	pop    %fs
pop es
c010173f:	07                   	pop    %es
pop ds
c0101740:	1f                   	pop    %ds
popa
c0101741:	61                   	popa   
add esp, 8 
c0101742:	83 c4 08             	add    $0x8,%esp
iret
c0101745:	cf                   	iret   

c0101746 <__isr23>:
    cli
c0101746:	fa                   	cli    
    push byte 0
c0101747:	6a 00                	push   $0x0
    push byte %1
c0101749:	6a 17                	push   $0x17
pusha
c010174b:	60                   	pusha  
push ds
c010174c:	1e                   	push   %ds
push es
c010174d:	06                   	push   %es
push fs
c010174e:	0f a0                	push   %fs
push gs
c0101750:	0f a8                	push   %gs
mov ax, 0x10
c0101752:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101756:	8e d8                	mov    %eax,%ds
mov es, ax
c0101758:	8e c0                	mov    %eax,%es
mov fs, ax
c010175a:	8e e0                	mov    %eax,%fs
mov gs, ax
c010175c:	8e e8                	mov    %eax,%gs
mov eax, esp
c010175e:	89 e0                	mov    %esp,%eax
push eax
c0101760:	50                   	push   %eax
mov eax, __fault_handler
c0101761:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101766:	ff d0                	call   *%eax
pop eax
c0101768:	58                   	pop    %eax
pop gs
c0101769:	0f a9                	pop    %gs
pop fs
c010176b:	0f a1                	pop    %fs
pop es
c010176d:	07                   	pop    %es
pop ds
c010176e:	1f                   	pop    %ds
popa
c010176f:	61                   	popa   
add esp, 8 
c0101770:	83 c4 08             	add    $0x8,%esp
iret
c0101773:	cf                   	iret   

c0101774 <__isr24>:
    cli
c0101774:	fa                   	cli    
    push byte 0
c0101775:	6a 00                	push   $0x0
    push byte %1
c0101777:	6a 18                	push   $0x18
pusha
c0101779:	60                   	pusha  
push ds
c010177a:	1e                   	push   %ds
push es
c010177b:	06                   	push   %es
push fs
c010177c:	0f a0                	push   %fs
push gs
c010177e:	0f a8                	push   %gs
mov ax, 0x10
c0101780:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101784:	8e d8                	mov    %eax,%ds
mov es, ax
c0101786:	8e c0                	mov    %eax,%es
mov fs, ax
c0101788:	8e e0                	mov    %eax,%fs
mov gs, ax
c010178a:	8e e8                	mov    %eax,%gs
mov eax, esp
c010178c:	89 e0                	mov    %esp,%eax
push eax
c010178e:	50                   	push   %eax
mov eax, __fault_handler
c010178f:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c0101794:	ff d0                	call   *%eax
pop eax
c0101796:	58                   	pop    %eax
pop gs
c0101797:	0f a9                	pop    %gs
pop fs
c0101799:	0f a1                	pop    %fs
pop es
c010179b:	07                   	pop    %es
pop ds
c010179c:	1f                   	pop    %ds
popa
c010179d:	61                   	popa   
add esp, 8 
c010179e:	83 c4 08             	add    $0x8,%esp
iret
c01017a1:	cf                   	iret   

c01017a2 <__isr25>:
    cli
c01017a2:	fa                   	cli    
    push byte 0
c01017a3:	6a 00                	push   $0x0
    push byte %1
c01017a5:	6a 19                	push   $0x19
pusha
c01017a7:	60                   	pusha  
push ds
c01017a8:	1e                   	push   %ds
push es
c01017a9:	06                   	push   %es
push fs
c01017aa:	0f a0                	push   %fs
push gs
c01017ac:	0f a8                	push   %gs
mov ax, 0x10
c01017ae:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01017b2:	8e d8                	mov    %eax,%ds
mov es, ax
c01017b4:	8e c0                	mov    %eax,%es
mov fs, ax
c01017b6:	8e e0                	mov    %eax,%fs
mov gs, ax
c01017b8:	8e e8                	mov    %eax,%gs
mov eax, esp
c01017ba:	89 e0                	mov    %esp,%eax
push eax
c01017bc:	50                   	push   %eax
mov eax, __fault_handler
c01017bd:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c01017c2:	ff d0                	call   *%eax
pop eax
c01017c4:	58                   	pop    %eax
pop gs
c01017c5:	0f a9                	pop    %gs
pop fs
c01017c7:	0f a1                	pop    %fs
pop es
c01017c9:	07                   	pop    %es
pop ds
c01017ca:	1f                   	pop    %ds
popa
c01017cb:	61                   	popa   
add esp, 8 
c01017cc:	83 c4 08             	add    $0x8,%esp
iret
c01017cf:	cf                   	iret   

c01017d0 <__isr26>:
    cli
c01017d0:	fa                   	cli    
    push byte 0
c01017d1:	6a 00                	push   $0x0
    push byte %1
c01017d3:	6a 1a                	push   $0x1a
pusha
c01017d5:	60                   	pusha  
push ds
c01017d6:	1e                   	push   %ds
push es
c01017d7:	06                   	push   %es
push fs
c01017d8:	0f a0                	push   %fs
push gs
c01017da:	0f a8                	push   %gs
mov ax, 0x10
c01017dc:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01017e0:	8e d8                	mov    %eax,%ds
mov es, ax
c01017e2:	8e c0                	mov    %eax,%es
mov fs, ax
c01017e4:	8e e0                	mov    %eax,%fs
mov gs, ax
c01017e6:	8e e8                	mov    %eax,%gs
mov eax, esp
c01017e8:	89 e0                	mov    %esp,%eax
push eax
c01017ea:	50                   	push   %eax
mov eax, __fault_handler
c01017eb:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c01017f0:	ff d0                	call   *%eax
pop eax
c01017f2:	58                   	pop    %eax
pop gs
c01017f3:	0f a9                	pop    %gs
pop fs
c01017f5:	0f a1                	pop    %fs
pop es
c01017f7:	07                   	pop    %es
pop ds
c01017f8:	1f                   	pop    %ds
popa
c01017f9:	61                   	popa   
add esp, 8 
c01017fa:	83 c4 08             	add    $0x8,%esp
iret
c01017fd:	cf                   	iret   

c01017fe <__isr27>:
    cli
c01017fe:	fa                   	cli    
    push byte 0
c01017ff:	6a 00                	push   $0x0
    push byte %1
c0101801:	6a 1b                	push   $0x1b
pusha
c0101803:	60                   	pusha  
push ds
c0101804:	1e                   	push   %ds
push es
c0101805:	06                   	push   %es
push fs
c0101806:	0f a0                	push   %fs
push gs
c0101808:	0f a8                	push   %gs
mov ax, 0x10
c010180a:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c010180e:	8e d8                	mov    %eax,%ds
mov es, ax
c0101810:	8e c0                	mov    %eax,%es
mov fs, ax
c0101812:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101814:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101816:	89 e0                	mov    %esp,%eax
push eax
c0101818:	50                   	push   %eax
mov eax, __fault_handler
c0101819:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c010181e:	ff d0                	call   *%eax
pop eax
c0101820:	58                   	pop    %eax
pop gs
c0101821:	0f a9                	pop    %gs
pop fs
c0101823:	0f a1                	pop    %fs
pop es
c0101825:	07                   	pop    %es
pop ds
c0101826:	1f                   	pop    %ds
popa
c0101827:	61                   	popa   
add esp, 8 
c0101828:	83 c4 08             	add    $0x8,%esp
iret
c010182b:	cf                   	iret   

c010182c <__isr28>:
    cli
c010182c:	fa                   	cli    
    push byte 0
c010182d:	6a 00                	push   $0x0
    push byte %1
c010182f:	6a 1c                	push   $0x1c
pusha
c0101831:	60                   	pusha  
push ds
c0101832:	1e                   	push   %ds
push es
c0101833:	06                   	push   %es
push fs
c0101834:	0f a0                	push   %fs
push gs
c0101836:	0f a8                	push   %gs
mov ax, 0x10
c0101838:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c010183c:	8e d8                	mov    %eax,%ds
mov es, ax
c010183e:	8e c0                	mov    %eax,%es
mov fs, ax
c0101840:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101842:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101844:	89 e0                	mov    %esp,%eax
push eax
c0101846:	50                   	push   %eax
mov eax, __fault_handler
c0101847:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c010184c:	ff d0                	call   *%eax
pop eax
c010184e:	58                   	pop    %eax
pop gs
c010184f:	0f a9                	pop    %gs
pop fs
c0101851:	0f a1                	pop    %fs
pop es
c0101853:	07                   	pop    %es
pop ds
c0101854:	1f                   	pop    %ds
popa
c0101855:	61                   	popa   
add esp, 8 
c0101856:	83 c4 08             	add    $0x8,%esp
iret
c0101859:	cf                   	iret   

c010185a <__isr29>:
    cli
c010185a:	fa                   	cli    
    push byte 0
c010185b:	6a 00                	push   $0x0
    push byte %1
c010185d:	6a 1d                	push   $0x1d
pusha
c010185f:	60                   	pusha  
push ds
c0101860:	1e                   	push   %ds
push es
c0101861:	06                   	push   %es
push fs
c0101862:	0f a0                	push   %fs
push gs
c0101864:	0f a8                	push   %gs
mov ax, 0x10
c0101866:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c010186a:	8e d8                	mov    %eax,%ds
mov es, ax
c010186c:	8e c0                	mov    %eax,%es
mov fs, ax
c010186e:	8e e0                	mov    %eax,%fs
mov gs, ax
c0101870:	8e e8                	mov    %eax,%gs
mov eax, esp
c0101872:	89 e0                	mov    %esp,%eax
push eax
c0101874:	50                   	push   %eax
mov eax, __fault_handler
c0101875:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c010187a:	ff d0                	call   *%eax
pop eax
c010187c:	58                   	pop    %eax
pop gs
c010187d:	0f a9                	pop    %gs
pop fs
c010187f:	0f a1                	pop    %fs
pop es
c0101881:	07                   	pop    %es
pop ds
c0101882:	1f                   	pop    %ds
popa
c0101883:	61                   	popa   
add esp, 8 
c0101884:	83 c4 08             	add    $0x8,%esp
iret
c0101887:	cf                   	iret   

c0101888 <__isr30>:
    cli
c0101888:	fa                   	cli    
    push byte 0
c0101889:	6a 00                	push   $0x0
    push byte %1
c010188b:	6a 1e                	push   $0x1e
pusha
c010188d:	60                   	pusha  
push ds
c010188e:	1e                   	push   %ds
push es
c010188f:	06                   	push   %es
push fs
c0101890:	0f a0                	push   %fs
push gs
c0101892:	0f a8                	push   %gs
mov ax, 0x10
c0101894:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c0101898:	8e d8                	mov    %eax,%ds
mov es, ax
c010189a:	8e c0                	mov    %eax,%es
mov fs, ax
c010189c:	8e e0                	mov    %eax,%fs
mov gs, ax
c010189e:	8e e8                	mov    %eax,%gs
mov eax, esp
c01018a0:	89 e0                	mov    %esp,%eax
push eax
c01018a2:	50                   	push   %eax
mov eax, __fault_handler
c01018a3:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c01018a8:	ff d0                	call   *%eax
pop eax
c01018aa:	58                   	pop    %eax
pop gs
c01018ab:	0f a9                	pop    %gs
pop fs
c01018ad:	0f a1                	pop    %fs
pop es
c01018af:	07                   	pop    %es
pop ds
c01018b0:	1f                   	pop    %ds
popa
c01018b1:	61                   	popa   
add esp, 8 
c01018b2:	83 c4 08             	add    $0x8,%esp
iret
c01018b5:	cf                   	iret   

c01018b6 <__isr31>:
    cli
c01018b6:	fa                   	cli    
    push byte 0
c01018b7:	6a 00                	push   $0x0
    push byte %1
c01018b9:	6a 1f                	push   $0x1f
pusha
c01018bb:	60                   	pusha  
push ds
c01018bc:	1e                   	push   %ds
push es
c01018bd:	06                   	push   %es
push fs
c01018be:	0f a0                	push   %fs
push gs
c01018c0:	0f a8                	push   %gs
mov ax, 0x10
c01018c2:	66 b8 10 00          	mov    $0x10,%ax
mov ds, ax
c01018c6:	8e d8                	mov    %eax,%ds
mov es, ax
c01018c8:	8e c0                	mov    %eax,%es
mov fs, ax
c01018ca:	8e e0                	mov    %eax,%fs
mov gs, ax
c01018cc:	8e e8                	mov    %eax,%gs
mov eax, esp
c01018ce:	89 e0                	mov    %esp,%eax
push eax
c01018d0:	50                   	push   %eax
mov eax, __fault_handler
c01018d1:	b8 4d 20 10 c0       	mov    $0xc010204d,%eax
call eax
c01018d6:	ff d0                	call   *%eax
pop eax
c01018d8:	58                   	pop    %eax
pop gs
c01018d9:	0f a9                	pop    %gs
pop fs
c01018db:	0f a1                	pop    %fs
pop es
c01018dd:	07                   	pop    %es
pop ds
c01018de:	1f                   	pop    %ds
popa
c01018df:	61                   	popa   
add esp, 8 
c01018e0:	83 c4 08             	add    $0x8,%esp
iret
c01018e3:	cf                   	iret   
c01018e4:	66 90                	xchg   %ax,%ax
c01018e6:	66 90                	xchg   %ax,%ax
c01018e8:	66 90                	xchg   %ax,%ax
c01018ea:	66 90                	xchg   %ax,%ax
c01018ec:	66 90                	xchg   %ax,%ax
c01018ee:	66 90                	xchg   %ax,%ax

c01018f0 <__idt_load>:
; http://www.osdever.net/bkerndev/Docs/idt.htm
section .text
global __idt_load
extern __idt_ptr
__idt_load:
    lidt [__idt_ptr]
c01018f0:	0f 01 1d 00 50 10 c0 	lidtl  0xc0105000
c01018f7:	c3                   	ret    
c01018f8:	66 90                	xchg   %ax,%ax
c01018fa:	66 90                	xchg   %ax,%ax
c01018fc:	66 90                	xchg   %ax,%ax
c01018fe:	66 90                	xchg   %ax,%ax

c0101900 <gdt_flush>:
; http://www.osdever.net/bkerndev/Docs/gdt.htm
section .text
global gdt_flush
extern __gdt_ptr
gdt_flush:
    lgdt [__gdt_ptr] ; load special pointer
c0101900:	0f 01 15 6c 58 10 c0 	lgdtl  0xc010586c
    mov ax, 0x10     ; 0x10 is offset for GDT to data
c0101907:	66 b8 10 00          	mov    $0x10,%ax
    mov ds, ax
c010190b:	8e d8                	mov    %eax,%ds
    mov es, ax
c010190d:	8e c0                	mov    %eax,%es
    mov fs, ax
c010190f:	8e e0                	mov    %eax,%fs
    mov gs, ax
c0101911:	8e e8                	mov    %eax,%gs
    mov ss, ax
c0101913:	8e d0                	mov    %eax,%ss
    jmp 0x08:flush2
c0101915:	ea 1c 19 10 c0 08 00 	ljmp   $0x8,$0xc010191c

c010191c <flush2>:

flush2:
    ret
c010191c:	c3                   	ret    
c010191d:	66 90                	xchg   %ax,%ax
c010191f:	90                   	nop

c0101920 <inportb>:

global inportb
global outportb

inportb:
    push edx
c0101920:	52                   	push   %edx
    mov edx, [esp + 8]
c0101921:	8b 54 24 08          	mov    0x8(%esp),%edx
    in ax, dx
c0101925:	66 ed                	in     (%dx),%ax
    pop edx
c0101927:	5a                   	pop    %edx

c0101928 <outportb>:

outportb:
    push edx
c0101928:	52                   	push   %edx
    mov edx, [esp+8]
c0101929:	8b 54 24 08          	mov    0x8(%esp),%edx
    mov eax, [esp+12]
c010192d:	8b 44 24 0c          	mov    0xc(%esp),%eax
    out dx, ax
c0101931:	66 ef                	out    %ax,(%dx)
c0101933:	5a                   	pop    %edx

c0101934 <idt_set_gate>:

extern void __idt_load();


err_t idt_set_gate(uint8_t num, size_t base, uint16_t sel, uint8_t flags)
{
c0101934:	f3 0f 1e fb          	endbr32 
    // Setup the routine's base address
    __idt[num].base_lo = (base & 0xFFFF);
c0101938:	e8 0f 0d 00 00       	call   c010264c <__x86.get_pc_thunk.ax>
c010193d:	05 97 27 00 00       	add    $0x2797,%eax
{
c0101942:	55                   	push   %ebp
c0101943:	89 e5                	mov    %esp,%ebp
c0101945:	8b 55 0c             	mov    0xc(%ebp),%edx
    __idt[num].base_lo = (base & 0xFFFF);
c0101948:	0f b6 4d 08          	movzbl 0x8(%ebp),%ecx
c010194c:	66 89 94 c8 4c 0f 00 	mov    %dx,0xf4c(%eax,%ecx,8)
c0101953:	00 
    __idt[num].base_hi = (base >> 16);
c0101954:	8d 84 c8 4c 0f 00 00 	lea    0xf4c(%eax,%ecx,8),%eax
c010195b:	c1 ea 10             	shr    $0x10,%edx
c010195e:	66 89 50 06          	mov    %dx,0x6(%eax)

    // set the segment 'selector' that the entry will use
    __idt[num].sel = sel;
c0101962:	8b 55 10             	mov    0x10(%ebp),%edx
    __idt[num].reserved = 0;
c0101965:	c6 40 04 00          	movb   $0x0,0x4(%eax)
    __idt[num].sel = sel;
c0101969:	66 89 50 02          	mov    %dx,0x2(%eax)
    __idt[num].flags = flags;
c010196d:	8b 55 14             	mov    0x14(%ebp),%edx
c0101970:	88 50 05             	mov    %dl,0x5(%eax)
    return OS32_SUCCESS;
}
c0101973:	31 c0                	xor    %eax,%eax
c0101975:	5d                   	pop    %ebp
c0101976:	c3                   	ret    

c0101977 <__install_idt>:

err_t __install_idt()
{
c0101977:	f3 0f 1e fb          	endbr32 
c010197b:	55                   	push   %ebp
c010197c:	89 e5                	mov    %esp,%ebp
c010197e:	53                   	push   %ebx
c010197f:	e8 cc 0c 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c0101984:	81 c3 50 27 00 00    	add    $0x2750,%ebx
c010198a:	83 ec 08             	sub    $0x8,%esp
    __idt_ptr.limit = (sizeof(struct idt_entry) * 256) -1;
    __idt_ptr.base = (uint32_t) &__idt;

    memset(&__idt, 0, sizeof(__idt));
c010198d:	68 00 08 00 00       	push   $0x800
c0101992:	6a 00                	push   $0x0
    __idt_ptr.limit = (sizeof(struct idt_entry) * 256) -1;
c0101994:	66 c7 83 2c 0f 00 00 	movw   $0x7ff,0xf2c(%ebx)
c010199b:	ff 07 
    __idt_ptr.base = (uint32_t) &__idt;
c010199d:	8d 83 4c 0f 00 00    	lea    0xf4c(%ebx),%eax
    memset(&__idt, 0, sizeof(__idt));
c01019a3:	50                   	push   %eax
    __idt_ptr.base = (uint32_t) &__idt;
c01019a4:	89 83 2e 0f 00 00    	mov    %eax,0xf2e(%ebx)
    memset(&__idt, 0, sizeof(__idt));
c01019aa:	e8 81 0c 00 00       	call   c0102630 <memset>


    __idt_load();
c01019af:	e8 3c ff ff ff       	call   c01018f0 <__idt_load>
    return OS32_SUCCESS;
}
c01019b4:	8b 5d fc             	mov    -0x4(%ebp),%ebx
c01019b7:	31 c0                	xor    %eax,%eax
c01019b9:	c9                   	leave  
c01019ba:	c3                   	ret    

c01019bb <idt_update>:

void idt_update()
{
c01019bb:	f3 0f 1e fb          	endbr32 
c01019bf:	55                   	push   %ebp
c01019c0:	89 e5                	mov    %esp,%ebp
c01019c2:	53                   	push   %ebx
c01019c3:	e8 88 0c 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c01019c8:	81 c3 0c 27 00 00    	add    $0x270c,%ebx
c01019ce:	50                   	push   %eax
    __idt_load();
c01019cf:	e8 1c ff ff ff       	call   c01018f0 <__idt_load>
c01019d4:	5a                   	pop    %edx
c01019d5:	5b                   	pop    %ebx
c01019d6:	5d                   	pop    %ebp
c01019d7:	c3                   	ret    

c01019d8 <testfn>:

#define __kernel_main_hlt while(1);
#define __kernel_main_sti __asm__ __volatile__ ("sti"); 

void testfn()
{
c01019d8:	f3 0f 1e fb          	endbr32 
c01019dc:	55                   	push   %ebp
c01019dd:	89 e5                	mov    %esp,%ebp
c01019df:	53                   	push   %ebx
c01019e0:	e8 6b 0c 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c01019e5:	81 c3 ef 26 00 00    	add    $0x26ef,%ebx
c01019eb:	83 ec 10             	sub    $0x10,%esp
    vgaPrintf("Hello\n");
c01019ee:	8d 83 18 ea ff ff    	lea    -0x15e8(%ebx),%eax
c01019f4:	50                   	push   %eax
c01019f5:	e8 af 09 00 00       	call   c01023a9 <vgaPrintf>
}
c01019fa:	8b 5d fc             	mov    -0x4(%ebp),%ebx
c01019fd:	83 c4 10             	add    $0x10,%esp
c0101a00:	c9                   	leave  
c0101a01:	c3                   	ret    

c0101a02 <_kernel_main>:

void _kernel_main()
{
c0101a02:	f3 0f 1e fb          	endbr32 
c0101a06:	55                   	push   %ebp
c0101a07:	89 e5                	mov    %esp,%ebp
c0101a09:	53                   	push   %ebx
c0101a0a:	e8 41 0c 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c0101a0f:	81 c3 c5 26 00 00    	add    $0x26c5,%ebx
c0101a15:	83 ec 10             	sub    $0x10,%esp
    vgaPrintf("%+ Entered Kernel Code\n");
c0101a18:	8d 83 1f ea ff ff    	lea    -0x15e1(%ebx),%eax
c0101a1e:	50                   	push   %eax
c0101a1f:	e8 85 09 00 00       	call   c01023a9 <vgaPrintf>
    if ( __install_gdt() == OS32_ERROR )
c0101a24:	e8 76 0b 00 00       	call   c010259f <__install_gdt>
c0101a29:	83 c4 10             	add    $0x10,%esp
c0101a2c:	40                   	inc    %eax
c0101a2d:	75 14                	jne    c0101a43 <_kernel_main+0x41>
    {
        vgaPrintf("%- GDT setup");
c0101a2f:	83 ec 0c             	sub    $0xc,%esp
c0101a32:	8d 83 37 ea ff ff    	lea    -0x15c9(%ebx),%eax
c0101a38:	50                   	push   %eax
c0101a39:	e8 6b 09 00 00       	call   c01023a9 <vgaPrintf>
c0101a3e:	83 c4 10             	add    $0x10,%esp
        __kernel_main_hlt
c0101a41:	eb fe                	jmp    c0101a41 <_kernel_main+0x3f>
    }
    vgaPrintf("%+ GDT Installed\n");
c0101a43:	83 ec 0c             	sub    $0xc,%esp
c0101a46:	8d 83 44 ea ff ff    	lea    -0x15bc(%ebx),%eax
c0101a4c:	50                   	push   %eax
c0101a4d:	e8 57 09 00 00       	call   c01023a9 <vgaPrintf>


    if ( __install_idt() == OS32_ERROR )
c0101a52:	e8 20 ff ff ff       	call   c0101977 <__install_idt>
c0101a57:	83 c4 10             	add    $0x10,%esp
c0101a5a:	40                   	inc    %eax
c0101a5b:	75 14                	jne    c0101a71 <_kernel_main+0x6f>
    {
        vgaPrintf("%- IDT setup");
c0101a5d:	83 ec 0c             	sub    $0xc,%esp
c0101a60:	8d 83 56 ea ff ff    	lea    -0x15aa(%ebx),%eax
c0101a66:	50                   	push   %eax
c0101a67:	e8 3d 09 00 00       	call   c01023a9 <vgaPrintf>
c0101a6c:	83 c4 10             	add    $0x10,%esp
        __kernel_main_hlt
c0101a6f:	eb fe                	jmp    c0101a6f <_kernel_main+0x6d>
    }
    vgaPrintf("%+ IDT Installed\n");
c0101a71:	83 ec 0c             	sub    $0xc,%esp
c0101a74:	8d 83 63 ea ff ff    	lea    -0x159d(%ebx),%eax
c0101a7a:	50                   	push   %eax
c0101a7b:	e8 29 09 00 00       	call   c01023a9 <vgaPrintf>

    if ( __install_isr() == OS32_ERROR )
c0101a80:	e8 00 03 00 00       	call   c0101d85 <__install_isr>
c0101a85:	83 c4 10             	add    $0x10,%esp
c0101a88:	40                   	inc    %eax
c0101a89:	75 14                	jne    c0101a9f <_kernel_main+0x9d>
    {
        vgaPrintf("%- ISRs setup");
c0101a8b:	83 ec 0c             	sub    $0xc,%esp
c0101a8e:	8d 83 75 ea ff ff    	lea    -0x158b(%ebx),%eax
c0101a94:	50                   	push   %eax
c0101a95:	e8 0f 09 00 00       	call   c01023a9 <vgaPrintf>
c0101a9a:	83 c4 10             	add    $0x10,%esp
        __kernel_main_hlt
c0101a9d:	eb fe                	jmp    c0101a9d <_kernel_main+0x9b>
    }
    vgaPrintf("%+ ISRs Installed\n");
c0101a9f:	83 ec 0c             	sub    $0xc,%esp
c0101aa2:	8d 83 83 ea ff ff    	lea    -0x157d(%ebx),%eax
c0101aa8:	50                   	push   %eax
c0101aa9:	e8 fb 08 00 00       	call   c01023a9 <vgaPrintf>

    if ( __install_irq() == OS32_ERROR )
c0101aae:	e8 88 00 00 00       	call   c0101b3b <__install_irq>
c0101ab3:	83 c4 10             	add    $0x10,%esp
c0101ab6:	40                   	inc    %eax
c0101ab7:	75 14                	jne    c0101acd <_kernel_main+0xcb>
    {
        vgaPrintf("%- IRQs setup");
c0101ab9:	83 ec 0c             	sub    $0xc,%esp
c0101abc:	8d 83 96 ea ff ff    	lea    -0x156a(%ebx),%eax
c0101ac2:	50                   	push   %eax
c0101ac3:	e8 e1 08 00 00       	call   c01023a9 <vgaPrintf>
c0101ac8:	83 c4 10             	add    $0x10,%esp
        __kernel_main_hlt
c0101acb:	eb fe                	jmp    c0101acb <_kernel_main+0xc9>
    }
    vgaPrintf("%+ IRQs Installed\n");
c0101acd:	83 ec 0c             	sub    $0xc,%esp
c0101ad0:	8d 83 a4 ea ff ff    	lea    -0x155c(%ebx),%eax
c0101ad6:	50                   	push   %eax
c0101ad7:	e8 cd 08 00 00       	call   c01023a9 <vgaPrintf>

    idt_update();
c0101adc:	e8 da fe ff ff       	call   c01019bb <idt_update>

    __kernel_main_sti
c0101ae1:	fb                   	sti    

    irq_install_routine( 0, (void*)testfn );
c0101ae2:	58                   	pop    %eax
c0101ae3:	8d 83 04 d9 ff ff    	lea    -0x26fc(%ebx),%eax
c0101ae9:	5a                   	pop    %edx
c0101aea:	50                   	push   %eax
c0101aeb:	6a 00                	push   $0x0
c0101aed:	e8 08 00 00 00       	call   c0101afa <irq_install_routine>


}
c0101af2:	8b 5d fc             	mov    -0x4(%ebp),%ebx
c0101af5:	83 c4 10             	add    $0x10,%esp
c0101af8:	c9                   	leave  
c0101af9:	c3                   	ret    

c0101afa <irq_install_routine>:

static irq_routine irq_routines[16] = {NULL};


err_t irq_install_routine( size_t irqno, irq_routine r )
{
c0101afa:	f3 0f 1e fb          	endbr32 
    irq_routines[irqno] = r;
c0101afe:	e8 49 0b 00 00       	call   c010264c <__x86.get_pc_thunk.ax>
c0101b03:	05 d1 25 00 00       	add    $0x25d1,%eax
{
c0101b08:	55                   	push   %ebp
c0101b09:	89 e5                	mov    %esp,%ebp
    irq_routines[irqno] = r;
c0101b0b:	8b 55 08             	mov    0x8(%ebp),%edx
c0101b0e:	8b 4d 0c             	mov    0xc(%ebp),%ecx
}
c0101b11:	5d                   	pop    %ebp
    irq_routines[irqno] = r;
c0101b12:	89 8c 90 4c 17 00 00 	mov    %ecx,0x174c(%eax,%edx,4)
}
c0101b19:	c3                   	ret    

c0101b1a <irq_uninstall_routine>:
err_t irq_uninstall_routine( size_t irqno)
{
c0101b1a:	f3 0f 1e fb          	endbr32 
    irq_routines[irqno] = NULL;
c0101b1e:	e8 29 0b 00 00       	call   c010264c <__x86.get_pc_thunk.ax>
c0101b23:	05 b1 25 00 00       	add    $0x25b1,%eax
{
c0101b28:	55                   	push   %ebp
c0101b29:	89 e5                	mov    %esp,%ebp
    irq_routines[irqno] = NULL;
c0101b2b:	8b 55 08             	mov    0x8(%ebp),%edx
}
c0101b2e:	5d                   	pop    %ebp
    irq_routines[irqno] = NULL;
c0101b2f:	c7 84 90 4c 17 00 00 	movl   $0x0,0x174c(%eax,%edx,4)
c0101b36:	00 00 00 00 
}
c0101b3a:	c3                   	ret    

c0101b3b <__install_irq>:
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

err_t __install_irq()
{
c0101b3b:	f3 0f 1e fb          	endbr32 
c0101b3f:	55                   	push   %ebp
c0101b40:	89 e5                	mov    %esp,%ebp
c0101b42:	53                   	push   %ebx
c0101b43:	e8 08 0b 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c0101b48:	81 c3 8c 25 00 00    	add    $0x258c,%ebx
c0101b4e:	83 ec 0c             	sub    $0xc,%esp
    outportb(0x20, 0x11);
c0101b51:	6a 11                	push   $0x11
c0101b53:	6a 20                	push   $0x20
c0101b55:	e8 ce fd ff ff       	call   c0101928 <outportb>
    outportb(0xA0, 0x11);
c0101b5a:	58                   	pop    %eax
c0101b5b:	5a                   	pop    %edx
c0101b5c:	6a 11                	push   $0x11
c0101b5e:	68 a0 00 00 00       	push   $0xa0
c0101b63:	e8 c0 fd ff ff       	call   c0101928 <outportb>
    outportb(0x21, 0x20);
c0101b68:	59                   	pop    %ecx
c0101b69:	58                   	pop    %eax
c0101b6a:	6a 20                	push   $0x20
c0101b6c:	6a 21                	push   $0x21
c0101b6e:	e8 b5 fd ff ff       	call   c0101928 <outportb>
    outportb(0xA1, 0x28);
c0101b73:	58                   	pop    %eax
c0101b74:	5a                   	pop    %edx
c0101b75:	6a 28                	push   $0x28
c0101b77:	68 a1 00 00 00       	push   $0xa1
c0101b7c:	e8 a7 fd ff ff       	call   c0101928 <outportb>
    outportb(0x21, 0x04);
c0101b81:	59                   	pop    %ecx
c0101b82:	58                   	pop    %eax
c0101b83:	6a 04                	push   $0x4
c0101b85:	6a 21                	push   $0x21
c0101b87:	e8 9c fd ff ff       	call   c0101928 <outportb>
    outportb(0xA1, 0x02);
c0101b8c:	58                   	pop    %eax
c0101b8d:	5a                   	pop    %edx
c0101b8e:	6a 02                	push   $0x2
c0101b90:	68 a1 00 00 00       	push   $0xa1
c0101b95:	e8 8e fd ff ff       	call   c0101928 <outportb>
    outportb(0x21, 0x01);
c0101b9a:	59                   	pop    %ecx
c0101b9b:	58                   	pop    %eax
c0101b9c:	6a 01                	push   $0x1
c0101b9e:	6a 21                	push   $0x21
c0101ba0:	e8 83 fd ff ff       	call   c0101928 <outportb>
    outportb(0xA1, 0x01);
c0101ba5:	58                   	pop    %eax
c0101ba6:	5a                   	pop    %edx
c0101ba7:	6a 01                	push   $0x1
c0101ba9:	68 a1 00 00 00       	push   $0xa1
c0101bae:	e8 75 fd ff ff       	call   c0101928 <outportb>
    outportb(0x21, 0x0);
c0101bb3:	59                   	pop    %ecx
c0101bb4:	58                   	pop    %eax
c0101bb5:	6a 00                	push   $0x0
c0101bb7:	6a 21                	push   $0x21
c0101bb9:	e8 6a fd ff ff       	call   c0101928 <outportb>
    outportb(0xA1, 0x0);
c0101bbe:	58                   	pop    %eax
c0101bbf:	5a                   	pop    %edx
c0101bc0:	6a 00                	push   $0x0
c0101bc2:	68 a1 00 00 00       	push   $0xa1
c0101bc7:	e8 5c fd ff ff       	call   c0101928 <outportb>
    irq_remap();

    idt_set_gate( 32, (size_t)__irq0, 0x08, 0x8E );
c0101bcc:	68 8e 00 00 00       	push   $0x8e
c0101bd1:	6a 08                	push   $0x8
c0101bd3:	ff b3 4c ff ff ff    	push   -0xb4(%ebx)
c0101bd9:	6a 20                	push   $0x20
c0101bdb:	e8 54 fd ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 33, (size_t)__irq1, 0x08, 0x8E );
c0101be0:	83 c4 20             	add    $0x20,%esp
c0101be3:	68 8e 00 00 00       	push   $0x8e
c0101be8:	6a 08                	push   $0x8
c0101bea:	ff b3 80 ff ff ff    	push   -0x80(%ebx)
c0101bf0:	6a 21                	push   $0x21
c0101bf2:	e8 3d fd ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 34, (size_t)__irq2, 0x08, 0x8E );
c0101bf7:	68 8e 00 00 00       	push   $0x8e
c0101bfc:	6a 08                	push   $0x8
c0101bfe:	ff b3 fc ff ff ff    	push   -0x4(%ebx)
c0101c04:	6a 22                	push   $0x22
c0101c06:	e8 29 fd ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 35, (size_t)__irq3, 0x08, 0x8E );
c0101c0b:	83 c4 20             	add    $0x20,%esp
c0101c0e:	68 8e 00 00 00       	push   $0x8e
c0101c13:	6a 08                	push   $0x8
c0101c15:	ff b3 f4 ff ff ff    	push   -0xc(%ebx)
c0101c1b:	6a 23                	push   $0x23
c0101c1d:	e8 12 fd ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 36, (size_t)__irq4, 0x08, 0x8E );
c0101c22:	68 8e 00 00 00       	push   $0x8e
c0101c27:	6a 08                	push   $0x8
c0101c29:	ff b3 74 ff ff ff    	push   -0x8c(%ebx)
c0101c2f:	6a 24                	push   $0x24
c0101c31:	e8 fe fc ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 37, (size_t)__irq5, 0x08, 0x8E );
c0101c36:	83 c4 20             	add    $0x20,%esp
c0101c39:	68 8e 00 00 00       	push   $0x8e
c0101c3e:	6a 08                	push   $0x8
c0101c40:	ff b3 88 ff ff ff    	push   -0x78(%ebx)
c0101c46:	6a 25                	push   $0x25
c0101c48:	e8 e7 fc ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 38, (size_t)__irq6, 0x08, 0x8E );
c0101c4d:	68 8e 00 00 00       	push   $0x8e
c0101c52:	6a 08                	push   $0x8
c0101c54:	ff b3 40 ff ff ff    	push   -0xc0(%ebx)
c0101c5a:	6a 26                	push   $0x26
c0101c5c:	e8 d3 fc ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 39, (size_t)__irq7, 0x08, 0x8E );
c0101c61:	83 c4 20             	add    $0x20,%esp
c0101c64:	68 8e 00 00 00       	push   $0x8e
c0101c69:	6a 08                	push   $0x8
c0101c6b:	ff b3 dc ff ff ff    	push   -0x24(%ebx)
c0101c71:	6a 27                	push   $0x27
c0101c73:	e8 bc fc ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 40, (size_t)__irq8, 0x08, 0x8E );
c0101c78:	68 8e 00 00 00       	push   $0x8e
c0101c7d:	6a 08                	push   $0x8
c0101c7f:	ff b3 60 ff ff ff    	push   -0xa0(%ebx)
c0101c85:	6a 28                	push   $0x28
c0101c87:	e8 a8 fc ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 41, (size_t)__irq9, 0x08, 0x8E );
c0101c8c:	83 c4 20             	add    $0x20,%esp
c0101c8f:	68 8e 00 00 00       	push   $0x8e
c0101c94:	6a 08                	push   $0x8
c0101c96:	ff b3 e4 ff ff ff    	push   -0x1c(%ebx)
c0101c9c:	6a 29                	push   $0x29
c0101c9e:	e8 91 fc ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 42, (size_t)__irq10, 0x08, 0x8E );
c0101ca3:	68 8e 00 00 00       	push   $0x8e
c0101ca8:	6a 08                	push   $0x8
c0101caa:	ff b3 ac ff ff ff    	push   -0x54(%ebx)
c0101cb0:	6a 2a                	push   $0x2a
c0101cb2:	e8 7d fc ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 43, (size_t)__irq11, 0x08, 0x8E );
c0101cb7:	83 c4 20             	add    $0x20,%esp
c0101cba:	68 8e 00 00 00       	push   $0x8e
c0101cbf:	6a 08                	push   $0x8
c0101cc1:	ff b3 64 ff ff ff    	push   -0x9c(%ebx)
c0101cc7:	6a 2b                	push   $0x2b
c0101cc9:	e8 66 fc ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 44, (size_t)__irq12, 0x08, 0x8E );
c0101cce:	68 8e 00 00 00       	push   $0x8e
c0101cd3:	6a 08                	push   $0x8
c0101cd5:	ff b3 c8 ff ff ff    	push   -0x38(%ebx)
c0101cdb:	6a 2c                	push   $0x2c
c0101cdd:	e8 52 fc ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 45, (size_t)__irq13, 0x08, 0x8E );
c0101ce2:	83 c4 20             	add    $0x20,%esp
c0101ce5:	68 8e 00 00 00       	push   $0x8e
c0101cea:	6a 08                	push   $0x8
c0101cec:	ff b3 b4 ff ff ff    	push   -0x4c(%ebx)
c0101cf2:	6a 2d                	push   $0x2d
c0101cf4:	e8 3b fc ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 46, (size_t)__irq14, 0x08, 0x8E );
c0101cf9:	68 8e 00 00 00       	push   $0x8e
c0101cfe:	6a 08                	push   $0x8
c0101d00:	ff b3 7c ff ff ff    	push   -0x84(%ebx)
c0101d06:	6a 2e                	push   $0x2e
c0101d08:	e8 27 fc ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 47, (size_t)__irq15, 0x08, 0x8E );
c0101d0d:	83 c4 20             	add    $0x20,%esp
c0101d10:	68 8e 00 00 00       	push   $0x8e
c0101d15:	6a 08                	push   $0x8
c0101d17:	ff b3 68 ff ff ff    	push   -0x98(%ebx)
c0101d1d:	6a 2f                	push   $0x2f
c0101d1f:	e8 10 fc ff ff       	call   c0101934 <idt_set_gate>
    return OS32_SUCCESS;

}
c0101d24:	8b 5d fc             	mov    -0x4(%ebp),%ebx
c0101d27:	31 c0                	xor    %eax,%eax
c0101d29:	c9                   	leave  
c0101d2a:	c3                   	ret    

c0101d2b <__irq_handler>:

void __irq_handler( interrupt_frame_t* regs )
{
c0101d2b:	f3 0f 1e fb          	endbr32 
c0101d2f:	55                   	push   %ebp
c0101d30:	89 e5                	mov    %esp,%ebp
c0101d32:	56                   	push   %esi
c0101d33:	53                   	push   %ebx
c0101d34:	8b 75 08             	mov    0x8(%ebp),%esi
c0101d37:	e8 14 09 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c0101d3c:	81 c3 98 23 00 00    	add    $0x2398,%ebx

    if ( irq_routines[ regs->intno ] )
c0101d42:	8b 46 30             	mov    0x30(%esi),%eax
c0101d45:	8b 84 83 4c 17 00 00 	mov    0x174c(%ebx,%eax,4),%eax
c0101d4c:	85 c0                	test   %eax,%eax
c0101d4e:	74 09                	je     c0101d59 <__irq_handler+0x2e>
    {
        irq_routines[ regs->intno ](regs);
c0101d50:	83 ec 0c             	sub    $0xc,%esp
c0101d53:	56                   	push   %esi
c0101d54:	ff d0                	call   *%eax
c0101d56:	83 c4 10             	add    $0x10,%esp
    // http://www.osdever.net/bkerndev/Docs/irqs.htm

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (regs->intno >= 40)
c0101d59:	83 7e 30 27          	cmpl   $0x27,0x30(%esi)
c0101d5d:	76 11                	jbe    c0101d70 <__irq_handler+0x45>
    {
        outportb(0xA0, 0x20);
c0101d5f:	52                   	push   %edx
c0101d60:	52                   	push   %edx
c0101d61:	6a 20                	push   $0x20
c0101d63:	68 a0 00 00 00       	push   $0xa0
c0101d68:	e8 bb fb ff ff       	call   c0101928 <outportb>
c0101d6d:	83 c4 10             	add    $0x10,%esp
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outportb(0x20, 0x20);
c0101d70:	50                   	push   %eax
c0101d71:	50                   	push   %eax
c0101d72:	6a 20                	push   $0x20
c0101d74:	6a 20                	push   $0x20
c0101d76:	e8 ad fb ff ff       	call   c0101928 <outportb>

c0101d7b:	83 c4 10             	add    $0x10,%esp
c0101d7e:	8d 65 f8             	lea    -0x8(%ebp),%esp
c0101d81:	5b                   	pop    %ebx
c0101d82:	5e                   	pop    %esi
c0101d83:	5d                   	pop    %ebp
c0101d84:	c3                   	ret    

c0101d85 <__install_isr>:
extern void __isr31();



err_t __install_isr()
{
c0101d85:	f3 0f 1e fb          	endbr32 
c0101d89:	55                   	push   %ebp
c0101d8a:	89 e5                	mov    %esp,%ebp
c0101d8c:	53                   	push   %ebx
c0101d8d:	e8 be 08 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c0101d92:	81 c3 42 23 00 00    	add    $0x2342,%ebx
c0101d98:	50                   	push   %eax

    //          gateno  function   segment, perms + required #14
    idt_set_gate( 0, (size_t)__isr0, 0x08, 0x8E );
c0101d99:	68 8e 00 00 00       	push   $0x8e
c0101d9e:	6a 08                	push   $0x8
c0101da0:	ff b3 d8 ff ff ff    	push   -0x28(%ebx)
c0101da6:	6a 00                	push   $0x0
c0101da8:	e8 87 fb ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 1, (size_t)__isr1, 0x08, 0x8E );
c0101dad:	68 8e 00 00 00       	push   $0x8e
c0101db2:	6a 08                	push   $0x8
c0101db4:	ff b3 44 ff ff ff    	push   -0xbc(%ebx)
c0101dba:	6a 01                	push   $0x1
c0101dbc:	e8 73 fb ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 2, (size_t)__isr2, 0x08, 0x8E );
c0101dc1:	83 c4 20             	add    $0x20,%esp
c0101dc4:	68 8e 00 00 00       	push   $0x8e
c0101dc9:	6a 08                	push   $0x8
c0101dcb:	ff b3 cc ff ff ff    	push   -0x34(%ebx)
c0101dd1:	6a 02                	push   $0x2
c0101dd3:	e8 5c fb ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 3, (size_t)__isr3, 0x08, 0x8E );
c0101dd8:	68 8e 00 00 00       	push   $0x8e
c0101ddd:	6a 08                	push   $0x8
c0101ddf:	ff b3 90 ff ff ff    	push   -0x70(%ebx)
c0101de5:	6a 03                	push   $0x3
c0101de7:	e8 48 fb ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 4, (size_t)__isr4, 0x08, 0x8E );
c0101dec:	83 c4 20             	add    $0x20,%esp
c0101def:	68 8e 00 00 00       	push   $0x8e
c0101df4:	6a 08                	push   $0x8
c0101df6:	ff b3 e0 ff ff ff    	push   -0x20(%ebx)
c0101dfc:	6a 04                	push   $0x4
c0101dfe:	e8 31 fb ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 5, (size_t)__isr5, 0x08, 0x8E );
c0101e03:	68 8e 00 00 00       	push   $0x8e
c0101e08:	6a 08                	push   $0x8
c0101e0a:	ff b3 f0 ff ff ff    	push   -0x10(%ebx)
c0101e10:	6a 05                	push   $0x5
c0101e12:	e8 1d fb ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 6, (size_t)__isr6, 0x08, 0x8E );
c0101e17:	83 c4 20             	add    $0x20,%esp
c0101e1a:	68 8e 00 00 00       	push   $0x8e
c0101e1f:	6a 08                	push   $0x8
c0101e21:	ff b3 a8 ff ff ff    	push   -0x58(%ebx)
c0101e27:	6a 06                	push   $0x6
c0101e29:	e8 06 fb ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 7, (size_t)__isr7, 0x08, 0x8E );
c0101e2e:	68 8e 00 00 00       	push   $0x8e
c0101e33:	6a 08                	push   $0x8
c0101e35:	ff b3 98 ff ff ff    	push   -0x68(%ebx)
c0101e3b:	6a 07                	push   $0x7
c0101e3d:	e8 f2 fa ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 8, (size_t)__isr8, 0x08, 0x8E );
c0101e42:	83 c4 20             	add    $0x20,%esp
c0101e45:	68 8e 00 00 00       	push   $0x8e
c0101e4a:	6a 08                	push   $0x8
c0101e4c:	ff b3 e8 ff ff ff    	push   -0x18(%ebx)
c0101e52:	6a 08                	push   $0x8
c0101e54:	e8 db fa ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 9, (size_t)__isr9, 0x08, 0x8E );
c0101e59:	68 8e 00 00 00       	push   $0x8e
c0101e5e:	6a 08                	push   $0x8
c0101e60:	ff b3 a0 ff ff ff    	push   -0x60(%ebx)
c0101e66:	6a 09                	push   $0x9
c0101e68:	e8 c7 fa ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 10, (size_t)__isr10, 0x08, 0x8E );
c0101e6d:	83 c4 20             	add    $0x20,%esp
c0101e70:	68 8e 00 00 00       	push   $0x8e
c0101e75:	6a 08                	push   $0x8
c0101e77:	ff b3 94 ff ff ff    	push   -0x6c(%ebx)
c0101e7d:	6a 0a                	push   $0xa
c0101e7f:	e8 b0 fa ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 11, (size_t)__isr11, 0x08, 0x8E );
c0101e84:	68 8e 00 00 00       	push   $0x8e
c0101e89:	6a 08                	push   $0x8
c0101e8b:	ff b3 48 ff ff ff    	push   -0xb8(%ebx)
c0101e91:	6a 0b                	push   $0xb
c0101e93:	e8 9c fa ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 12, (size_t)__isr12, 0x08, 0x8E );
c0101e98:	83 c4 20             	add    $0x20,%esp
c0101e9b:	68 8e 00 00 00       	push   $0x8e
c0101ea0:	6a 08                	push   $0x8
c0101ea2:	ff b3 b0 ff ff ff    	push   -0x50(%ebx)
c0101ea8:	6a 0c                	push   $0xc
c0101eaa:	e8 85 fa ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 13, (size_t)__isr13, 0x08, 0x8E );
c0101eaf:	68 8e 00 00 00       	push   $0x8e
c0101eb4:	6a 08                	push   $0x8
c0101eb6:	ff b3 c4 ff ff ff    	push   -0x3c(%ebx)
c0101ebc:	6a 0d                	push   $0xd
c0101ebe:	e8 71 fa ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 14, (size_t)__isr14, 0x08, 0x8E );
c0101ec3:	83 c4 20             	add    $0x20,%esp
c0101ec6:	68 8e 00 00 00       	push   $0x8e
c0101ecb:	6a 08                	push   $0x8
c0101ecd:	ff b3 8c ff ff ff    	push   -0x74(%ebx)
c0101ed3:	6a 0e                	push   $0xe
c0101ed5:	e8 5a fa ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 15, (size_t)__isr15, 0x08, 0x8E );
c0101eda:	68 8e 00 00 00       	push   $0x8e
c0101edf:	6a 08                	push   $0x8
c0101ee1:	ff b3 70 ff ff ff    	push   -0x90(%ebx)
c0101ee7:	6a 0f                	push   $0xf
c0101ee9:	e8 46 fa ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 16, (size_t)__isr16, 0x08, 0x8E );
c0101eee:	83 c4 20             	add    $0x20,%esp
c0101ef1:	68 8e 00 00 00       	push   $0x8e
c0101ef6:	6a 08                	push   $0x8
c0101ef8:	ff b3 5c ff ff ff    	push   -0xa4(%ebx)
c0101efe:	6a 10                	push   $0x10
c0101f00:	e8 2f fa ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 17, (size_t)__isr17, 0x08, 0x8E );
c0101f05:	68 8e 00 00 00       	push   $0x8e
c0101f0a:	6a 08                	push   $0x8
c0101f0c:	ff b3 84 ff ff ff    	push   -0x7c(%ebx)
c0101f12:	6a 11                	push   $0x11
c0101f14:	e8 1b fa ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 18, (size_t)__isr18, 0x08, 0x8E );
c0101f19:	83 c4 20             	add    $0x20,%esp
c0101f1c:	68 8e 00 00 00       	push   $0x8e
c0101f21:	6a 08                	push   $0x8
c0101f23:	ff b3 d0 ff ff ff    	push   -0x30(%ebx)
c0101f29:	6a 12                	push   $0x12
c0101f2b:	e8 04 fa ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 19, (size_t)__isr19, 0x08, 0x8E );
c0101f30:	68 8e 00 00 00       	push   $0x8e
c0101f35:	6a 08                	push   $0x8
c0101f37:	ff b3 58 ff ff ff    	push   -0xa8(%ebx)
c0101f3d:	6a 13                	push   $0x13
c0101f3f:	e8 f0 f9 ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 20, (size_t)__isr20, 0x08, 0x8E );
c0101f44:	83 c4 20             	add    $0x20,%esp
c0101f47:	68 8e 00 00 00       	push   $0x8e
c0101f4c:	6a 08                	push   $0x8
c0101f4e:	ff b3 ec ff ff ff    	push   -0x14(%ebx)
c0101f54:	6a 14                	push   $0x14
c0101f56:	e8 d9 f9 ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 21, (size_t)__isr21, 0x08, 0x8E );
c0101f5b:	68 8e 00 00 00       	push   $0x8e
c0101f60:	6a 08                	push   $0x8
c0101f62:	ff b3 50 ff ff ff    	push   -0xb0(%ebx)
c0101f68:	6a 15                	push   $0x15
c0101f6a:	e8 c5 f9 ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 22, (size_t)__isr22, 0x08, 0x8E );
c0101f6f:	83 c4 20             	add    $0x20,%esp
c0101f72:	68 8e 00 00 00       	push   $0x8e
c0101f77:	6a 08                	push   $0x8
c0101f79:	ff b3 b8 ff ff ff    	push   -0x48(%ebx)
c0101f7f:	6a 16                	push   $0x16
c0101f81:	e8 ae f9 ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 23, (size_t)__isr23, 0x08, 0x8E );
c0101f86:	68 8e 00 00 00       	push   $0x8e
c0101f8b:	6a 08                	push   $0x8
c0101f8d:	ff b3 6c ff ff ff    	push   -0x94(%ebx)
c0101f93:	6a 17                	push   $0x17
c0101f95:	e8 9a f9 ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 24, (size_t)__isr24, 0x08, 0x8E );
c0101f9a:	83 c4 20             	add    $0x20,%esp
c0101f9d:	68 8e 00 00 00       	push   $0x8e
c0101fa2:	6a 08                	push   $0x8
c0101fa4:	ff b3 54 ff ff ff    	push   -0xac(%ebx)
c0101faa:	6a 18                	push   $0x18
c0101fac:	e8 83 f9 ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 25, (size_t)__isr25, 0x08, 0x8E );
c0101fb1:	68 8e 00 00 00       	push   $0x8e
c0101fb6:	6a 08                	push   $0x8
c0101fb8:	ff b3 d4 ff ff ff    	push   -0x2c(%ebx)
c0101fbe:	6a 19                	push   $0x19
c0101fc0:	e8 6f f9 ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 26, (size_t)__isr26, 0x08, 0x8E );
c0101fc5:	83 c4 20             	add    $0x20,%esp
c0101fc8:	68 8e 00 00 00       	push   $0x8e
c0101fcd:	6a 08                	push   $0x8
c0101fcf:	ff b3 bc ff ff ff    	push   -0x44(%ebx)
c0101fd5:	6a 1a                	push   $0x1a
c0101fd7:	e8 58 f9 ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 27, (size_t)__isr27, 0x08, 0x8E );
c0101fdc:	68 8e 00 00 00       	push   $0x8e
c0101fe1:	6a 08                	push   $0x8
c0101fe3:	ff b3 a4 ff ff ff    	push   -0x5c(%ebx)
c0101fe9:	6a 1b                	push   $0x1b
c0101feb:	e8 44 f9 ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 28, (size_t)__isr28, 0x08, 0x8E );
c0101ff0:	83 c4 20             	add    $0x20,%esp
c0101ff3:	68 8e 00 00 00       	push   $0x8e
c0101ff8:	6a 08                	push   $0x8
c0101ffa:	ff b3 f8 ff ff ff    	push   -0x8(%ebx)
c0102000:	6a 1c                	push   $0x1c
c0102002:	e8 2d f9 ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 29, (size_t)__isr29, 0x08, 0x8E );
c0102007:	68 8e 00 00 00       	push   $0x8e
c010200c:	6a 08                	push   $0x8
c010200e:	ff b3 78 ff ff ff    	push   -0x88(%ebx)
c0102014:	6a 1d                	push   $0x1d
c0102016:	e8 19 f9 ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 30, (size_t)__isr30, 0x08, 0x8E );
c010201b:	83 c4 20             	add    $0x20,%esp
c010201e:	68 8e 00 00 00       	push   $0x8e
c0102023:	6a 08                	push   $0x8
c0102025:	ff b3 9c ff ff ff    	push   -0x64(%ebx)
c010202b:	6a 1e                	push   $0x1e
c010202d:	e8 02 f9 ff ff       	call   c0101934 <idt_set_gate>
    idt_set_gate( 31, (size_t)__isr31, 0x08, 0x8E );
c0102032:	68 8e 00 00 00       	push   $0x8e
c0102037:	6a 08                	push   $0x8
c0102039:	ff b3 c0 ff ff ff    	push   -0x40(%ebx)
c010203f:	6a 1f                	push   $0x1f
c0102041:	e8 ee f8 ff ff       	call   c0101934 <idt_set_gate>
    return OS32_SUCCESS;
}
c0102046:	8b 5d fc             	mov    -0x4(%ebp),%ebx
c0102049:	31 c0                	xor    %eax,%eax
c010204b:	c9                   	leave  
c010204c:	c3                   	ret    

c010204d <__fault_handler>:


void __fault_handler( interrupt_frame_t* regs )
{
c010204d:	f3 0f 1e fb          	endbr32 
c0102051:	55                   	push   %ebp
c0102052:	89 e5                	mov    %esp,%ebp
c0102054:	53                   	push   %ebx
c0102055:	e8 f6 05 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c010205a:	81 c3 7a 20 00 00    	add    $0x207a,%ebx
c0102060:	51                   	push   %ecx
    if (regs->intno < 32)
c0102061:	8b 45 08             	mov    0x8(%ebp),%eax
c0102064:	8b 40 30             	mov    0x30(%eax),%eax
c0102067:	83 f8 1f             	cmp    $0x1f,%eax
c010206a:	77 1a                	ja     c0102086 <__fault_handler+0x39>
    {
        vgaPrintf("%- %s", exception_messages[regs->intno]);
c010206c:	52                   	push   %edx
c010206d:	52                   	push   %edx
c010206e:	ff b4 83 0c 00 00 00 	push   0xc(%ebx,%eax,4)
c0102075:	8d 83 b7 ea ff ff    	lea    -0x1549(%ebx),%eax
c010207b:	50                   	push   %eax
c010207c:	e8 28 03 00 00       	call   c01023a9 <vgaPrintf>
c0102081:	83 c4 10             	add    $0x10,%esp
        for(;;);
c0102084:	eb fe                	jmp    c0102084 <__fault_handler+0x37>
    }
c0102086:	8b 5d fc             	mov    -0x4(%ebp),%ebx
c0102089:	c9                   	leave  
c010208a:	c3                   	ret    

c010208b <VGAScroll>:
#pragma pack(0)

static VGACell_t* VGAO = (VGACell_t*)(0xc00b8000);

static void VGAScroll()
{
c010208b:	55                   	push   %ebp
c010208c:	89 e5                	mov    %esp,%ebp
c010208e:	56                   	push   %esi
c010208f:	be 00 80 0b c0       	mov    $0xc00b8000,%esi
c0102094:	53                   	push   %ebx
c0102095:	e8 b6 05 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c010209a:	81 c3 3a 20 00 00    	add    $0x203a,%ebx
    for ( size_t i = 1; i < VGA_HEIGHT; i++)
    {
        memcpy( &VGAO[ (i-1)*VGA_WIDTH ], &VGAO[ i*VGA_WIDTH ], VGA_WIDTH*sizeof(VGACell_t) );
c01020a0:	89 f0                	mov    %esi,%eax
c01020a2:	81 c6 a0 00 00 00    	add    $0xa0,%esi
c01020a8:	52                   	push   %edx
c01020a9:	68 a0 00 00 00       	push   $0xa0
c01020ae:	56                   	push   %esi
c01020af:	50                   	push   %eax
c01020b0:	e8 5a 05 00 00       	call   c010260f <memcpy>
    for ( size_t i = 1; i < VGA_HEIGHT; i++)
c01020b5:	83 c4 10             	add    $0x10,%esp
c01020b8:	81 fe 00 8f 0b c0    	cmp    $0xc00b8f00,%esi
c01020be:	75 e0                	jne    c01020a0 <VGAScroll+0x15>
    }
    memset( &VGAO[ (VGA_HEIGHT-1)*VGA_WIDTH ], 0, VGA_WIDTH * sizeof(VGACell_t) );
c01020c0:	50                   	push   %eax
c01020c1:	68 a0 00 00 00       	push   $0xa0
c01020c6:	6a 00                	push   $0x0
c01020c8:	68 00 8f 0b c0       	push   $0xc00b8f00
c01020cd:	e8 5e 05 00 00       	call   c0102630 <memset>
}
c01020d2:	83 c4 10             	add    $0x10,%esp
c01020d5:	8d 65 f8             	lea    -0x8(%ebp),%esp
c01020d8:	5b                   	pop    %ebx
c01020d9:	5e                   	pop    %esi
c01020da:	5d                   	pop    %ebp
c01020db:	c3                   	ret    

c01020dc <vgaNewline>:
        }
    }
}

void vgaNewline()
{
c01020dc:	f3 0f 1e fb          	endbr32 
    cursor.x = 0;
c01020e0:	e8 6f 05 00 00       	call   c0102654 <__x86.get_pc_thunk.dx>
c01020e5:	81 c2 ef 1f 00 00    	add    $0x1fef,%edx
c01020eb:	c7 82 90 17 00 00 00 	movl   $0x0,0x1790(%edx)
c01020f2:	00 00 00 
    cursor.y++;
c01020f5:	8b 82 94 17 00 00    	mov    0x1794(%edx),%eax
c01020fb:	40                   	inc    %eax
c01020fc:	89 82 94 17 00 00    	mov    %eax,0x1794(%edx)
    if (cursor.y >= VGA_HEIGHT)
c0102102:	83 f8 18             	cmp    $0x18,%eax
c0102105:	76 02                	jbe    c0102109 <vgaNewline+0x2d>
    {
        VGAScroll();
c0102107:	eb 82                	jmp    c010208b <VGAScroll>
    }
}
c0102109:	c3                   	ret    

c010210a <vgaPutc>:

void vgaPutc( char c )
{
c010210a:	f3 0f 1e fb          	endbr32 
    if (c == VGA_NEWLINE)
c010210e:	e8 39 05 00 00       	call   c010264c <__x86.get_pc_thunk.ax>
c0102113:	05 c1 1f 00 00       	add    $0x1fc1,%eax
{
c0102118:	55                   	push   %ebp
c0102119:	89 e5                	mov    %esp,%ebp
c010211b:	57                   	push   %edi
c010211c:	56                   	push   %esi
c010211d:	53                   	push   %ebx
c010211e:	83 ec 1c             	sub    $0x1c,%esp
c0102121:	8b 75 08             	mov    0x8(%ebp),%esi
    if (c == VGA_NEWLINE)
c0102124:	89 f3                	mov    %esi,%ebx
c0102126:	80 fb 0a             	cmp    $0xa,%bl
c0102129:	75 09                	jne    c0102134 <vgaPutc+0x2a>
    else
    {
        VGAO[ cursor.x + cursor.y*VGA_WIDTH ] = (VGACell_t){ c, cur_fg, cur_bg };
        VGAAdvance();
    }
}
c010212b:	83 c4 1c             	add    $0x1c,%esp
c010212e:	5b                   	pop    %ebx
c010212f:	5e                   	pop    %esi
c0102130:	5f                   	pop    %edi
c0102131:	5d                   	pop    %ebp
        vgaNewline();
c0102132:	eb a8                	jmp    c01020dc <vgaNewline>
        VGAO[ cursor.x + cursor.y*VGA_WIDTH ] = (VGACell_t){ c, cur_fg, cur_bg };
c0102134:	8b 98 94 17 00 00    	mov    0x1794(%eax),%ebx
c010213a:	8b b8 90 17 00 00    	mov    0x1790(%eax),%edi
c0102140:	89 f1                	mov    %esi,%ecx
c0102142:	0f b6 b0 8c 17 00 00 	movzbl 0x178c(%eax),%esi
c0102149:	6b d3 50             	imul   $0x50,%ebx,%edx
c010214c:	89 7d e4             	mov    %edi,-0x1c(%ebp)
c010214f:	c1 e6 04             	shl    $0x4,%esi
c0102152:	01 fa                	add    %edi,%edx
c0102154:	0f b6 b8 3c ff ff ff 	movzbl -0xc4(%eax),%edi
c010215b:	01 d2                	add    %edx,%edx
c010215d:	83 e7 0f             	and    $0xf,%edi
c0102160:	88 8a 00 80 0b c0    	mov    %cl,-0x3ff48000(%edx)
c0102166:	09 fe                	or     %edi,%esi
c0102168:	89 f1                	mov    %esi,%ecx
c010216a:	88 8a 01 80 0b c0    	mov    %cl,-0x3ff47fff(%edx)
    cursor.x++;
c0102170:	8b 4d e4             	mov    -0x1c(%ebp),%ecx
c0102173:	41                   	inc    %ecx
    if (cursor.x >= VGA_WIDTH)
c0102174:	83 f9 4f             	cmp    $0x4f,%ecx
c0102177:	77 08                	ja     c0102181 <vgaPutc+0x77>
    cursor.x++;
c0102179:	89 88 90 17 00 00    	mov    %ecx,0x1790(%eax)
c010217f:	eb 24                	jmp    c01021a5 <vgaPutc+0x9b>
        cursor.x = 0;
c0102181:	c7 80 90 17 00 00 00 	movl   $0x0,0x1790(%eax)
c0102188:	00 00 00 
        cursor.y++;
c010218b:	8d 53 01             	lea    0x1(%ebx),%edx
c010218e:	89 90 94 17 00 00    	mov    %edx,0x1794(%eax)
        if (cursor.y >= VGA_HEIGHT)
c0102194:	83 fa 18             	cmp    $0x18,%edx
c0102197:	76 0c                	jbe    c01021a5 <vgaPutc+0x9b>
}
c0102199:	83 c4 1c             	add    $0x1c,%esp
c010219c:	5b                   	pop    %ebx
c010219d:	5e                   	pop    %esi
c010219e:	5f                   	pop    %edi
c010219f:	5d                   	pop    %ebp
            VGAScroll();
c01021a0:	e9 e6 fe ff ff       	jmp    c010208b <VGAScroll>
}
c01021a5:	83 c4 1c             	add    $0x1c,%esp
c01021a8:	5b                   	pop    %ebx
c01021a9:	5e                   	pop    %esi
c01021aa:	5f                   	pop    %edi
c01021ab:	5d                   	pop    %ebp
c01021ac:	c3                   	ret    

c01021ad <vgaPuts>:
void vgaPuts( const char* msg )
{
c01021ad:	f3 0f 1e fb          	endbr32 
c01021b1:	55                   	push   %ebp
c01021b2:	89 e5                	mov    %esp,%ebp
c01021b4:	53                   	push   %ebx
c01021b5:	50                   	push   %eax
c01021b6:	8b 5d 08             	mov    0x8(%ebp),%ebx
    while (*msg) vgaPutc( *(msg++) );
c01021b9:	0f be 03             	movsbl (%ebx),%eax
c01021bc:	84 c0                	test   %al,%al
c01021be:	74 0f                	je     c01021cf <vgaPuts+0x22>
c01021c0:	83 ec 0c             	sub    $0xc,%esp
c01021c3:	43                   	inc    %ebx
c01021c4:	50                   	push   %eax
c01021c5:	e8 40 ff ff ff       	call   c010210a <vgaPutc>
c01021ca:	83 c4 10             	add    $0x10,%esp
c01021cd:	eb ea                	jmp    c01021b9 <vgaPuts+0xc>
}
c01021cf:	8b 5d fc             	mov    -0x4(%ebp),%ebx
c01021d2:	c9                   	leave  
c01021d3:	c3                   	ret    

c01021d4 <vgaPutln>:
void vgaPutln( const char* msg )
{
c01021d4:	f3 0f 1e fb          	endbr32 
c01021d8:	55                   	push   %ebp
c01021d9:	89 e5                	mov    %esp,%ebp
c01021db:	53                   	push   %ebx
c01021dc:	50                   	push   %eax
c01021dd:	8b 5d 08             	mov    0x8(%ebp),%ebx
    while (*msg) vgaPutc( *(msg++) );
c01021e0:	0f be 03             	movsbl (%ebx),%eax
c01021e3:	84 c0                	test   %al,%al
c01021e5:	74 0f                	je     c01021f6 <vgaPutln+0x22>
c01021e7:	83 ec 0c             	sub    $0xc,%esp
c01021ea:	43                   	inc    %ebx
c01021eb:	50                   	push   %eax
c01021ec:	e8 19 ff ff ff       	call   c010210a <vgaPutc>
c01021f1:	83 c4 10             	add    $0x10,%esp
c01021f4:	eb ea                	jmp    c01021e0 <vgaPutln+0xc>
    VGAScroll();
}
c01021f6:	8b 5d fc             	mov    -0x4(%ebp),%ebx
c01021f9:	c9                   	leave  
    VGAScroll();
c01021fa:	e9 8c fe ff ff       	jmp    c010208b <VGAScroll>

c01021ff <vgaPrintInt>:

const char* vgaMultiBaseIntChars = "012345679abcdef";

void vgaPrintInt( unsigned num, bool sign, uint32_t base )
{
c01021ff:	f3 0f 1e fb          	endbr32 
c0102203:	55                   	push   %ebp
c0102204:	89 e5                	mov    %esp,%ebp
c0102206:	57                   	push   %edi
c0102207:	56                   	push   %esi
c0102208:	53                   	push   %ebx
c0102209:	e8 42 04 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c010220e:	81 c3 c6 1e 00 00    	add    $0x1ec6,%ebx
c0102214:	83 ec 3c             	sub    $0x3c,%esp
c0102217:	8b 4d 10             	mov    0x10(%ebp),%ecx
c010221a:	8b 75 08             	mov    0x8(%ebp),%esi
c010221d:	8b 45 0c             	mov    0xc(%ebp),%eax
c0102220:	89 4d c4             	mov    %ecx,-0x3c(%ebp)
    
    char digits[22];
    
    if (num == 0){
c0102223:	85 f6                	test   %esi,%esi
c0102225:	75 13                	jne    c010223a <vgaPrintInt+0x3b>
        vgaPutc('0');
c0102227:	c7 45 08 30 00 00 00 	movl   $0x30,0x8(%ebp)
    do
    {
        counter--;
        vgaPutc(digits[counter]);
    }while (counter);
}
c010222e:	8d 65 f4             	lea    -0xc(%ebp),%esp
c0102231:	5b                   	pop    %ebx
c0102232:	5e                   	pop    %esi
c0102233:	5f                   	pop    %edi
c0102234:	5d                   	pop    %ebp
        vgaPutc('0');
c0102235:	e9 d0 fe ff ff       	jmp    c010210a <vgaPutc>
    else if (((int)num) < 0 && sign)
c010223a:	79 13                	jns    c010224f <vgaPrintInt+0x50>
c010223c:	84 c0                	test   %al,%al
c010223e:	74 0f                	je     c010224f <vgaPrintInt+0x50>
        vgaPutc('-');
c0102240:	83 ec 0c             	sub    $0xc,%esp
        num = -(int)num;
c0102243:	f7 de                	neg    %esi
        vgaPutc('-');
c0102245:	6a 2d                	push   $0x2d
c0102247:	e8 be fe ff ff       	call   c010210a <vgaPutc>
        num = -(int)num;
c010224c:	83 c4 10             	add    $0x10,%esp
    memset(digits, 0, sizeof(digits));
c010224f:	8d 7d d2             	lea    -0x2e(%ebp),%edi
c0102252:	50                   	push   %eax
c0102253:	6a 16                	push   $0x16
c0102255:	6a 00                	push   $0x0
c0102257:	57                   	push   %edi
c0102258:	e8 d3 03 00 00       	call   c0102630 <memset>
        digits[counter] = vgaMultiBaseIntChars[ ( num % base ) ];
c010225d:	8b 8b 8c 00 00 00    	mov    0x8c(%ebx),%ecx
c0102263:	83 c4 10             	add    $0x10,%esp
    size_t counter = 0;
c0102266:	31 db                	xor    %ebx,%ebx
        digits[counter] = vgaMultiBaseIntChars[ ( num % base ) ];
c0102268:	89 f0                	mov    %esi,%eax
c010226a:	31 d2                	xor    %edx,%edx
c010226c:	f7 75 c4             	divl   -0x3c(%ebp)
c010226f:	8a 14 11             	mov    (%ecx,%edx,1),%dl
c0102272:	88 54 1d d2          	mov    %dl,-0x2e(%ebp,%ebx,1)
        num /= base;
c0102276:	89 f2                	mov    %esi,%edx
        counter ++;
c0102278:	43                   	inc    %ebx
        num /= base;
c0102279:	89 c6                	mov    %eax,%esi
    while (num)
c010227b:	39 55 c4             	cmp    %edx,-0x3c(%ebp)
c010227e:	76 e8                	jbe    c0102268 <vgaPrintInt+0x69>
        counter--;
c0102280:	4b                   	dec    %ebx
        vgaPutc(digits[counter]);
c0102281:	83 ec 0c             	sub    $0xc,%esp
c0102284:	0f be 04 1f          	movsbl (%edi,%ebx,1),%eax
c0102288:	50                   	push   %eax
c0102289:	e8 7c fe ff ff       	call   c010210a <vgaPutc>
    }while (counter);
c010228e:	83 c4 10             	add    $0x10,%esp
c0102291:	85 db                	test   %ebx,%ebx
c0102293:	75 eb                	jne    c0102280 <vgaPrintInt+0x81>
}
c0102295:	8d 65 f4             	lea    -0xc(%ebp),%esp
c0102298:	5b                   	pop    %ebx
c0102299:	5e                   	pop    %esi
c010229a:	5f                   	pop    %edi
c010229b:	5d                   	pop    %ebp
c010229c:	c3                   	ret    

c010229d <vgaOk>:

void vgaOk()
{
c010229d:	f3 0f 1e fb          	endbr32 
c01022a1:	55                   	push   %ebp
c01022a2:	89 e5                	mov    %esp,%ebp
c01022a4:	53                   	push   %ebx
c01022a5:	e8 a6 03 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c01022aa:	81 c3 2a 1e 00 00    	add    $0x1e2a,%ebx
c01022b0:	83 ec 20             	sub    $0x20,%esp
    vgac_t o_fg, o_bg;
    vgaGetCol(&o_fg, &o_bg);
    vgaSetCol( VGA_WHITE, VGA_BLACK );
    vgaPuts("[ ");
c01022b3:	8d 8b f9 eb ff ff    	lea    -0x1407(%ebx),%ecx
    cur_fg = fg;
    cur_bg = bg;
}
void vgaGetCol( vgac_t* fg, vgac_t* bg )
{
    *fg = cur_fg;
c01022b9:	8a 93 3c ff ff ff    	mov    -0xc4(%ebx),%dl
    *bg = cur_bg;
c01022bf:	8a 83 8c 17 00 00    	mov    0x178c(%ebx),%al
    cur_fg = fg;
c01022c5:	c6 83 3c ff ff ff 0f 	movb   $0xf,-0xc4(%ebx)
    vgaPuts("[ ");
c01022cc:	51                   	push   %ecx
    *fg = cur_fg;
c01022cd:	88 55 f6             	mov    %dl,-0xa(%ebp)
    *bg = cur_bg;
c01022d0:	88 45 f7             	mov    %al,-0x9(%ebp)
    cur_bg = bg;
c01022d3:	c6 83 8c 17 00 00 00 	movb   $0x0,0x178c(%ebx)
    vgaPuts("[ ");
c01022da:	e8 ce fe ff ff       	call   c01021ad <vgaPuts>
    vgaPuts("OK");
c01022df:	8d 8b fc eb ff ff    	lea    -0x1404(%ebx),%ecx
{
    return cur_bg;
}
void vgaSetFg( vgac_t fg )
{
    cur_fg = fg;
c01022e5:	c6 83 3c ff ff ff 02 	movb   $0x2,-0xc4(%ebx)
    vgaPuts("OK");
c01022ec:	89 0c 24             	mov    %ecx,(%esp)
c01022ef:	e8 b9 fe ff ff       	call   c01021ad <vgaPuts>
    vgaPuts(" ]");
c01022f4:	8d 8b ff eb ff ff    	lea    -0x1401(%ebx),%ecx
    cur_fg = fg;
c01022fa:	c6 83 3c ff ff ff 0f 	movb   $0xf,-0xc4(%ebx)
    vgaPuts(" ]");
c0102301:	89 0c 24             	mov    %ecx,(%esp)
c0102304:	e8 a4 fe ff ff       	call   c01021ad <vgaPuts>
    cur_fg = fg;
c0102309:	8a 55 f6             	mov    -0xa(%ebp),%dl
    cur_bg = bg;
c010230c:	8a 45 f7             	mov    -0x9(%ebp),%al
}
c010230f:	83 c4 10             	add    $0x10,%esp
    cur_fg = fg;
c0102312:	88 93 3c ff ff ff    	mov    %dl,-0xc4(%ebx)
    cur_bg = bg;
c0102318:	88 83 8c 17 00 00    	mov    %al,0x178c(%ebx)
}
c010231e:	8b 5d fc             	mov    -0x4(%ebp),%ebx
c0102321:	c9                   	leave  
c0102322:	c3                   	ret    

c0102323 <vgaFailed>:
{
c0102323:	f3 0f 1e fb          	endbr32 
c0102327:	55                   	push   %ebp
c0102328:	89 e5                	mov    %esp,%ebp
c010232a:	53                   	push   %ebx
c010232b:	e8 20 03 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c0102330:	81 c3 a4 1d 00 00    	add    $0x1da4,%ebx
c0102336:	83 ec 20             	sub    $0x20,%esp
    vgaPuts("[ ");
c0102339:	8d 8b f9 eb ff ff    	lea    -0x1407(%ebx),%ecx
    *fg = cur_fg;
c010233f:	8a 93 3c ff ff ff    	mov    -0xc4(%ebx),%dl
    *bg = cur_bg;
c0102345:	8a 83 8c 17 00 00    	mov    0x178c(%ebx),%al
    cur_fg = fg;
c010234b:	c6 83 3c ff ff ff 0f 	movb   $0xf,-0xc4(%ebx)
    vgaPuts("[ ");
c0102352:	51                   	push   %ecx
    *fg = cur_fg;
c0102353:	88 55 f6             	mov    %dl,-0xa(%ebp)
    *bg = cur_bg;
c0102356:	88 45 f7             	mov    %al,-0x9(%ebp)
    cur_bg = bg;
c0102359:	c6 83 8c 17 00 00 00 	movb   $0x0,0x178c(%ebx)
    vgaPuts("[ ");
c0102360:	e8 48 fe ff ff       	call   c01021ad <vgaPuts>
    vgaPuts("FAILED");
c0102365:	8d 8b 02 ec ff ff    	lea    -0x13fe(%ebx),%ecx
    cur_fg = fg;
c010236b:	c6 83 3c ff ff ff 04 	movb   $0x4,-0xc4(%ebx)
    vgaPuts("FAILED");
c0102372:	89 0c 24             	mov    %ecx,(%esp)
c0102375:	e8 33 fe ff ff       	call   c01021ad <vgaPuts>
    vgaPuts(" ]");
c010237a:	8d 8b ff eb ff ff    	lea    -0x1401(%ebx),%ecx
    cur_fg = fg;
c0102380:	c6 83 3c ff ff ff 0f 	movb   $0xf,-0xc4(%ebx)
    vgaPuts(" ]");
c0102387:	89 0c 24             	mov    %ecx,(%esp)
c010238a:	e8 1e fe ff ff       	call   c01021ad <vgaPuts>
    cur_fg = fg;
c010238f:	8a 55 f6             	mov    -0xa(%ebp),%dl
    cur_bg = bg;
c0102392:	8a 45 f7             	mov    -0x9(%ebp),%al
}
c0102395:	83 c4 10             	add    $0x10,%esp
    cur_fg = fg;
c0102398:	88 93 3c ff ff ff    	mov    %dl,-0xc4(%ebx)
    cur_bg = bg;
c010239e:	88 83 8c 17 00 00    	mov    %al,0x178c(%ebx)
}
c01023a4:	8b 5d fc             	mov    -0x4(%ebp),%ebx
c01023a7:	c9                   	leave  
c01023a8:	c3                   	ret    

c01023a9 <vgaPrintf>:
{
c01023a9:	f3 0f 1e fb          	endbr32 
c01023ad:	55                   	push   %ebp
c01023ae:	89 e5                	mov    %esp,%ebp
c01023b0:	57                   	push   %edi
c01023b1:	56                   	push   %esi
c01023b2:	53                   	push   %ebx
    va_start(args, format);
c01023b3:	8d 5d 0c             	lea    0xc(%ebp),%ebx
{
c01023b6:	83 ec 0c             	sub    $0xc,%esp
c01023b9:	8b 7d 08             	mov    0x8(%ebp),%edi
    while (*format)
c01023bc:	0f be 07             	movsbl (%edi),%eax
c01023bf:	84 c0                	test   %al,%al
c01023c1:	0f 84 be 00 00 00    	je     c0102485 <vgaPrintf+0xdc>
            format++;
c01023c7:	8d 77 01             	lea    0x1(%edi),%esi
        if (*format == '%')
c01023ca:	3c 25                	cmp    $0x25,%al
c01023cc:	0f 85 9d 00 00 00    	jne    c010246f <vgaPrintf+0xc6>
            switch (*format)
c01023d2:	8a 47 01             	mov    0x1(%edi),%al
c01023d5:	3c 64                	cmp    $0x64,%al
c01023d7:	74 60                	je     c0102439 <vgaPrintf+0x90>
c01023d9:	7f 35                	jg     c0102410 <vgaPrintf+0x67>
c01023db:	3c 2d                	cmp    $0x2d,%al
c01023dd:	0f 84 85 00 00 00    	je     c0102468 <vgaPrintf+0xbf>
c01023e3:	7f 16                	jg     c01023fb <vgaPrintf+0x52>
c01023e5:	3c 25                	cmp    $0x25,%al
c01023e7:	74 70                	je     c0102459 <vgaPrintf+0xb0>
c01023e9:	3c 2b                	cmp    $0x2b,%al
c01023eb:	0f 85 8c 00 00 00    	jne    c010247d <vgaPrintf+0xd4>
                vgaOk();
c01023f1:	e8 a7 fe ff ff       	call   c010229d <vgaOk>
                break;
c01023f6:	e9 82 00 00 00       	jmp    c010247d <vgaPrintf+0xd4>
            switch (*format)
c01023fb:	3c 63                	cmp    $0x63,%al
c01023fd:	75 7e                	jne    c010247d <vgaPrintf+0xd4>
                vgaPutc( va_arg( args, int ) );
c01023ff:	0f be 03             	movsbl (%ebx),%eax
c0102402:	83 ec 0c             	sub    $0xc,%esp
c0102405:	8d 7b 04             	lea    0x4(%ebx),%edi
c0102408:	50                   	push   %eax
c0102409:	e8 fc fc ff ff       	call   c010210a <vgaPutc>
                break;
c010240e:	eb 42                	jmp    c0102452 <vgaPrintf+0xa9>
            switch (*format)
c0102410:	3c 75                	cmp    $0x75,%al
c0102412:	74 2f                	je     c0102443 <vgaPrintf+0x9a>
c0102414:	7f 17                	jg     c010242d <vgaPrintf+0x84>
c0102416:	3c 69                	cmp    $0x69,%al
c0102418:	74 1f                	je     c0102439 <vgaPrintf+0x90>
c010241a:	3c 73                	cmp    $0x73,%al
c010241c:	75 5f                	jne    c010247d <vgaPrintf+0xd4>
                vgaPuts( va_arg(args, const char*) );
c010241e:	83 ec 0c             	sub    $0xc,%esp
c0102421:	ff 33                	push   (%ebx)
c0102423:	8d 7b 04             	lea    0x4(%ebx),%edi
c0102426:	e8 82 fd ff ff       	call   c01021ad <vgaPuts>
                break;
c010242b:	eb 25                	jmp    c0102452 <vgaPrintf+0xa9>
            switch (*format)
c010242d:	3c 78                	cmp    $0x78,%al
c010242f:	75 4c                	jne    c010247d <vgaPrintf+0xd4>
                vgaPrintInt( va_arg( args, int ), false, 16 );
c0102431:	8d 7b 04             	lea    0x4(%ebx),%edi
c0102434:	52                   	push   %edx
c0102435:	6a 10                	push   $0x10
c0102437:	eb 10                	jmp    c0102449 <vgaPrintf+0xa0>
                vgaPrintInt( va_arg(args, int), true, 10 );
c0102439:	8d 7b 04             	lea    0x4(%ebx),%edi
c010243c:	51                   	push   %ecx
c010243d:	6a 0a                	push   $0xa
c010243f:	6a 01                	push   $0x1
c0102441:	eb 08                	jmp    c010244b <vgaPrintf+0xa2>
                vgaPrintInt( va_arg( args, int ), false, 10 );
c0102443:	50                   	push   %eax
c0102444:	8d 7b 04             	lea    0x4(%ebx),%edi
c0102447:	6a 0a                	push   $0xa
                vgaPrintInt( va_arg( args, int ), false, 16 );
c0102449:	6a 00                	push   $0x0
c010244b:	ff 33                	push   (%ebx)
c010244d:	e8 ad fd ff ff       	call   c01021ff <vgaPrintInt>
                break;
c0102452:	83 c4 10             	add    $0x10,%esp
                vgaPrintInt( va_arg( args, int ), false, 16 );
c0102455:	89 fb                	mov    %edi,%ebx
                break;
c0102457:	eb 24                	jmp    c010247d <vgaPrintf+0xd4>
                vgaPutc('%');
c0102459:	83 ec 0c             	sub    $0xc,%esp
c010245c:	6a 25                	push   $0x25
c010245e:	e8 a7 fc ff ff       	call   c010210a <vgaPutc>
                break;
c0102463:	83 c4 10             	add    $0x10,%esp
c0102466:	eb 15                	jmp    c010247d <vgaPrintf+0xd4>
                vgaFailed();
c0102468:	e8 b6 fe ff ff       	call   c0102323 <vgaFailed>
                break;
c010246d:	eb 0e                	jmp    c010247d <vgaPrintf+0xd4>
            vgaPutc(*format);
c010246f:	83 ec 0c             	sub    $0xc,%esp
c0102472:	89 fe                	mov    %edi,%esi
c0102474:	50                   	push   %eax
c0102475:	e8 90 fc ff ff       	call   c010210a <vgaPutc>
c010247a:	83 c4 10             	add    $0x10,%esp
        format++;
c010247d:	8d 7e 01             	lea    0x1(%esi),%edi
c0102480:	e9 37 ff ff ff       	jmp    c01023bc <vgaPrintf+0x13>
}
c0102485:	8d 65 f4             	lea    -0xc(%ebp),%esp
c0102488:	5b                   	pop    %ebx
c0102489:	5e                   	pop    %esi
c010248a:	5f                   	pop    %edi
c010248b:	5d                   	pop    %ebp
c010248c:	c3                   	ret    

c010248d <vgaSetCol>:
{
c010248d:	f3 0f 1e fb          	endbr32 
    cur_fg = fg;
c0102491:	e8 b6 01 00 00       	call   c010264c <__x86.get_pc_thunk.ax>
c0102496:	05 3e 1c 00 00       	add    $0x1c3e,%eax
{
c010249b:	55                   	push   %ebp
c010249c:	89 e5                	mov    %esp,%ebp
    cur_fg = fg;
c010249e:	8b 55 08             	mov    0x8(%ebp),%edx
c01024a1:	88 90 3c ff ff ff    	mov    %dl,-0xc4(%eax)
    cur_bg = bg;
c01024a7:	8b 55 0c             	mov    0xc(%ebp),%edx
}
c01024aa:	5d                   	pop    %ebp
    cur_bg = bg;
c01024ab:	88 90 8c 17 00 00    	mov    %dl,0x178c(%eax)
}
c01024b1:	c3                   	ret    

c01024b2 <vgaGetCol>:
{
c01024b2:	f3 0f 1e fb          	endbr32 
    *fg = cur_fg;
c01024b6:	e8 91 01 00 00       	call   c010264c <__x86.get_pc_thunk.ax>
c01024bb:	05 19 1c 00 00       	add    $0x1c19,%eax
{
c01024c0:	55                   	push   %ebp
c01024c1:	89 e5                	mov    %esp,%ebp
    *fg = cur_fg;
c01024c3:	8b 55 08             	mov    0x8(%ebp),%edx
c01024c6:	8a 88 3c ff ff ff    	mov    -0xc4(%eax),%cl
c01024cc:	88 0a                	mov    %cl,(%edx)
    *bg = cur_bg;
c01024ce:	8a 90 8c 17 00 00    	mov    0x178c(%eax),%dl
c01024d4:	8b 45 0c             	mov    0xc(%ebp),%eax
c01024d7:	88 10                	mov    %dl,(%eax)
}
c01024d9:	5d                   	pop    %ebp
c01024da:	c3                   	ret    

c01024db <vgaGetFg>:
{
c01024db:	f3 0f 1e fb          	endbr32 
    return cur_fg;
c01024df:	e8 68 01 00 00       	call   c010264c <__x86.get_pc_thunk.ax>
c01024e4:	05 f0 1b 00 00       	add    $0x1bf0,%eax
c01024e9:	8a 80 3c ff ff ff    	mov    -0xc4(%eax),%al
}
c01024ef:	c3                   	ret    

c01024f0 <vgaGetBg>:
{
c01024f0:	f3 0f 1e fb          	endbr32 
    return cur_bg;
c01024f4:	e8 53 01 00 00       	call   c010264c <__x86.get_pc_thunk.ax>
c01024f9:	05 db 1b 00 00       	add    $0x1bdb,%eax
c01024fe:	8a 80 8c 17 00 00    	mov    0x178c(%eax),%al
}
c0102504:	c3                   	ret    

c0102505 <vgaSetFg>:
{
c0102505:	f3 0f 1e fb          	endbr32 
    cur_fg = fg;
c0102509:	e8 3e 01 00 00       	call   c010264c <__x86.get_pc_thunk.ax>
c010250e:	05 c6 1b 00 00       	add    $0x1bc6,%eax
{
c0102513:	55                   	push   %ebp
c0102514:	89 e5                	mov    %esp,%ebp
    cur_fg = fg;
c0102516:	8b 55 08             	mov    0x8(%ebp),%edx
}
c0102519:	5d                   	pop    %ebp
    cur_fg = fg;
c010251a:	88 90 3c ff ff ff    	mov    %dl,-0xc4(%eax)
}
c0102520:	c3                   	ret    

c0102521 <vgaSetBg>:
void vgaSetBg( vgac_t bg )
{
c0102521:	f3 0f 1e fb          	endbr32 
    cur_bg = bg;
c0102525:	e8 22 01 00 00       	call   c010264c <__x86.get_pc_thunk.ax>
c010252a:	05 aa 1b 00 00       	add    $0x1baa,%eax
{
c010252f:	55                   	push   %ebp
c0102530:	89 e5                	mov    %esp,%ebp
    cur_bg = bg;
c0102532:	8b 55 08             	mov    0x8(%ebp),%edx
}
c0102535:	5d                   	pop    %ebp
    cur_bg = bg;
c0102536:	88 90 8c 17 00 00    	mov    %dl,0x178c(%eax)
}
c010253c:	c3                   	ret    

c010253d <gdt_set_gate>:
    unsigned long base, 
    unsigned long limit, 
    uint8_t access, 
    uint8_t granularity 
)
{
c010253d:	f3 0f 1e fb          	endbr32 
c0102541:	55                   	push   %ebp
c0102542:	89 e5                	mov    %esp,%ebp
c0102544:	57                   	push   %edi
c0102545:	56                   	push   %esi
c0102546:	8b 4d 0c             	mov    0xc(%ebp),%ecx
c0102549:	53                   	push   %ebx
c010254a:	8b 75 08             	mov    0x8(%ebp),%esi
c010254d:	e8 fe 00 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c0102552:	81 c3 82 1b 00 00    	add    $0x1b82,%ebx
    //http://www.osdever.net/bkerndev/Docs/gdt.htm
    __gdt[number].base_low = (base & 0xFFFF);
    __gdt[number].base_middle = (base >> 16) & 0xFF;
c0102558:	89 cf                	mov    %ecx,%edi
c010255a:	c1 ef 10             	shr    $0x10,%edi
c010255d:	89 f8                	mov    %edi,%eax
    __gdt[number].base_low = (base & 0xFFFF);
c010255f:	8d 94 f3 a0 17 00 00 	lea    0x17a0(%ebx,%esi,8),%edx
    __gdt[number].base_middle = (base >> 16) & 0xFF;
c0102566:	88 42 04             	mov    %al,0x4(%edx)
    __gdt[number].base_high = (base >> 24) & 0xFF;

    __gdt[number].limit_low = (limit & 0xFFFF);
c0102569:	8b 45 10             	mov    0x10(%ebp),%eax
    __gdt[number].base_low = (base & 0xFFFF);
c010256c:	66 89 4a 02          	mov    %cx,0x2(%edx)
    __gdt[number].base_high = (base >> 24) & 0xFF;
c0102570:	c1 e9 18             	shr    $0x18,%ecx
    __gdt[number].limit_low = (limit & 0xFFFF);
c0102573:	66 89 84 f3 a0 17 00 	mov    %ax,0x17a0(%ebx,%esi,8)
c010257a:	00 
    __gdt[number].granularity = ((limit >> 16) & 0x0F);
c010257b:	8b 45 10             	mov    0x10(%ebp),%eax
    __gdt[number].base_high = (base >> 24) & 0xFF;
c010257e:	88 4a 07             	mov    %cl,0x7(%edx)

    __gdt[number].granularity |= (granularity & 0xF0);
c0102581:	8a 4d 18             	mov    0x18(%ebp),%cl
    __gdt[number].granularity = ((limit >> 16) & 0x0F);
c0102584:	c1 e8 10             	shr    $0x10,%eax
c0102587:	83 e0 0f             	and    $0xf,%eax
    __gdt[number].granularity |= (granularity & 0xF0);
c010258a:	83 e1 f0             	and    $0xfffffff0,%ecx
c010258d:	09 c8                	or     %ecx,%eax
c010258f:	88 42 06             	mov    %al,0x6(%edx)
    __gdt[number].access = access;
c0102592:	8b 45 14             	mov    0x14(%ebp),%eax
c0102595:	88 42 05             	mov    %al,0x5(%edx)

    return OS32_SUCCESS;
}
c0102598:	31 c0                	xor    %eax,%eax
c010259a:	5b                   	pop    %ebx
c010259b:	5e                   	pop    %esi
c010259c:	5f                   	pop    %edi
c010259d:	5d                   	pop    %ebp
c010259e:	c3                   	ret    

c010259f <__install_gdt>:

err_t __install_gdt()
{
c010259f:	f3 0f 1e fb          	endbr32 
c01025a3:	55                   	push   %ebp
c01025a4:	89 e5                	mov    %esp,%ebp
c01025a6:	53                   	push   %ebx
c01025a7:	e8 a4 00 00 00       	call   c0102650 <__x86.get_pc_thunk.bx>
c01025ac:	81 c3 28 1b 00 00    	add    $0x1b28,%ebx
c01025b2:	50                   	push   %eax
    __gdt_ptr.limit = (sizeof(struct gdt_entry) * 3)-1;
c01025b3:	66 c7 83 98 17 00 00 	movw   $0x17,0x1798(%ebx)
c01025ba:	17 00 
    __gdt_ptr.base = (uint32_t) &__gdt;
c01025bc:	8d 83 a0 17 00 00    	lea    0x17a0(%ebx),%eax
c01025c2:	89 83 9a 17 00 00    	mov    %eax,0x179a(%ebx)
    __gdt[number].limit_low = (limit & 0xFFFF);
c01025c8:	c7 83 a0 17 00 00 00 	movl   $0x0,0x17a0(%ebx)
c01025cf:	00 00 00 
    __gdt[number].base_middle = (base >> 16) & 0xFF;
c01025d2:	c7 83 a4 17 00 00 00 	movl   $0x0,0x17a4(%ebx)
c01025d9:	00 00 00 
    __gdt[number].limit_low = (limit & 0xFFFF);
c01025dc:	c7 83 a8 17 00 00 ff 	movl   $0xffff,0x17a8(%ebx)
c01025e3:	ff 00 00 
    __gdt[number].base_middle = (base >> 16) & 0xFF;
c01025e6:	c7 83 ac 17 00 00 00 	movl   $0xcf9a00,0x17ac(%ebx)
c01025ed:	9a cf 00 
    __gdt[number].limit_low = (limit & 0xFFFF);
c01025f0:	c7 83 b0 17 00 00 ff 	movl   $0xffff,0x17b0(%ebx)
c01025f7:	ff 00 00 
    __gdt[number].base_middle = (base >> 16) & 0xFF;
c01025fa:	c7 83 b4 17 00 00 00 	movl   $0xcf9200,0x17b4(%ebx)
c0102601:	92 cf 00 

    gdt_set_gate( 0,0,0,0,0 );
    gdt_set_gate( 1, 0, 0xFFFFFFFF, 0x9A, 0xCF );
    gdt_set_gate( 2, 0, 0xFFFFFFFF, 0x92, 0xCF );
    
    gdt_flush();
c0102604:	e8 f7 f2 ff ff       	call   c0101900 <gdt_flush>
    return OS32_SUCCESS;
c0102609:	5a                   	pop    %edx
c010260a:	31 c0                	xor    %eax,%eax
c010260c:	5b                   	pop    %ebx
c010260d:	5d                   	pop    %ebp
c010260e:	c3                   	ret    

c010260f <memcpy>:
#include "stdlib/string.h"

void memcpy( void * dest, const void * source, size_t bytes )
{
c010260f:	f3 0f 1e fb          	endbr32 
c0102613:	55                   	push   %ebp
c0102614:	89 e5                	mov    %esp,%ebp
c0102616:	53                   	push   %ebx
c0102617:	8b 55 08             	mov    0x8(%ebp),%edx
c010261a:	8b 4d 0c             	mov    0xc(%ebp),%ecx
c010261d:	8b 45 10             	mov    0x10(%ebp),%eax
    for (;bytes;bytes--)
c0102620:	85 c0                	test   %eax,%eax
c0102622:	74 09                	je     c010262d <memcpy+0x1e>
        ((char*)dest)[bytes]=((const char*)source)[bytes];
c0102624:	8a 1c 01             	mov    (%ecx,%eax,1),%bl
c0102627:	88 1c 02             	mov    %bl,(%edx,%eax,1)
    for (;bytes;bytes--)
c010262a:	48                   	dec    %eax
c010262b:	eb f3                	jmp    c0102620 <memcpy+0x11>
}
c010262d:	5b                   	pop    %ebx
c010262e:	5d                   	pop    %ebp
c010262f:	c3                   	ret    

c0102630 <memset>:
void memset( void* dest, char byte, size_t count )
{
c0102630:	f3 0f 1e fb          	endbr32 
c0102634:	55                   	push   %ebp
c0102635:	89 e5                	mov    %esp,%ebp
c0102637:	8b 55 08             	mov    0x8(%ebp),%edx
c010263a:	8b 45 10             	mov    0x10(%ebp),%eax
c010263d:	8a 4d 0c             	mov    0xc(%ebp),%cl
    for(;count;count--) ((char*)dest)[count] = byte;
c0102640:	85 c0                	test   %eax,%eax
c0102642:	74 06                	je     c010264a <memset+0x1a>
c0102644:	88 0c 02             	mov    %cl,(%edx,%eax,1)
c0102647:	48                   	dec    %eax
c0102648:	eb f6                	jmp    c0102640 <memset+0x10>
c010264a:	5d                   	pop    %ebp
c010264b:	c3                   	ret    

Disassembly of section .text.__x86.get_pc_thunk.ax:

c010264c <__x86.get_pc_thunk.ax>:
c010264c:	8b 04 24             	mov    (%esp),%eax
c010264f:	c3                   	ret    

Disassembly of section .text.__x86.get_pc_thunk.bx:

c0102650 <__x86.get_pc_thunk.bx>:
c0102650:	8b 1c 24             	mov    (%esp),%ebx
c0102653:	c3                   	ret    

Disassembly of section .text.__x86.get_pc_thunk.dx:

c0102654 <__x86.get_pc_thunk.dx>:
c0102654:	8b 14 24             	mov    (%esp),%edx
c0102657:	c3                   	ret    
