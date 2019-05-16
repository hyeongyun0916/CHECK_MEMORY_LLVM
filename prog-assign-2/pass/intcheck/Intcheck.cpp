#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Module.h"
using namespace llvm;
using namespace std;

namespace {
  struct IntcheckPass : public FunctionPass {
    static char ID;
    LLVMContext *C;
    Constant *logFunc;
    Type *VoidTy;
    Type *Int32Ty;
    bool isLogOp;

    IntcheckPass() : FunctionPass(ID) {}

    bool doInitialization(Module &M) {
      C = &(M.getContext());
      // errs() << M.getModuleIdentifier() << "\n";
      isLogOp = M.getModuleIdentifier().find("loaop.cpp") != string::npos;
      VoidTy = Type::getVoidTy(*C);
      Int32Ty = Type::getInt32Ty(*C);
      logFunc = M.getOrInsertFunction("logop", Int32Ty, Int32Ty,Int32Ty,Int32Ty, NULL);
      return true;
    }

    virtual bool runOnFunction(Function &F) {
      bool res = false;
      if (isLogOp) {
        return res;
      }
      // errs() << F << "\n";
      for (auto &B : F) {
        for (auto &I : B) {
          if(AllocaInst *allocInst = dyn_cast<AllocaInst>(&I)){
            PointerType *p = allocInst->getType();
            if (p->getElementType()->isPointerTy()) {
              errs() << I << "\t" << *(p->getElementType()) << "\n";
            }
            else if(p->getElementType()->isArrayTy()){
              Value* v = allocInst->getOperand(0);
              const DataLayout &DL = I.getModule()->getDataLayout();
              // *TypeSize = DL.getTypeStoreSizeInBits(Ty);
              
              errs() << I << "\n"
              << I.getName()
              << "\t" << (allocInst->getAllocatedType()->getArrayNumElements())
              << "\t" << DL.getTypeStoreSizeInBits(allocInst->getAllocatedType()->getArrayElementType())
              << "\t" <<*(allocInst->getAllocatedType()->getArrayElementType()) << "\n\n";
              IRBuilder<> builder(allocInst);
              builder.SetInsertPoint(&B, ++builder.GetInsertPoint());
              v = dyn_cast<Value>(&I);
              Value* args[] = {ConstantInt::get(Int32Ty, -1), ConstantInt::get(Int32Ty, -1), ConstantInt::get(Int32Ty, -1)};
              builder.CreateCall(logFunc, args);
              
            }
          }
          if (auto *op = dyn_cast<BinaryOperator>(&I)) {
            /*
            // TODO: Implement the shouldCheckOverflow() function.
            if (!shouldCheckOverflow(&I, 0))
              continue;

            errs() << "Instrument: " << I << "\n";

            // Insert call instruction *after* `op`.
            // TODO: Pass more information including operands of computations.
            IRBuilder<> builder(op);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());
            Value* args[] = {op, getLineNum(&I)};
            builder.CreateCall(logFunc, args);
            res |= true;
            */
          }
        }
      }
      return res;
    }
  };
  // RegisterPass<IntcheckPass> X("hello", "Hello World Pass");
}

char IntcheckPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerIntcheckPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new IntcheckPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerIntcheckPass);
