//===--- PreambleBenchmark.cpp - Clangd preamble benchmarks -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../Preamble.h"
#include "benchmark/benchmark.h"

#include <fstream>
#include <iostream>

const char *FileName;

namespace clang {
namespace clangd {
namespace {

void buildPreamble(const ParseInputs &Inputs, const CompilerInvocation &CI) {
  clang::clangd::buildPreamble(FileName, CI, Inputs, false,
                               [](ASTContext &Ctx,
                                  std::shared_ptr<clang::Preprocessor> PP,
                                  const CanonicalIncludes &CanonIncludes) {});
}

static void preambleBuildBenchmark(benchmark::State &State) {
  std::ifstream SourceFile(FileName);
  std::string SourceContent((std::istreambuf_iterator<char>(SourceFile)),
                            std::istreambuf_iterator<char>());
  RealThreadsafeFS TFS;
  IgnoreDiagnostics Diags;
  DirectoryBasedGlobalCompilationDatabase::Options CDBOpts(TFS);
  auto CDB = std::make_unique<DirectoryBasedGlobalCompilationDatabase>(CDBOpts);

  ParseInputs Inputs;
  auto &Argv = Inputs.CompileCommand.CommandLine;
  Argv = {"clang"};
  // Put the file name at the end -- this allows the extra arg (-xc++) to
  // override the language setting.
  Argv.push_back(FileName);
  Inputs.CompileCommand = CDB->getCompileCommand(FileName).getValue();
  Inputs.Contents = std::move(SourceContent);
  Inputs.TFS = &TFS;
  Inputs.Opts = ParseOptions();
  auto CI = *buildCompilerInvocation(Inputs, Diags);

  for (auto _ : State)
    buildPreamble(Inputs, CI);
}
BENCHMARK(preambleBuildBenchmark);

} // namespace
} // namespace clangd
} // namespace clang

int main(int argc, char *argv[]) {
  if (argc < 2) {
    llvm::errs() << "Usage: " << argv[0] << " file_to_parse.cpp";
    return -1;
  }
  FileName = argv[1];
  // Trim first two arguments of the benchmark invocation and pretend no
  // arguments were passed in the first place.
  argv[1] = argv[0];
  argc -= 1;
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
}
