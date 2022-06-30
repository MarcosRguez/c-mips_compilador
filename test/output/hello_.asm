.data

str0:.asciiz "¡Hola mundo!"
a:.word 1
b:.word 2

.text

func:

main:
	li $v0,4
	la $a0,str0
	syscall
	jal func
	li $v0,10
	syscall
