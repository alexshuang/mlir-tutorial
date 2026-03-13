#ifndef TUTORIAL_DIALECT_TOYDIALECT_H_
#define TUTORIAL_DIALECT_TOYDIALECT_H_

#include "mlir/IR/Dialect.h"
#include "mlir/IR/DialectImplementation.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "mlir/Bytecode/BytecodeOpInterface.h"

#include "Dialect/Toy/IR/ToyDialect.h.inc"

#define GET_TYPEDEF_CLASSES
#include "Dialect/Toy/IR/ToyTypes.h.inc"

#define GET_OP_CLASSES
#include "Dialect/Toy/IR/ToyOps.h.inc"

#endif
