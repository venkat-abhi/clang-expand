#ifndef CLANG_EXPAND_RESULT_HPP
#define CLANG_EXPAND_RESULT_HPP

// Project includes
#include "clang-expand/common/call-data.hpp"
#include "clang-expand/common/declaration-data.hpp"
#include "clang-expand/common/definition-data.hpp"
#include "clang-expand/common/location.hpp"

// LLVM includes
#include <llvm/Support/YAMLTraits.h>

// Standard includes
#include <iosfwd>

namespace ClangExpand {
class Query;

struct Result {
  explicit Result(Query&& query);
  Range callExtent;
  DeclarationData declaration;
  DefinitionData definition;
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& stream, const Result& result);
}  // namespace ClangExpand

namespace llvm::yaml {
template <>
struct MappingTraits<ClangExpand::Result> {
  static void mapping(llvm::yaml::IO& io, ClangExpand::Result& result) {
    io.mapRequired("call", result.callExtent);
    io.mapRequired("declaration", result.declaration);
    io.mapRequired("definition", result.definition);
  }
};
}  // namespace llvm::yaml

#endif  // CLANG_EXPAND_RESULT_HPP
