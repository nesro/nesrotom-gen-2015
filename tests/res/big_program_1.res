TM  simulation (enter h for help)...
Enter command: Tracing now on.
Enter command: Printing instruction count now on.
Enter command:     0:    LDC  0,  0(0)
    1:    OUT  0,0,0
OUT: 0
    2:     IN  0,0,0
Enter value for IN instruction:     3:     ST  0,  0(6)
    4:     IN  0,0,0
Enter value for IN instruction:     5:     ST  0,  1(6)
    6:     LD  0,  0(6)
    7:    OUT  0,0,0
OUT: 10
    8:     LD  0,  1(6)
    9:    OUT  0,0,0
OUT: 20
   10:    LDC  0, 10(0)
   11:     ST  0,  2(6)
   12:    LDC  0, 10(0)
   13:     ST  0,  3(6)
   14:     LD  0,  0(6)
   15:    LDC  1, 12(0)
   16:    SUB  0,1,0
   17:    JGT  0,  2(7)
   20:    LDC  0,  1(0)
   21:    JEQ  0, 64(6)
   22:     LD  0,  0(6)
   23:     LD  1,  2(6)
   24:    ADD  0,0,1
   25:     LD  1,  0(6)
   26:    MUL  0,0,1
   27:     ST  0,  2(6)
   28:     LD  0,  0(6)
   29:     LD  1,  2(6)
   30:    ADD  0,0,1
   31:     LD  1,  1(6)
   32:    MUL  0,0,1
   33:     ST  0,  3(6)
   34:     LD  0,  2(6)
   35:    OUT  0,0,0
OUT: 200
   36:     LD  0,  3(6)
   37:    OUT  0,0,0
OUT: 4200
   38:     LD  0,  2(6)
   39:     LD  1,  3(6)
   40:    ADD  0,0,1
   41:    LDC  1, 50(0)
   42:    SUB  0,1,0
   43:    JLT  0,  2(7)
   46:    LDC  0,  1(0)
   47:    JEQ  0, 51(6)
   48:    LDC  0,123(0)
   49:    OUT  0,0,0
OUT: 123
   50:    LDA  7, 53(6)
   53:    LDC  0,  1(0)
   54:    OUT  0,0,0
OUT: 1
   55:    LDC  0,  1(0)
   56:     LD  1,  3(6)
   57:    ADD  0,0,1
   58:     ST  0,  3(6)
   59:    LDC  0,  1(0)
   60:     LD  1,  0(6)
   61:    ADD  0,0,1
   62:     ST  0,  0(6)
   63:    LDA  7, 14(6)
   14:     LD  0,  0(6)
   15:    LDC  1, 12(0)
   16:    SUB  0,1,0
   17:    JGT  0,  2(7)
   20:    LDC  0,  1(0)
   21:    JEQ  0, 64(6)
   22:     LD  0,  0(6)
   23:     LD  1,  2(6)
   24:    ADD  0,0,1
   25:     LD  1,  0(6)
   26:    MUL  0,0,1
   27:     ST  0,  2(6)
   28:     LD  0,  0(6)
   29:     LD  1,  2(6)
   30:    ADD  0,0,1
   31:     LD  1,  1(6)
   32:    MUL  0,0,1
   33:     ST  0,  3(6)
   34:     LD  0,  2(6)
   35:    OUT  0,0,0
OUT: 2321
   36:     LD  0,  3(6)
   37:    OUT  0,0,0
OUT: 46640
   38:     LD  0,  2(6)
   39:     LD  1,  3(6)
   40:    ADD  0,0,1
   41:    LDC  1, 50(0)
   42:    SUB  0,1,0
   43:    JLT  0,  2(7)
   46:    LDC  0,  1(0)
   47:    JEQ  0, 51(6)
   48:    LDC  0,123(0)
   49:    OUT  0,0,0
OUT: 123
   50:    LDA  7, 53(6)
   53:    LDC  0,  1(0)
   54:    OUT  0,0,0
OUT: 1
   55:    LDC  0,  1(0)
   56:     LD  1,  3(6)
   57:    ADD  0,0,1
   58:     ST  0,  3(6)
   59:    LDC  0,  1(0)
   60:     LD  1,  0(6)
   61:    ADD  0,0,1
   62:     ST  0,  0(6)
   63:    LDA  7, 14(6)
   14:     LD  0,  0(6)
   15:    LDC  1, 12(0)
   16:    SUB  0,1,0
   17:    JGT  0,  2(7)
   18:    LDC  0,  0(0)
   19:    LDA  7,  1(7)
   21:    JEQ  0, 64(6)
   64:   HALT  0,0,0
HALT: 0,0,0
Number of instructions executed = 110
Halted
Enter command: Simulation done.
