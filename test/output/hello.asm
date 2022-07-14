.data
.text
main:
	li $s0,1
	li $s1,0
	bnez $s0,main_if0
	bnez $s1,main_if0
	b main_if0_
	main_if0:
	move $s2,$zero
	main_if0_:
	li $v0,0
	b main_
	main_:
	li $v0,10
	syscall