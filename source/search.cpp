// Project includes
#include "clang-expand/search.hpp"
#include "clang-expand/common/data.hpp"
#include "clang-expand/common/query.hpp"
#include "clang-expand/common/routines.hpp"
#include "clang-expand/definition-search/tool-factory.hpp"
#include "clang-expand/symbol-search/tool-factory.hpp"

// Clang includes
#include <clang/Tooling/Tooling.h>

// Standard includes
#include <cstdlib>
#include <functional>
#include <string>
#include <variant>
#include <vector>

namespace ClangExpand {
Search::Search(const std::string& file, unsigned line, unsigned column)
: _location(Routines::makeAbsolute(file), line, column) {
}

Search::Result
Search::run(clang::tooling::CompilationDatabase& compilationDatabase,
            const SourceVector& sources) {
  Query query;

  _symbolSearch(compilationDatabase, query);

  if (query.isEmpty()) {
    Routines::error("Could not recognize token at specified location");
  }

  if (!query.isDefinition()) {
    _definitionSearch(compilationDatabase, sources, query);
  }

  if (!query.isDefinition()) {
    Routines::error("Could not find definition");
  }

  return query.definition();
}

void Search::_symbolSearch(CompilationDatabase& compilationDatabase,
                           Query& query) {
  clang::tooling::ClangTool SymbolSearch(compilationDatabase,
                                         {_location.filename});

  const auto error = SymbolSearch.run(
      new ClangExpand::SymbolSearch::ToolFactory(_location, &query));
  if (error) std::exit(error);
}

void Search::_definitionSearch(CompilationDatabase& compilationDatabase,
                               const SourceVector& sources,
                               Query& query) {
  clang::tooling::ClangTool DefinitionSearch(compilationDatabase, sources);

  const auto error = DefinitionSearch.run(
      new ClangExpand::DefinitionSearch::ToolFactory(_location.filename,
                                                     &query));
  if (error) std::exit(error);
}

}  // namespace ClangExpand
