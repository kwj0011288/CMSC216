# author: Wonjae Kim
# UID: 117283365
# Directory ID: wkim1128

.text
    .globl reverse_prefix_sum

reverse_prefix_sum:
    # PROLOGUE
    subu $sp, $sp, 12         # make space by decreasing sp
    sw   $ra, 8($sp)          # save return address
    sw   $a0, 4($sp)          # save arr
    sw   $s0, 0($sp)          # save r
    
    # BODY
    lw   $t0, 0($a0)          # load array element into $t0
    li   $t1, -1              # load -1 into $t1 to compare
    beq  $t0, $t1, end        # if current element is -1, go to end

    addiu $a0, $a0, 4         # increment for next element
    jal   reverse_prefix_sum  # recursive call

    lw   $t0, -4($a0)         # load previous element
    addu $v0, $v0, $t0        # add element to return value
    sw   $v0, -4($a0)         # store the sum back into the array

    # EPILOGUE
    lw   $s0, 0($sp)          # restore $s0
    lw   $a0, 4($sp)          # restore arr pointer
    lw   $ra, 8($sp)          # restore return address
    addi $sp, $sp, 12         # deallocate stack space
    jr   $ra                  # return

end:
    li   $v0, 0               # set return value to 0
    lw   $s0, 0($sp)          # restore $s0
    lw   $a0, 4($sp)          # restore arr pointer
    lw   $ra, 8($sp)          # restore return address
    addi $sp, $sp, 12         # deallocate stack space
    jr   $ra                  # return
