#ifndef TOY_CONVERSION_PASSES_H_
#define TOY_CONVERSION_PASSES_H_

#include "mlir/Pass/Pass.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/Dialect/Affine/IR/AffineOps.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "Dialect/Toy/IR/Dialect.h"

namespace mlir {
namespace toy {

#define GEN_PASS_DECL
#include "Conversion/ToyToToyCPU/Passes.h.inc"

#define GEN_PASS_REGISTRATION
#include "Conversion/ToyToToyCPU/Passes.h.inc"

}
}

#endif
