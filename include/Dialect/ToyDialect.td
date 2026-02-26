#ifndef TOY_DIALECT
#define TOY_DIALECT

include "mlir/IR/DialectBase.td"

def Toy_Dialect : Dialect {
    let name = "toy";
    let description = [{
        The toy dialect define types and operations.
    }];
    let cppNamespace = "::mlir::toy";
    let useDefaultTypePrinterParser = 1;
}

#endif
