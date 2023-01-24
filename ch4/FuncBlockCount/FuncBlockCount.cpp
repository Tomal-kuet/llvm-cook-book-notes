#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "FunctionBlockCount"

STATISTIC(FuncBlockCounter, "Counts number of functions greeted");

namespace {

struct FuncBlockCount : public FunctionPass {
	static char ID;
	FuncBlockCount() : FunctionPass (ID) {}
	bool runOnFunction(Function &F) override {
		errs() << "Function " << F.getName() << '\n';
		return false;
		}
	};
}

char FuncBlockCount::ID = 0;
static RegisterPass<FuncBlockCount> X("funcblockcount", "Function Block Count", false, false);
