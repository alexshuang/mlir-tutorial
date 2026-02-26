#include "Dialect/ToyDialect.h"
#include "Dialect/ToyDialect.cpp.inc"
#include "llvm/ADT/TypeSwitch.h"
#include "mlir/IR/DialectImplementation.h"
#include "mlir/IR/Builders.h"

#define GET_TYPEDEF_CLASSES
#include "Dialect/ToyTypes.cpp.inc"

namespace mlir {
namespace toy {

void ToyDialect::initialize() {
    // Used to register types and operations with the dialect
    addTypes<
#define GET_TYPEDEF_LIST
#include "Dialect/ToyTypes.cpp.inc"
    >();
}

}
}
