#include "LocalOpts.h"
#include <iostream>
#include <vector>

using namespace llvm;

static int getShift(int64_t x){
    // 判断是否为2的次方
    if(x <= 0 || (x & (~x + 1)) != x){
        return -1;
    }
    int i = 0;
    while(x > 1){
        x >>= 1;
        ++i;
    }
    return i;
}

PreservedAnalyses StrengthReductionPass::run([[maybe_unused]] Function &F,
                                             FunctionAnalysisManager &) {

    /// @todo(CSCD70) Please complete this method.
	std::vector<Instruction*> DelInsList;
    int StrengthOptNum = 0;
	//遍历整个function
	for(auto &BB: F){
		for(auto &In: BB){
			if(In.getNumOperands() == 2){   
                int shift;
                Value* op;
				auto op1 = In.getOperand(0);
				auto op2 = In.getOperand(1);
                int shift1 = -1, shift2 = -1;
				if(isa<ConstantInt>(op1)){
					shift1 = getShift(dyn_cast<ConstantInt>(op1)->getSExtValue());
				}
				if(isa<ConstantInt>(op2)){
					shift2 = getShift(dyn_cast<ConstantInt>(op2)->getSExtValue());
				}
                
                switch(In.getOpcode()){
                    case Instruction::Mul:
                    case Instruction::SDiv:
                        if ((isa<ConstantInt>(op1) && shift1 != -1) || (isa<ConstantInt>(op2) && shift2 != -1)){
                            op = (shift1 == -1)? op1 : op2;
                            shift = (shift1 == -1) ? shift2 : shift1;
                            IRBuilder<> builder(&In);
                            Value* NewIns;
                            switch (In.getOpcode()){
                                case Instruction::Mul:
                                // (2 ^ n) * x => x << n 
                                // x * (2 ^ n) => x << n
                                    NewIns = builder.CreateShl(op,shift);
                                    break;
                                case Instruction::SDiv:
                                // x / (2 ^ n) => x >> n
                                    NewIns = builder.CreateAShr(op,shift);
                                    break;            
                            }
                            
                            In.replaceAllUsesWith(NewIns);
                            DelInsList.push_back(&In);
                            ++StrengthOptNum;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

	for(auto &ins: DelInsList){
		if(ins->isSafeToRemove()){
			ins->eraseFromParent();
    	}
	}
    outs() << "define dso_local void @StrengthReduction(i32 noundef %0) {" << "\n";
    // 输出Strength Reduction次数
    outs() << "Strength Reduction: " << StrengthOptNum << "\n";

    return PreservedAnalyses::none();
}