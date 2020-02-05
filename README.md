# Embedded System P1
This is a simulated ARM compiler using a petri net diagram as a guide.
There are 16 registers (R0-R15) and 16 locations in data memory. These are initally declared in the registers.txt and datamemory.txt files. The instructions are located in the instructions.txt file. 

**Available Instructions:

ST: Store value in a register to memory at specified location with an offset value

ADD: Adds 2 values together and store them in a destination register

SUB: Subtracts 2 values together and store them in a destination register

MUL: Multiplies 2 values together and store them in a destination register. This takes an extra cycle to complete
