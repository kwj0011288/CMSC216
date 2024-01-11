.data

arrow: .asciiz " -> "

.text

main:
    li      $sp,        0x7ffffffc      # initialize $sp

# PROLOGUE
    subu    $sp,        $sp,        8   # expand stack by 8 bytes
    sw      $ra,        8($sp)          # push $ra (ret addr, 4 bytes)
    sw      $fp,        4($sp)          # push $fp (4 bytes)
    addu    $fp,        $sp,        8   # set $fp to saved $ra

    subu    $sp,        $sp,        12  # save s0 and s1 on stack before using them
    sw      $s0,        12($sp)         # push $s0
    sw      $s1,        8($sp)          # push $s1
    sw      $s2,        4($sp)          # push $s2

    la      $s0,        xarr            # load address to s0

main_for:
    lw      $s1,        ($s0)           # use s1 for xarr[i] value
    li      $s2,        0               # use s2 for initial depth (steps)
    beqz    $s1,        main_end        # if xarr[i] == 0, stop.

# save args on stack rightmost one first
    subu    $sp,        $sp,        8   # save args on stack
    sw      $s2,        8($sp)          # save depth
    sw      $s1,        4($sp)          # save xarr[i]

    li      $v0,        1
    move    $a0,        $s1             # print_int(xarr[i])
    syscall 

    li      $v0,        4               # print " -> "
    la      $a0,        arrow
    syscall 

    jal     collatz                     # result = collatz(xarr[i])

    move    $a0,        $v0             # print_int(result)
    li      $v0,        1
    syscall 

    li      $a0,        10              # print_char('\n')
    li      $v0,        11
    syscall 

    addu    $s0,        $s0,        4   # make s0 point to the next element

    lw      $s2,        8($sp)          # save depth
    lw      $s1,        4($sp)          # save xarr[i]
    addu    $sp,        $sp,        8   # save args on stack
    j       main_for

main_end:
    lw      $s0,        12($sp)         # push $s0
    lw      $s1,        8($sp)          # push $s1
    lw      $s2,        4($sp)          # push $s2

# EPILOGUE
    move    $sp,        $fp             # restore $sp
    lw      $ra,        ($fp)           # restore saved $ra
    lw      $fp,        -4($sp)         # restore saved $fp
    jr      $ra                         # return to kernel
.data

# array terminated by 0 (which is not part of the array)
xarr:
.word 2, 4, 6, 8, 10, 0
    .text
    .globl collatz

collatz:
    # Prologue
    subu    $sp, $sp, 24         # Allocate stack space for 6 words
    sw      $ra, 20($sp)         # Save return address
    sw      $fp, 16($sp)         # Save frame pointer
    sw      $a0, 12($sp)         # Save argument n
    sw      $a1, 8($sp)          # Save argument d
    addu    $fp, $sp, 20         # Set up frame pointer

    # Check if n == 1
    lw      $a0, 12($fp)         # Load n
    li      $t0, 1
    beq     $a0, $t0, return_d   # if (n == 1) return d

    # Increment d for the recursive call
    lw      $a1, 8($fp)          # Load depth (d)
    addi    $a1, $a1, 1          # d = d + 1

    # Check if n is even or odd
    andi    $t1, $a0, 1
    beq     $t1, $zero, even     # if (n % 2 == 0) goto even

    # Odd case
odd:
    li      $t2, 3
    mul     $t3, $a0, $t2        # t3 = 3 * n
    addi    $t3, $t3, 1          # t3 = 3 * n + 1
    move    $a0, $t3
    jal     collatz              # Recursive call with n = 3 * n + 1, d = d + 1
    j       end_collatz

    # Even case
even:
    srl     $t4, $a0, 1          # t4 = n / 2
    move    $a0, $t4
    jal     collatz              # Recursive call with n = n / 2, d = d + 1

end_collatz:
    # Epilogue
    lw      $ra, 20($sp)         # Restore return address
    lw      $fp, 16($sp)         # Restore frame pointer
    addu    $sp, $sp, 24         # Deallocate stack space
    jr      $ra                  # Return

return_d:
    lw      $a1, 8($sp)          # Load depth (d)
    move    $v0, $a1             # Return the depth d
    j       end_collatz
