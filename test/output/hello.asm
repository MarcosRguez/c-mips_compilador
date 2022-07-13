.data
.text
main:
li $t0,5
li $t1,3
add $t2,$t0,$t1
move $s0,$t2
li $v0,0
b main_
main_:
li $v0,10
syscall