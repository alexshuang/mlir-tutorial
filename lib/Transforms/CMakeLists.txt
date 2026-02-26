add_toy_library(ToyTransforms
  AffineFullUnroll.cpp

  DEPENDS
  ToyTransformsIncGen

  LINK_LIBS PUBLIC
  MLIRIR
  MLIRLLVMDialect
  MLIRAffineDialect
)