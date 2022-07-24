.data
.text
main:
li $s0,4
li $s1,0
main_for0:
	li $t0,5
	slt $t1,$s1,$t0
	beqz $t1,main_for0_
	li $t0,1
	add $t2,$s1,$t0
	move $s1,$t2
	b main_for0
main_for0_:
li $v0,0
b main_
main_:
li $v0,10
syscall