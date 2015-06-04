TM  simulation (enter h for help)...
Enter command: Tracing now on.
Enter command: Printing instruction count now on.
Enter command:     0:     IN  0,0,0
Enter value for IN instruction:     1:     ST  0,  0(6)
    2:    LDC  0, 10(0)
    3:     LD  1,  0(6)
    4:    ADD  0,0,1
    5:     ST  0,  1(6)
    6:     LD  0,  1(6)
    7:    OUT  0,0,0
OUT: 20
    8:     LD  0,  0(6)
    9:    LDC  1,  5(0)
   10:    SUB  0,0,1
   11:     ST  0,  1(6)
   12:     LD  0,  1(6)
   13:    OUT  0,0,0
OUT: 5
   14:    LDC  0,  2(0)
   15:     LD  1,  0(6)
   16:    MUL  0,0,1
   17:     ST  0,  1(6)
   18:     LD  0,  1(6)
   19:    OUT  0,0,0
OUT: 20
   20:     LD  0,  0(6)
   21:    LDC  1,  2(0)
   22:    DIV  0,0,1
   23:     ST  0,  1(6)
   24:     LD  0,  1(6)
   25:    OUT  0,0,0
OUT: 5
   26:     LD  0,  0(6)
   27:    LDC  1, 20(0)
   28:    SUB  0,1,0
   29:    JGT  0,  2(7)
   32:    LDC  0,  1(0)
   33:    JEQ  0, 37(6)
   34:    LDC  0,  1(0)
   35:    OUT  0,0,0
OUT: 1
   36:    LDA  7, 39(6)
   39:     LD  0,  0(6)
   40:    LDC  1,  5(0)
   41:    SUB  0,1,0
   42:    JGT  0,  2(7)
   43:    LDC  0,  0(0)
   44:    LDA  7,  1(7)
   46:    JEQ  0, 50(6)
   50:    LDC  0,  2(0)
   51:    OUT  0,0,0
OUT: 2
   52:     LD  0,  0(6)
   53:    LDC  1, 10(0)
   54:    SUB  0,1,0
   55:    JGT  0,  2(7)
   56:    LDC  0,  0(0)
   57:    LDA  7,  1(7)
   59:    JEQ  0, 63(6)
   63:    LDC  0,  3(0)
   64:    OUT  0,0,0
OUT: 3
   65:     LD  0,  0(6)
   66:    LDC  1, 10(0)
   67:    SUB  0,1,0
   68:    JEQ  0,  2(7)
   71:    LDC  0,  1(0)
   72:    JEQ  0, 76(6)
   73:    LDC  0,  4(0)
   74:    OUT  0,0,0
OUT: 4
   75:    LDA  7, 78(6)
   78:     LD  0,  0(6)
   79:    LDC  1, 20(0)
   80:    SUB  0,1,0
   81:    JEQ  0,  2(7)
   82:    LDC  0,  0(0)
   83:    LDA  7,  1(7)
   85:    JEQ  0, 89(6)
   89:    LDC  0,  5(0)
   90:    OUT  0,0,0
OUT: 5
   91:     LD  0,  0(6)
   92:    LDC  1, 20(0)
   93:    SUB  0,1,0
   94:    JNE  0,  2(7)
   97:    LDC  0,  1(0)
   98:    JEQ  0,102(6)
   99:    LDC  0,  6(0)
  100:    OUT  0,0,0
OUT: 6
  101:    LDA  7,104(6)
  104:     LD  0,  0(6)
  105:    LDC  1, 10(0)
  106:    SUB  0,1,0
  107:    JNE  0,  2(7)
  108:    LDC  0,  0(0)
  109:    LDA  7,  1(7)
  111:    JEQ  0,115(6)
  115:    LDC  0,  7(0)
  116:    OUT  0,0,0
OUT: 7
  117:     LD  0,  0(6)
  118:    LDC  1,  5(0)
  119:    SUB  0,1,0
  120:    JLT  0,  2(7)
  123:    LDC  0,  1(0)
  124:    JEQ  0,128(6)
  125:    LDC  0,  8(0)
  126:    OUT  0,0,0
OUT: 8
  127:    LDA  7,130(6)
  130:     LD  0,  0(6)
  131:    LDC  1, 20(0)
  132:    SUB  0,1,0
  133:    JLT  0,  2(7)
  134:    LDC  0,  0(0)
  135:    LDA  7,  1(7)
  137:    JEQ  0,141(6)
  141:    LDC  0,  9(0)
  142:    OUT  0,0,0
OUT: 9
  143:     LD  0,  0(6)
  144:    LDC  1, 10(0)
  145:    SUB  0,1,0
  146:    JLT  0,  2(7)
  147:    LDC  0,  0(0)
  148:    LDA  7,  1(7)
  150:    JEQ  0,154(6)
  154:    LDC  0, 10(0)
  155:    OUT  0,0,0
OUT: 10
  156:     LD  0,  0(6)
  157:    LDC  1, 20(0)
  158:    SUB  0,1,0
  159:    JGE  0,  2(7)
  162:    LDC  0,  1(0)
  163:    JEQ  0,167(6)
  164:    LDC  0, 11(0)
  165:    OUT  0,0,0
OUT: 11
  166:    LDA  7,169(6)
  169:     LD  0,  0(6)
  170:    LDC  1,  5(0)
  171:    SUB  0,1,0
  172:    JGE  0,  2(7)
  173:    LDC  0,  0(0)
  174:    LDA  7,  1(7)
  176:    JEQ  0,180(6)
  180:    LDC  0, 12(0)
  181:    OUT  0,0,0
OUT: 12
  182:     LD  0,  0(6)
  183:    LDC  1, 10(0)
  184:    SUB  0,1,0
  185:    JGE  0,  2(7)
  188:    LDC  0,  1(0)
  189:    JEQ  0,193(6)
  190:    LDC  0, 13(0)
  191:    OUT  0,0,0
OUT: 13
  192:    LDA  7,195(6)
  195:     LD  0,  0(6)
  196:    LDC  1,  5(0)
  197:    SUB  0,1,0
  198:    JLE  0,  2(7)
  201:    LDC  0,  1(0)
  202:    JEQ  0,206(6)
  203:    LDC  0, 14(0)
  204:    OUT  0,0,0
OUT: 14
  205:    LDA  7,208(6)
  208:     LD  0,  0(6)
  209:    LDC  1, 20(0)
  210:    SUB  0,1,0
  211:    JLE  0,  2(7)
  212:    LDC  0,  0(0)
  213:    LDA  7,  1(7)
  215:    JEQ  0,219(6)
  219:    LDC  0, 15(0)
  220:    OUT  0,0,0
OUT: 15
  221:     LD  0,  0(6)
  222:    LDC  1, 10(0)
  223:    SUB  0,1,0
  224:    JLE  0,  2(7)
  227:    LDC  0,  1(0)
  228:    JEQ  0,232(6)
  229:    LDC  0, 16(0)
  230:    OUT  0,0,0
OUT: 16
  231:    LDA  7,234(6)
  234:   HALT  0,0,0
HALT: 0,0,0
Number of instructions executed = 171
Halted
Enter command: Simulation done.
