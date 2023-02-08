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

-----------------------------------------------------------------------------------------------------------
<br/><br/> 

# Chapter 5 #
There were some compilation bugs for the given source code in the book. The modified pass is documented in this [folder](./ch5/dead/).

Sample output:
```
Instructions:  %1 = call i32 @strlen(i8* null)
Instructions:  ret void

 passed the while loop

 passed the for loop

 passed all
; ModuleID = 'testcode.ll'
source_filename = "testcode.ll"

; Function Attrs: nounwind memory(read)
declare i32 @strlen(i8*) #0

define void @test() {
  %1 = call i32 @strlen(i8* null)
  ret void
}

attributes #0 = { nounwind memory(read) }
```
## Inlining transformation pass ##
Skipped

## Memory optimization pass ##
This test is based on a existing mass call `memcpyopt`, which tries to replace the `memcpy` function call to `memset` for speed up. If we compare the [memcopytest](./ch5/memcopytest.ll) file and sample output below we can see the `memcpy` is repalced my `memset`. Commands to run the module:
```
 opt -enable-new-pm=0 -memcpyopt -S memcopytest.ll
```
Sample output:
```
; ModuleID = 'memcopytest.ll'
source_filename = "memcopytest.ll"

@cst = internal constant [3 x i32] [i32 -1, i32 -1, i32 -1], align 4

; Function Attrs: nounwind
declare void @foo(i32*) #0

; Function Attrs: nounwind
define void @test1() #0 {
  %arr = alloca [3 x i32], align 4
  %arr_i8 = bitcast [3 x i32]* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* align 4 %arr_i8, i8 -1, i64 12, i1 false)
  %arraydecay = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i64 0, i64 0
  call void @foo(i32* %arraydecay) #0
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #1

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #2

attributes #0 = { nounwind }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: write) }
```

## Combining LLVM IR ##
The example shown in the book is alredy implemented in the current version.If we run the comment `opt -enable-new-pm=0 -instcombine -S combine-test.ll` on the [file](./ch5/combine-test.ll) we can see the below output same as the book:
```
; ModuleID = 'combine-test.ll'
source_filename = "combine-test.ll"

define i32 @test19(i32 %x, i32 %y, i32 %z) {
  %xor1 = xor i32 %y, %z
  %1 = and i32 %xor1, %x
  ret i32 %1
}
```
See the InstCombineAndOrXor.cpp file in LLVM repository to understand what kind of combining can be done. These depend on the mathmatical reductions.

## Transforming and optimizing loops ##
Command: `opt -enable-new-pm=0 -licm -S testlicm.ll `
Sample-output:
```
; ModuleID = 'testlicm.ll'
source_filename = "testlicm.ll"

define void @testfunc(i32 %i) {
  %i2 = mul i32 %i, 17
  br label %Loop

Loop:                                             ; preds = %Loop, %0
  %j = phi i32 [ 0, %0 ], [ %Next, %Loop ]
  %Next = add i32 %j, %i2
  %cond = icmp eq i32 %Next, 0
  br i1 %cond, label %Out, label %Loop

Out:                                              ; preds = %Loop
  ret void
}
```
Command for deletion loop pass:
```opt -enable-new-pm=0 -loop-deletion -S deletetest.ll```
## Important notes ##

-----------------------------------------------------------------------------------------------------------
<br/><br/> 

# Chapter 6 #
The dag commands work perfectly with `llc`. Note that the output does not exactly match with the book.
Important functions for instrumentation:
- `BuildMI` build arbitrary machine instructions.
- `getBasicBlock` to get the current basic block of the instructions.
- `std::vector<MachineBasicBlock *> Predecessors` to keep track of the Predecessors.
-  `std::vector<MachineBasicBlock *> Successors` to keep track of the Successors.
-  `insert` unction should be added to insert a machine instruction into the basic
block
- `SplitCriticalEdge()` splits critical edges from current block to the successor.

Most of this chapter is theory, gives you an idea how selectionDAG working from IR to machine code.

## Important notes ##
After the IR transformations and optimizations are over, the LLVM IR instruction passes
through a Selection DAG node incarnation. Selection DAG nodes are created by the
SelectionDAGBuilder class. The SelectionDAGBuilder::visit() function call from
the SelectionDAGISel class visits each IR instruction for creating an SDAGNode node.
The method that handles an SDiv instruction is SelectionDAGBuilder::visitSDiv . It
requests a new SDNode node from the DAG with the ISD::SDIV opcode, which then becomes
a node in the DAG

-----------------------------------------------------------------------------------------------------------
<br/><br/> 

# Chapter 6 #
The dag commands 