.data
.text
main:
li $s0,4
li $s1,0
main_for0:
	li $t0,5
	bge $s1,$t0,main_for0_
	li $t0,1
	add $t1,$s1,$t0
	move $s1,$t1
	b main_for0
main_for0_:
li $v0,0
b main_
main_:
li $v0,10
syscall