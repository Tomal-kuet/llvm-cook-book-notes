# Chapter 1 #
## How to know the availables passes in OPT ##
```
- opt --print-passes
```

    The syntex for LLVM newer version is different from the LLVM cook book. The new commands are listed below, commands from the book not list here denotes the book command still works:
```
- opt  testfile.ll -S -p instcombine -o output.ll 
- opt  testfile.ll -S -p deadargelim -o output-dead.ll
- clang -emit-llvm -S multiply.c -o multiply.ll
- opt -p mem2reg -S multiply.ll -o multiply-m2r.ll

```




## Important texts ##
### C code converted IR ###
The process of C code getting converted to IR starts with the process of lexing, wherein the
C code is broken into a token stream, with each token representing an Identifier, Literal,
Operator, and so on. This stream of tokens is fed to the parser, which builds up an abstract
syntax tree with the help of Context free grammar (CFG) for the language. Semantic analysis
is done afterwards to check whether the code is semantically correct, and then we generate
code to IR.

### IR converts to LLVM bitcode ###
The LLVM bit code file format (also known as bytecode) is actually two things: a bitstream container format and an encoding
of LLVM IR into the container format.

### Dragonegg is a gcc plugin to let gcc compiler utilize LLVM optimizations ### 

### The llgo compiler is the LLVM-based frontend for Go written in Go language only. Using this frontend, we can generate the LLVM assembly code from a program written in Go. ### 

 
 
  
 

# Chapter 2 #

