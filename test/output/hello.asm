.data
.text
java:
move $fp,$sp
addi $sp,$sp,-4
sw $ra,0($sp)
li $s0,
li $v0,4
b java_
java_:
lw $ra,0($sp)
move $sp,$fp
jr $ra
main:
li $a0,5
jal java
li $v0,0
b main_
main_:
li $v0,10
syscall