# author: Wonjae Kim
# UID: 117283365
# Directory ID: wkim1128

.text
    .globl isqrt

isqrt:
    # PROLOGUE
    subu $sp, $sp, 16          # make space by decreasing sp
    sw   $ra, 12($sp)          # save return address
    sw   $a0, 8($sp)           # save n
    sw   $s0, 4($sp)           # save $s0 (small)
    sw   $s1, 0($sp)           # save $s1 (large)

    # BODY
    li   $t0,   2              # set temp register to 2
    slt  $t1,   $a0,    $t0    # $t1 = 1 if n < 2, else 0
    bnez $t1,   return_n       # If n < 2, jump to base case

    srl  $a0,   $a0,    2      # n >> 2
    jal  isqrt                 # recursive call isqrt(n >> 2)
    sll  $s0,   $v0,    1      # small = result << 1
    addi $s1,   $s0,    1      # large = small + 1

    mul  $t0,   $s1,    $s1    # compute large * large
    lw   $a0,   8($sp)         # restore n
    slt  $t1,   $a0,    $t0    # check if n < large * large
    beqz $t1,   return_large   # if n >= large * large, return 'large'


    move $v0,   $s0            # return small
    j    end                   # jump to the end

return_large:
    move $v0,   $s1            # return large
    j    end                   # jump to the end

return_n:
    move $v0,   $a0            # return n

    # EPILOGUE
end:
    lw   $s1,   0($sp)         # restore $s1
    lw   $s0,   4($sp)         # restore $s0
    lw   $a0,   8($sp)         # restore n
    lw   $ra,   12($sp)        # restore return address
    addi $sp,   $sp,    16     # deallocate stack space
    jr   $ra                   # return to caller
