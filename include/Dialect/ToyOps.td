#ifndef TOY_OPS
#define TOY_OPS

include "ToyDialect.td"
include "ToyTypes.td"
include "mlir/IR/OpBase.td"

class Toy_Op<string mnemonic> : Op<Toy_Dialect, mnemonic> {}

def Toy_AddPtrOp : Toy_Op<"addptr"> {
    let summary = "Add pointer addresss with an offset";
    let description = [{
        Add pointer addresss with an offset.
    }];

    let arguments = (ins Toy_Ptr:$ptr, Toy_Int:$offset);
    let results = (outs Toy_Ptr:$res);

    let assemblyFormat = "$ptr `,` $offset attr-dict `:` type($ptr) `,` type($offset) `->` type($res)";
}

#endif
