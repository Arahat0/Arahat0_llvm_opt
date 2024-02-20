#include "LocalOpts.h"
#include <iostream>
#include <vector>

using namespace llvm;

void delIns(std::vector<Instruction*> InsList){
	for(auto &ins: InsList){
		if(ins->isSafeToRemove()){
			ins->eraseFromParent();
    	}
	}
}

PreservedAnalyses AlgebraicIdentityPass::run([[maybe_unused]] Function &F,
                                             FunctionAnalysisManager &) {

    /// @todo(CSCD70) Please complete this method.
	std::vector<Instruction*> DelInsList;
	int AlgebraicOptNum = 0;
	//遍历整个function
	for(auto &BB: F){
		for(auto &In: BB){
			if(In.getNumOperands() == 2){
				Value* AlgebraicIdentity;
				auto op1 = In.getOperand(0);
				auto op2 = In.getOperand(1);
				int64_t ConstVal1, ConstVal2;
				if (isa<ConstantInt>(op1)) {
					ConstVal1 = dyn_cast<ConstantInt>(op1)->getSExtValue();
				}
				if (isa<ConstantInt>(op2)) {
					ConstVal2 = dyn_cast<ConstantInt>(op2)->getSExtValue();
				}

				bool AlgebraicFlag = false;
				switch (In.getOpcode()){
					case Instruction::Add: // x + 0 ; 0 + x
						if((isa<ConstantInt>(op1) && ConstVal1 == 0) || (isa<ConstantInt>(op2) && ConstVal2 == 0)){
							AlgebraicIdentity = op1 ? op1 : op2;
							AlgebraicFlag = true;
						}
						break;
					case Instruction::Sub: // x - 0 ; x - x
						 if(isa<ConstantInt>(op2) && ConstVal2 == 0){
							AlgebraicIdentity = op1;
							AlgebraicFlag = true;
						}
						else if(op1 == op2){
							AlgebraicIdentity = ConstantInt::getSigned(In.getType(), 0);
							AlgebraicFlag = true;
						}
						break;
					case Instruction::Mul: 
					      // x * 0 ; 0 * x 
						if((isa<ConstantInt>(op1) && ConstVal1 == 0) || (isa<ConstantInt>(op2) && ConstVal2 == 0)){
							AlgebraicIdentity = op1 ? op1 : op2;
							AlgebraicFlag = true;
						} // x * 1 ; 1 * x
						else if((isa<ConstantInt>(op1) && ConstVal1 == 1) || (isa<ConstantInt>(op2) && ConstVal2 == 1)){
							AlgebraicIdentity = (ConstVal1 == 1) ? op2 : op1;
							AlgebraicFlag = true;
						}
						break;	
					case Instruction::SDiv: // x / 1 ; x / x
						if(isa<ConstantInt>(op2) && ConstVal2 == 1){
							AlgebraicIdentity = op1;
							AlgebraicFlag = true;
						}
						if(op1 == op2){
							AlgebraicIdentity = ConstantInt::getSigned(In.getType(), 1);
							AlgebraicFlag = true;
						}
						break;
					default:
						break;
				}
				if (AlgebraicFlag == true){
					In.replaceAllUsesWith(AlgebraicIdentity);
					DelInsList.push_back(&In);
					++AlgebraicOptNum;
				}
			}
		}
	}
	
	for(auto &ins: DelInsList){
		if(ins->isSafeToRemove()){
			ins->eraseFromParent();
    	}
	}
	// 用于标记模式匹配的位置
	outs() << "define dso_local void @AlgebraicIdentity(i32 noundef %0) {" << "\n";
	// 输出Algebraic Identity次数
	outs() << "Algebraic Identity: " << AlgebraicOptNum << "\n";

	return PreservedAnalyses::none();
}