#include "LocalOpts.h"
#include <iostream>
#include <vector>

using namespace llvm;

PreservedAnalyses MultiInstOptPass::run([[maybe_unused]] Function &F,
                                        FunctionAnalysisManager &) {

    /// @todo(CSCD70) Please complete this method.
	std::vector<Instruction*> DelInsList;
	int MultiInstOptNum = 0;
	//遍历整个function
	for(auto &BB: F){
		for(auto &In: BB){
			if(In.getNumOperands() == 2){
				auto op1 = In.getOperand(0);
				auto op2 = In.getOperand(1);
				switch (In.getOpcode()){
                    case Instruction::Add:
                        for(User* user:In.users()){
                            if(Instruction* UserIn = dyn_cast<Instruction>(user)){
                                Value* NewVal = NULL;
                                if(UserIn->getOpcode() == Instruction::Sub){
                                    if(UserIn->getOperand(1) == op1){
                                        NewVal = op2;
                                    }
                                    else if(UserIn->getOperand(1) == op2){
                                        NewVal = op1;
                                    }
                                }
                                if(NewVal){
                                    UserIn->replaceAllUsesWith(NewVal);  
                                    DelInsList.push_back(UserIn);
                                    ++MultiInstOptNum;
                                }
                            }
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

    outs() << "define dso_local void @MultiInstOpt(i32 noundef %0, i32 noundef %1) {" << "\n";
	// 输出Multi-Instruction Optimization次数
	outs() << "Multi-Instruction Optimization: " << MultiInstOptNum << "\n";

	return PreservedAnalyses::none();
}
