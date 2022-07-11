.data
.text
java:
move $fp,$sp
addi $sp,$sp,-4
sw $ra,0($sp)
li $s0,
java_return:
lw $ra,0($sp)
move $sp,$fp
jr $ra
main:
li $a5
jal java
li $v0,10
syscall