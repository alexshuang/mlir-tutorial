#ifndef TOY_OPS
#define TOY_OPS

include "ToyDialect.td"
include "ToyTypes.td"
include "mlir/IR/OpBase.td"
include "mlir/Interfaces/SideEffectInterfaces.td"

class Toy_Op<string mnemonic, list<Trait> traits = []> : Op<Toy_Dialect, mnemonic, traits> {}

def Toy_AddPtrOp : Toy_Op<"addptr", [Pure, ElementwiseMappable]> {
    let summary = "Add pointer addresss with an offset";
    let description = [{
        Add pointer addresss with an offset.
    }];

    let arguments = (ins Toy_PtrLike:$ptr, Toy_IntLike:$offset);
    let results = (outs Toy_PtrLike:$res);

    let assemblyFormat = "$ptr `,` $offset attr-dict `:` type($ptr) `,` type($offset) `->` type($res)";
}

#endif
