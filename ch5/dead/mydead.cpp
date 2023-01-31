#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Scalar.h"
using namespace llvm;

namespace {
struct MYADCE : public FunctionPass {
  static char ID; // Pass identification, replacement for
  MYADCE() : FunctionPass(ID) {}
  bool runOnFunction(Function &F) override;
  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
  }
};
} // namespace

bool MYADCE::runOnFunction(Function &F) {

  if (skipFunction(F))
    return false;

  SmallPtrSet<Instruction *, 32> Alive;
  SmallVector<Instruction *, 32> Worklist;

  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    Instruction *ins = &*I;
    errs() << "Instructions"
           << ":";
    errs() << *ins << "\n";
    if (ins->isTerminator() || ins->isDebugOrPseudoInst() || ins->isEHPad() ||
        ins->mayHaveSideEffects()) {
      Alive.insert(ins);
      Worklist.push_back(ins);
    }
  }

  // Propagate liveness backwards to operands.
  while (!Worklist.empty()) {
    Instruction *Curr = Worklist.pop_back_val();
    for (Use &OI : Curr->operands()) {
      if (Instruction *Inst = dyn_cast<Instruction>(OI)) {
        if (Alive.insert(Inst).second)
          Worklist.push_back(Inst);
      }
    }
  }
  errs() << "\n passed the while loop\n";
  // // the instructions which are not in live set are considered dead in this
  // // pass.The instructions which do not effect the control flow,
  // //   return value and do not have any side effects are hence
  // // deleted.
  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    Instruction *ins = &*I;
    if (!Alive.count(ins)) {
      Worklist.push_back(ins);
      ins->dropAllReferences();
    }
  }
  errs() << "\n passed the for loop\n";

  for (Instruction *&I : Worklist) {
    I->eraseFromParent();
  }
   errs() << "\n passed all\n";
  
  return !Worklist.empty();
  
}
char MYADCE::ID = 0;
static RegisterPass<MYADCE> X("dead", "eliminate dead code");