set(LLVM_TARGET_DEFINITIONS ToyCanonicalize.td)
mlir_tablegen(ToyCanonicalize.cpp.inc -gen-rewriters)
add_public_tablegen_target(ToyCanonicalizeIncGen)

add_toy_library(ToyIR
  Dialect.cpp

  DEPENDS
  ToyOpsIncGen
  ToyCanonicalizeIncGen

  LINK_LIBS PUBLIC
  MLIRIR
)