add_toy_library(ToyConversion
  ConvertToyToToyCPU.cpp

  DEPENDS
  ToyConversionPassIncGen

  LINK_LIBS PUBLIC
  MLIRIR
)