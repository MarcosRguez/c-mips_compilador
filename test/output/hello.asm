.data
.text
func:
move $fp,$sp
addi $sp,$sp,-4
sw $ra,0($sp)
li $v0,5
li $v0,0