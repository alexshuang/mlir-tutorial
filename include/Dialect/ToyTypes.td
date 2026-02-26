#ifndef TOY_TYPES
#define TOY_TYPES

include "ToyDialect.td"
include "mlir/IR/AttrTypeBase.td"

class Toy_Type<string name, string typeMnemonic> : TypeDef<Toy_Dialect, name> {
    let mnemonic = typeMnemonic;
}

def Ptr : Toy_Type<"Ptr", "ptr"> {
    let summary = "Toy buffer pointer type";
    let description = [{
        Toy buffer pointer type
    }];

    let parameters = (ins "Type":$elemType);
    let assemblyFormat = "`<` $elemType `>`";
}

#endif
