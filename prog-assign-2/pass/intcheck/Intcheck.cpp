#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;
using namespace std;

namespace {
struct IntcheckPass : public FunctionPass {
  static char ID;
  LLVMContext *C;
  Constant *logFunc;
  Type *VoidTy;
  Type *Int32Ty;
  Type *Int128Ty;
  bool isLogOp;

  IntcheckPass() : FunctionPass(ID) {}

  bool doInitialization(Module &M) {
    C = &(M.getContext());
    // errs() << M.getModuleIdentifier() << "\n";
    isLogOp = M.getModuleIdentifier().find("loaop.cpp") != string::npos;
    VoidTy = Type::getVoidTy(*C);
    Int32Ty = Type::getInt32Ty(*C);
    Int128Ty = Type::getInt128Ty(*C);
    logFunc = M.getOrInsertFunction("checkMemory", VoidTy, Int32Ty, Int32Ty,
                                    Int32Ty, Int128Ty, NULL);
    return true;
  }

  Value *getLineNum(Instruction *I) {
    const DebugLoc debugLoc = I->getDebugLoc();
    unsigned line = -1;
    if (debugLoc)
      line =  debugLoc.getLine();
      // errs() << *I << "\n" << debugLoc.getLine() << "\tdebugloc\n";
      // return ConstantInt::get(Int32Ty, debugLoc->getLine());
    return ConstantInt::get(Int32Ty, line);
  }

  Type *getElementType(Type *type) {
    if (PointerType *pt = dyn_cast<PointerType>(type)) {
      return getElementType(pt->getElementType());
    } else {
      return type;
    }
  }

  virtual bool runOnFunction(Function &F) {
    bool res = false;
    if (isLogOp) {
      return res;
    }
    // errs() << F << "\n";
    for (auto &B : F) {
      for (auto &I : B) {
        const DataLayout &DL = I.getModule()->getDataLayout();
        
        // errs() << I << "\n";
        if (AllocaInst *allocInst = dyn_cast<AllocaInst>(&I)) {
          PointerType *p = allocInst->getType();
          // errs() << I << "\nalloca type: " << *allocInst->getAllocatedType()
          // << "\n";
          if (p->getElementType()->isPointerTy()) {
            // errs() << "pointer: " << I << "\t" << *(p->getElementType()) <<
            // "\n";
          } else if (allocInst->getAllocatedType()->isArrayTy()) {
            // Value *v = allocInst->getOperand(0);
            // *TypeSize = DL.getTypeStoreSizeInBits(Ty);
            /*
            errs() << I << "\n"
                   << I.getName() << "\t"
                   << (allocInst->getAllocatedType()->getArrayNumElements())
                   << "\t"
                   << DL.getTypeStoreSizeInBits(
                          allocInst->getAllocatedType()->getArrayElementType())
                   << "\t"
                   << *(allocInst->getAllocatedType()->getArrayElementType())
                   << "\n\n";
            */
            // errs() << "alloca\n";
            IRBuilder<> builder(allocInst);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());
            uint64_t arraySize =
                allocInst->getAllocatedType()->getArrayNumElements();
            uint64_t elementSize = DL.getTypeStoreSizeInBits(
                allocInst->getAllocatedType()->getArrayElementType());
            unsigned __int128 totalSize = arraySize * elementSize;
            Value *v = dyn_cast<Value>(&I);
            // errs() << I << "\n";
            Value *args[] = {getLineNum(&I), ConstantInt::get(Int32Ty, 1), v,
                             ConstantInt::get(Int128Ty, totalSize)};
            builder.CreateCall(logFunc, args);
          }
        }
        if (StoreInst *storeInst = dyn_cast<StoreInst>(&I)) {
          if (GetElementPtrInst *getElementPtrInst =
                  dyn_cast<GetElementPtrInst>(storeInst->getPointerOperand())) {
            // errs() << I << "\n";

            // errs() <<
            // storeInst->getPointerOperand()->getType()->getElementType()->isArrayTy()
            // << "\tstoreInst: " << I << "\n";
            IRBuilder<> builder(storeInst);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());
            /*
            uint64_t arraySize =
                allocInst->getAllocatedType()->getArrayNumElements();
            uint64_t elementSize = DL.getTypeStoreSizeInBits(
                allocInst->getAllocatedType()->getArrayElementType());
            unsigned __int128 totalSize = arraySize * elementSize;
            */
            // errs() << "storeInst\n";
            Value *v = storeInst->getPointerOperand();
            // errs() << DL.getTypeStoreSizeInBits(getElementType(v->getType()))
            // << "\n";
            Value *args[] = {
                getLineNum(&I), ConstantInt::get(Int32Ty, 2), v,
                ConstantInt::get(Int128Ty, DL.getTypeStoreSizeInBits(
                                               getElementType(v->getType())))};
            builder.CreateCall(logFunc, args);
          }
        }
        if (LoadInst *loadInst = dyn_cast<LoadInst>(&I)) {
          // errs() << "loadInst: " << I << "\n";
        }
        /*
        if (ReturnInst *returnInst = dyn_cast<ReturnInst>(&I)) {
          errs() << "returnInst: " << I << "\n";
        }
        */
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
