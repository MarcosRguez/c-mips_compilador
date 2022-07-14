.data
.text
main:
li $s0,1
li $s1,0
main_while0:
bnez $s0,main_while0_
move $s2,$zero
b main_while0
main_while0_:
li $v0,0
b main_
main_:
li $v0,10
syscall