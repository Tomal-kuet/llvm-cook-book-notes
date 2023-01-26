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
- Note that load the shared object file with opt tool does not mean running the program. It will run the pass while compiling the program. In the pass we have a print statement which shows the function names.
- The ```opt``` tool can used to load shared object files with either bitcode files or llvm IR. The hello pass is loaded here from bitcode(hello.bc) and the sample.ll is in llvm IR form.

<br/><br/> 
## Using a pass result with a different pass ##
We can use another pass results in our implemented pass. The book shows and explain an example of this. But for the later versions the source code needs to be changed.
We need to overwrite the ```getAnalysisUsage``` function  and add the ```LoopInfoWrapperPass``` as the parameter in ```addRequired``` function of the object 

```
  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LoopInfoWrapperPass>();
  }
```

The reasoning is specified in the comments of the LLVM source code;
```
 /// getAnalysisUsage - This function should be overriden by passes that need
   /// analysis information to do their job.  If a pass specifies that it uses a
   /// particular analysis result to this function, it can then use the
   /// getAnalysis<AnalysisType>() function, below.
   virtual void getAnalysisUsage(AnalysisUsage &) const;
```
commands to load:
```
opt -enable-new-pm=0 -load /home/tomal/llvm_all/llvm-project/build/lib/LLVMFuncBlockCount.so -funcblockcount sample-loop-count.ll > /dev/null

opt -enable-new-pm=0 -load /home/tomal/llvm_all/llvm-project/build/lib/LLVMFuncBlockCount.so -funcblockcount sample-loop-count.ll -disable-output -debug-pass=Structure
```
Sample output:

```
Pass Arguments:  -targetlibinfo -tti -targetpassconfig -domtree -loops -funcblockcount -verify
Target Library Information
Target Transform Information
Target Pass Configuration
  ModulePass Manager
    FunctionPass Manager
      Dominator Tree Construction
      Natural Loop Information
      Function Block Count
      Module Verifier
Function main
Loop level 0 has 11 blocks
Loop level 1 has 3 blocks
Loop level 1 has 3 blocks
Loop level 0 has 15 blocks
Loop level 1 has 7 blocks
Loop level 2 has 3 blocks
Loop level 1 has 3 blocks
```

<br/><br/>

## Register a pass with pass manager ##

Following the steps described in the llvm book resulted in the following error:
```
home/tomal/llvm_all/llvm-project/llvm/include/llvm/InitializePasses.h:420:44: error: variable or field ‘initializeFuncBlockCountPass’ declared void
  420 | void initializeFuncBlockCountPass (Registry); // llvm cook book testing for pass manager registration
```
The new pass manager is different.
<br/> <br/>
This [Link](https://llvm.org/docs/WritingAnLLVMNewPMPass.html) works. It a simpler approch now to register passes with pass manager.

Follow the steps and lastly instead of ninja build i am using make. After the modifications just run ```make opt``` then write the [a.ll](./ch4/a.ll) & invoke opt with ```opt -disable-output a.ll -p=helloworld```

## Writing an analysis pass ##

Same as the previous the exact code from the book works.
commands to load:
```
opt -enable-new-pm=0 -load /home/tomal/llvm_all/llvm-project/build/lib/LLVMOpCodeCounter.so -opcodeCounter anlysis.bc
```
Sample output:
```
Function func
add: 3
alloca: 5
br: 8
icmp: 3
load: 10
ret: 1
select: 1
store: 8
zext: 1
```

Commands differ from the book:
```
opt -enable-new-pm=0 -aa-eval -disable-output anlysis.bc
```

## Important notes ##

The LLVM code representation is designed to be used in three different forms: as an in-memory compiler IR, as an on-disk bitcode representation (suitable for fast loading by a Just-In-Time compiler), and as a human readable assembly language representation. 

