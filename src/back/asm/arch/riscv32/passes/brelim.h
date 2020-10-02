#ifndef MIMIC_BACK_ASM_ARCH_RISCV32_PASSES_BRELIM_H_
#define MIMIC_BACK_ASM_ARCH_RISCV32_PASSES_BRELIM_H_

#include "back/asm/mir/pass.h"
#include "back/asm/arch/riscv32/instdef.h"

namespace mimic::back::asmgen::riscv32 {

/*
  this pass will:
  1.  eliminate redundant unconditional branch instructions
  2.  eliminate unused labels
*/
class BranchEliminationPass : public PassInterface {
 public:
  BranchEliminationPass() {}

  void RunOn(const OprPtr &func_label, InstPtrList &insts) override {
    using OpCode = RISCV32Inst::OpCode;
    // remove redundant branches
    InstPtr last;
    for (auto it = insts.begin(); it != insts.end(); ++it) {
      if (last) {
        auto last_inst = static_cast<RISCV32Inst *>(last.get());
        auto cur_inst = static_cast<RISCV32Inst *>(it->get());
        // check if last instruction can be removed
        if (last_inst->opcode() == OpCode::J &&
            cur_inst->opcode() == OpCode::LABEL &&
            last_inst->oprs()[0].value() == cur_inst->oprs()[0].value()) {
          it = insts.erase(--it);
        }
      }
      // remember last instruction
      last = *it;
    }
    // remove unused labels
    for (auto it = insts.begin(); it != insts.end();) {
      auto cur_inst = static_cast<RISCV32Inst *>(it->get());
      // check if label can be removed
      if (cur_inst->opcode() == OpCode::LABEL &&
          cur_inst->oprs()[0].value()->use_count() == 1) {
        it = insts.erase(it);
      }
      else {
        ++it;
      }
    }
  }
};

}  // namespace mimic::back::asmgen::riscv32

#endif  // MIMIC_BACK_ASM_ARCH_RISCV32_PASSES_BRELIM_H_
