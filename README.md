# fixedpt
This module provides fixedpt<int> implementation to perform basic arithmetic on real numbers using nonfloating hardware. 
It can be used for faster deploying of embedded software to Reduced Instruction Set Computing (RISC) processors (like ARM). 
It has inbuilt design to convert floating point arithmetic calculations to use generic fixed point arithmetic implemented on integer hardware. The goal is to have a systematic model providing reliable, faster, cheaper solution for implementing floating point in software written on RISC platforms.

## Background

Since the advent of floating point arithmetic units and single instruction arithmetic operations on them, the computational freedom & possibility has greatly enhanced with combined ease of use. Floating point numbers work by reserving some of the storage bits for tracking exponent, thereby reducing the available bits for storing precision.

While having this in hardware vastly improves development time, there is renewed focus on Reduced Instruction Set Computing (RISC), due to its simpler form factor and better efficiencies, especially in devices designed for mobility. The fastest growing demand for processors is in smart phones, which mostly employ RISC processors like ARM. RISC architectures enable reduced form factor, thereby requiring lower power requirements for the same design capability.

Most RISC architectures, by way of their reduced instruction sets, do not possess ability to handle floating point arithmetic. For this they provide bit shift operations, which combine with other arithmetic operations in the same cycle. These bit shift instructions, combined with arithmetic operations, are to be used to perform the functions of floating point arithmetic.
	e.g. ADD{cond}{S} Rd, Rn, <Operand2> N Z C V Rd := Rn + Operand2
	where Operand2 can be any shift result, like, 	Rm, LSL #<shift> Allowed shifts 0-31.

With the movement to RISC architectures, the responsibility for the task of tracking of bits is now on the software application. Due to lack of inbuilt floating point support, software writers end up having to spend huge efforts, impacting both time to market and costs, while developing software for RISC platforms.

## Overview
It contains two modules to store real numbers:
* class floatpt; contains wrapper over 32-bit float, providing basic arithmetic functionality, to be consistent with fixedpt.
* class fixedpt<int q>; stores real numbers as p.q form in 32-integer storage, with q bits of binary precision (henceforth, precision is defined thus). e.g. 16.16 is the standard fixed point familiar to embedded programmers, and 32.0 is the regular int
Both the classes can be extended to support more arithmetic operations. Contributions are welcome (mailto:atif.hussain@gmail.com)

## Benefits
* The declaration of dimension of the variable at only one place, simplifies the ease-of-use and chance of error, thereby reduces debugging costs and time-to-completion. 
* As long as 1 variable is used for 1 purpose, typically there is a reasonable magnitude it is required to store, in almost 95+% cases. The restriction is that the same variable must not be reused for multiple purposes. 
* Pre-identifying magnitude using floatpt wrapper accurately identifies the kind of fixed point representation that'll work, e.g. 16dot16 or 1dot31 etc. that is needed to model the floating number, thereby saving hits & trials, that generally result in precious time lost in project delivery. 

## Usage

For each float variable to migrate to fixed point, change it to use floatpt, and make the code to use only predefined arithmetic variant functions (e.g. currently '+' is not supported, instead use '+='). It is a simple wrapper to track the variable through its life, to identify max magnitude, used to set the fixedpt precision that can be stored in a 32-bit int without overflowing. Any standard wrapper on float basic type will work. What is provided here is a very skeletal implementation. For a better comprehensive wrapper, author(s) recommend https://github.com/jehugaleahsa/primitive which should be modified to output magnitude. 

Once magnitude is identified, compute possible precision, q <= 31 - digits_to_store_magnitude. Then convert the floatpt variable to fixedpt<_q> . This would define it as a fixed point number of declared precision, and use it as so for all its computations. 

Above changes are to be done only once for each variable - at the point of declaration. Computations itself should be supported by the included module. 

## Tips & Rules

1. Create 1 float variable for 1 purpose, ie. for each 1 occurence in code.
 1float<->1purpose ensures they are logically consistent and likely to work with same magnitude fixedpt. 
 1float<->1code ensures each declaration can be mapped to single sized precision to use for fixedpt.

2. Do not reuse float variables to share storage. Consider:
    float i;
    i = ...
    some computations on i
    i = .. reuse i, reassigning something else
    some more computations on i
    //free i
 Here, same variable i is reused for different purposes, potentially of varying magnitudes e.g. i1>100000 and i2< 0.00001 where i1±i2 cannnot be differentiated. Here, i1,i2 need different q's to retain accuracy.

3. Do not create float variables within loop. 
    float a[100] = ...initialized
    for (int i=0; i<100; i++) {
      float c = a[i];
      some computations on c
    }
 Here, a new float c is created in each cycle, and no single magnitude can be defined to convert to fixedpt. So instead, create c outside the loop, and reuse (for same purpose) in each loop iteration. 
    float a[100] = ...initialized
    float c;
    for (int i=0; i<100; i++) {
      c = a[i];
      some computations on c
    }
    //free c

4. For functions called in a loop, avoid creating internal float variables that'll have different magnitudes each time the code is run. 

5. When rules 3 & 4 above are skipped, and local variables created for easier readability, 
magnitude finding needs to account for magnitude across all iterations (var purpose is same across calls). fixedpt can still be used as is, as long as rule 2 is followed and variable not reused for different purposes. 

6. If a declared float, in iterative calls, is used for too wide variety of magnitudes (eg. distant to next galaxy and interatomic distances), converting to fixedpt will produce artifacts of precision loss (also in float a = 100000.00001f). In such cases, fixedpt cannot be used, and code needs to be reengineered or use double float. 
