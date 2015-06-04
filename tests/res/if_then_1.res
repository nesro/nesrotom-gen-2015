TM  simulation (enter h for help)...
Enter command: Tracing now on.
Enter command: Printing instruction count now on.
Enter command:     0:     IN  0,0,0
Enter value for IN instruction:     1:     ST  0,  0(6)
    2:    LDC  0,666(0)
    3:    OUT  0,0,0
OUT: 666
    4:     LD  0,  0(6)
    5:    LDC  1, 10(0)
    6:    SUB  0,1,0
    7:    JGT  0,  2(7)
   10:    LDC  0,  1(0)
   11:    JEQ  0, 15(6)
   12:    LDC  0,111(0)
   13:    OUT  0,0,0
OUT: 111
   14:    LDC  0,777(0)
   15:    OUT  0,0,0
OUT: 777
   16:   HALT  0,0,0
HALT: 0,0,0
Number of instructions executed = 15
Halted
Enter command: Simulation done.
