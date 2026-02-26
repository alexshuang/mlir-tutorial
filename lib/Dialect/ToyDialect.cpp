#include "Dialect/ToyDialect.h"
#include "Dialect/ToyDialect.cpp.inc"
#include "llvm/ADT/TypeSwitch.h"
#include "mlir/IR/Builders.h"

#define GET_TYPEDEF_CLASSES
#include "Dialect/ToyTypes.cpp.inc"

#define GET_OP_CLASSES
#include "Dialect/ToyOps.cpp.inc"

namespace mlir {
namespace toy {

void ToyDialect::initialize() {
    // Used to register types and operations with the dialect
    addTypes<
#define GET_TYPEDEF_LIST
#include "Dialect/ToyTypes.cpp.inc"
    >();

    addOperations<
#define GET_OP_LIST
#include "Dialect/ToyOps.cpp.inc"
    >();
}

}
}
