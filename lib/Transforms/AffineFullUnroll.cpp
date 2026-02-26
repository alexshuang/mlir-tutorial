#include "mlir/Dialect/Affine/IR/AffineOps.h"
#include "mlir/Dialect/Affine/LoopUtils.h"
#include "Transforms/AffineFullUnroll.h"
#include "mlir/Pass/Pass.h"

namespace mlir {
namespace toy {

using mlir::affine::AffineForOp;
using mlir::affine::loopUnrollFull;

#define GEN_PASS_DEF_AFFINEFULLUNROLL
#include "Transforms/Passes.h.inc"

struct AffineFullUnroll : public impl::AffineFullUnrollBase<AffineFullUnroll> {
    using AffineFullUnrollBase::AffineFullUnrollBase;

    void runOnOperation() {
        auto mod = getOperation();
        mod->walk([&](AffineForOp op){
            if (failed(loopUnrollFull(op))) {
                op.emitError("unrolling failed");
                signalPassFailure();
            }
        });
    }
};

}
}