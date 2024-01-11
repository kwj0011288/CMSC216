# author: Wonjae Kim
# UID: 117283365
# Directory ID: wkim1128

.text
    .globl collatz

collatz:
    # PROLOGUE
    subu $sp, $sp, 16           # allocate stack space
    sw   $ra, 16($sp)           # save return address
    sw   $fp, 12($sp)           # save frame pointer
    addu $fp, $sp, 16           # set frame pointer

    # BODY
    lw   $t1, 8($fp)            # load n into $t0
    lw   $t0, 4($fp)            # load d into $t1

    li   $t2, 1                 # set $t2 to 1
    beq  $t0, $t2, base_case    # if n == 1, go to base_case

    rem  $t3, $t0, 2            # $t3 = n % 2
    bnez $t3, odd_case          # if n is odd, jump to odd_case

even_case:
    div  $t0, $t0, 2            # $t0 = n / 2
    j    recursion              # jump to recursive call

odd_case:
    li   $t2, 3                 # set $t2 to 3
    mul  $t0, $t0, $t2          # $t0 = 3 * n
    addi $t0, $t0, 1            # $t0 = 3 * n + 1


recursion:
    addi $t1, $t1, 1            # d++
    sw   $t0, 4($sp)            # store n on the stack
    sw   $t1, 8($sp)            # store d on the stack
    jal  collatz                # recursive call

base_case:
    move $v0, $t1               # if n is 1, return d

end:
    # EPILOGUE
    move $sp, $fp               # restore stack pointer
    lw   $ra, ($fp)             # restore return address
    lw   $fp, -4($sp)           # restore frame pointer
    jr   $ra                    # return to caller
