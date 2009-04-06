[bits 32]

[extern i86_pit_irq]
[global i86_pit_irq_wrapper]
i86_pit_irq_wrapper:
    pushad
    
    call i86_pit_irq
    
    popad
    iret