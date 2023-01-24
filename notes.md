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

 
 <br/><br/> 

-----------------------------------------------------------------------------------------------------------
<br/><br/> 

# Chapter 2 #
Quickly went over this chapter for now. It demostrates AST, lexical parser for a toy language. If required will revisit this chapter.

 
 <br/><br/> 

-----------------------------------------------------------------------------------------------------------
<br/><br/> 

# Chapter 3 #
Quickly went over this chapter for now. It extends from the previous chapter. If required will revisit this chapter.

 
 <br/><br/> 

-----------------------------------------------------------------------------------------------------------
<br/><br/> 

# Chapter 4 #
Below link is helpful to understand LLVM IR basics along with other things.
[LLVM IR](https://llvm.org/docs/LangRef.html)

Below are modified commands for new llvm version to see debug outputs of different passes run:
```
opt --debug-pass-manager=verbose  -O3 -S example.ll -o e-opt1.ll

```
<br/><br/>
Helo world pass loading
```
opt -enable-new-pm=0 -load /home/tomal/llvm_all/llvm-project/build/lib/LLVMHello.so -hello hello.bc > /dev/null
```
<br/><br/>
FunctionBlock count pass loading & simple.c compilation steps:
```
clang -o0 -S -emit-llvm sample.c -o sample.ll

opt -enable-new-pm=0 -load /home/tomal/llvm_all/llvm-project/build/lib/LLVMFuncBlockCount.so -funcblockcount sample.ll > /dev/null
```
<br/><br/>

## Steps to develop you own pass ##
- First we have to create a folder in <"llvm root direcotry">/llvm/lib/Transforms with pass name
- Then we have to create a CMakeLists.txt file with in the directory. Example of are in [Example CMake](ch4/FuncBlockCount/CMakeLists.txt)
- Lastly we have to create a pass itself. It will be a cpp file. [FunctionBlockcount cpp file](ch4/FuncBlockCount/FuncBlockCount.cpp)
- Take a look at the [Link](https://llvm.org/docs/WritingAnLLVMPass.html) to understand the step by step explanation.
- While naming the pass and .so (in CMake) file, we need to give unique names. If there is a issue naming it most likely the CMake files or Registerpass function call names.
<br/><br/> 
## Important notes ##

The LLVM code representation is designed to be used in three different forms: as an in-memory compiler IR, as an on-disk bitcode representation (suitable for fast loading by a Just-In-Time compiler), and as a human readable assembly language representation. 

