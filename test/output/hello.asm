.data
jota:.word	8
joto:.word	87
.text
java:
move $fp,$sp
addi $sp,$sp,-4
sw $ra,0($sp)
move $v0,$a1
b java_
java_:
lw $ra,0($sp)
move $sp,$fp
jr $ra
main:
li $s0,
li $v0,1
b main_
main_:
li $v0,10
syscall