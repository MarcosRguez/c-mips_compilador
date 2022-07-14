.data
.text
main:
li $t0,6
li $t1,2
sub $s0,$t0,$t1
li $v0,0
b main_
main_:
li $v0,10
syscall