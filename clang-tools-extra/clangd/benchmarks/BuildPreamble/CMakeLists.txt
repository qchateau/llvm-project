include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

add_benchmark(PreambleBenchmark PreambleBenchmark.cpp)
target_link_libraries(PreambleBenchmark
  PRIVATE
  clangDaemon
  clangFrontend
  clangBasic
  clangdSupport
  LLVMSupport
)
