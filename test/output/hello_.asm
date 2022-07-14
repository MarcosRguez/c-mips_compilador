.data

str0:.asciiz "¡Hola mundo!"
a:.word 1
b:.word 2

.text

func:

main:
	li $s0,1
	li $s1,0
	bnez $s0,if
	bnez $s1,if
		b if_
	if:
		# stuff
	if_:
	li $v0,10
	syscall
