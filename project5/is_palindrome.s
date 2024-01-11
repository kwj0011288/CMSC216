# author: Wonjae Kim
# UID: 117283365
# Directory ID: wkim1128

.text
   .globl is_palindrome
   .globl strlen

strlen:
   # PROLOGUE
    subu $sp, $sp, 4               # alloc space
    sw   $ra, 0($sp)               # save return address
    move $t0, $a0                  # move string address to $t0

   # BODY
strlen_loop:
    lbu  $t1, 0($t0)               # load the curent position
    beqz $t1, strlen_end           # if it is zero, strlen_end
    addi $t0, $t0, 1               # increase the string pointer
    j    strlen_loop               # jump to strlen_loop

   # EPILOGUE
strlen_end:
    subu $v0, $t0, $a0              # get string length
    lw   $ra, 0($sp)                # restore the return address
    addi $sp, $sp, 4                # increment the stack pointer
    jr   $ra                        # return 


is_palindrome:
   li   $sp,   0x7ffffffc           # initialize sp
   # PROLOGUE
   subu $sp, $sp, 20                # 16 for i, len and 4 for $s0, alloc space
   sw   $ra, 16($sp)                # save return address
   sw   $s0, 12($sp)                # save $s0 register

   move $s0, $a0                    # save string pointer
   jal  strlen                      # call strlen
   sw   $v0, 8($sp)                 # store length of the string

   li   $t0, 0                      # initialize i to 0
   sw   $t0, 4($sp)                 # store 0 in i at $sp + 4

   # BODY
   lw   $t1, 8($sp)                 # load length into $t1
   beq  $t1, $zero, palindrome      # if length is zero, palindrome

loop:   
   lw   $t0, 4($sp)                 # load value of i
   srl  $t2, $t1, 1                 # len / 2
   bge  $t0, $t2, palindrome        # if i >= len / 2, end

   add  $t3, $t0, $s0               # address of string[i]
   lbu  $t3, 0($t3)                 # load string[i]

   subu $t4, $t1, $t0               # len - i
   subu $t4, $t4, 1                 # len - i - 1
   add  $t4, $s0, $t4               # address of [len - i - 1]
   lbu  $t4, 0($t4)                 # load the char at [len - i - 1]

   bne  $t3, $t4, not_palindrome    # compare chars

   addi $t0, $t0, 1                 # i++
   sw   $t0, 4($sp)                 # store updated i
   j    loop                        # Jump to loop

not_palindrome:
   li   $v0, 0                      # return 0
   j    end                         # jump to end

palindrome:
   li   $v0, 1                      # return 1

   # EPILOGUE
end:    
   lw   $s0, 12($sp)                # restore $s0 register
   lw   $ra, 16($sp)                # restore return address
   addi $sp, $sp, 20                # deallocate stack space
   jr   $ra                         # return
