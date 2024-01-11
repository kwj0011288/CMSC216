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
.word 1, 3, 5, 7, 9, 0
.text
.globl collatz

collatz:
    # PROLOGUE
    addi $sp, $sp, -24         # Allocate stack space for $ra, $a1, and other variables
    sw   $ra, 20($sp)          # Save return address
    sw   $a1, 16($sp)          # Save depth (d)

    # Check if n is 1
    li   $t1, 1
    beq  $a0, $t1, end         # If n == 1, return d

    # Check if n is even (n % 2 == 0)
    andi $t0, $a0, 1           # $t0 = n % 2 (0 if even, 1 if odd)
    beq  $t0, $zero, even      # If n is even, jump to 'even'

    # n = 3 * n + 1 for odd n
    li   $t1, 3
    mul  $a0, $a0, $t1         # n = 3 * n
    addi $a0, $a0, 1           # n = n + 1
    j    recurse               # Jump to recurse for recursive call

even:
    # n = n / 2 for even n
    srl  $a0, $a0, 1           # n = n / 2

recurse:
    # d++
    lw   $t1, 16($sp)          # Load d
    addi $t1, $t1, 1           # Increment d
    move $a1, $t1              # Update $a1 with the new d value
    jal  collatz               # Recursive call to collatz
    lw   $a1, 16($sp)          # Restore d after return from recursion

    # EPILOGUE
end:
    move $v0, $a1              # Move result to $v0
    lw   $ra, 20($sp)          # Restore return address
    addi $sp, $sp, 24          # Deallocate stack space
    jr   $ra                   # Return to caller
