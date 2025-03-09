addi $t0, $0, 12       # $t0 = 12
addi $t1, $0, 0        # $t1 = 0

loop:
add   $t1, $t1, $t0    # sum += counter
addi  $t0, $t0, -1     # counter--
bne   $t0, $0, loop    # Branch to loop if counter != 0
add   $t2, $t1, $t0
sw    $t1, 0($t2)       # Store result

# addi $t0, $0, 5     # $t0 = 5
# addi $t1, $0, 3     # $t1 = 3
# add  $t2, $t0, $t1  # $t2 = 8
