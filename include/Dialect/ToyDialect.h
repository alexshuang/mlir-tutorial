#ifndef TUTORIAL_DIALECT_TOYDIALECT_H_
#define TUTORIAL_DIALECT_TOYDIALECT_H_

#include "mlir/IR/Dialect.h"
#include "mlir/IR/DialectImplementation.h"

#include "Dialect/ToyDialect.h.inc"

#define GET_TYPEDEF_CLASSES
#include "Dialect/ToyTypes.h.inc"

#define GET_OP_CLASSES
#include "Dialect/ToyOps.h.inc"

#endif
