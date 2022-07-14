.data
.text
main:
li $s0,6
li $t0,9
move $s0,$t0
li $v0,0
b main_
main_:
li $v0,10
syscall