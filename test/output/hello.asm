.data
.text
main:
li $s0,6
li $t0,3
add $t1,$s0,$t0
move $s1,$t1
li $v0,0
b main_
main_:
li $v0,10
syscall