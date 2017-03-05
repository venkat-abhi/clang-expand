// Project includes
#include "clang-expand/common/parameter-rewriter.hpp"

// Clang includes
#include "clang/AST/Decl.h"
#include "clang/Rewrite/Core/Rewriter.h"

namespace ClangExpand {

UsageFinder::UsageFinder(const ParameterMap& parameterMap,
                         clang::Rewriter& rewriter)
: _parameterMap(parameterMap), _rewriter(rewriter) {
}

bool UsageFinder::VisitStmt(clang::Stmt* statement) {
  const auto* use = llvm::dyn_cast<clang::DeclRefExpr>(statement);
  if (!use) return true;

  const auto* decl = llvm::dyn_cast<clang::ParmVarDecl>(use->getDecl());
  if (!decl) return true;

  const auto name = decl->getName();

  auto iterator = _parameterMap.find(name);
  if (iterator != _parameterMap.end()) {
    const auto& argument = iterator->getValue();
    bool error = _rewriter.ReplaceText(use->getSourceRange(), argument);
    assert(!error && "Error replacing text in definition");
  }

  return true;
}

}  // namespace ClangExpand