#include "popova_e_radix_sort_for_double_with_simple_merge/tbb/include/ops_tbb.hpp"

#include <tbb/tbb.h>

#include <atomic>
#include <numeric>
#include <util/include/util.hpp>
#include <vector>

#include "popova_e_radix_sort_for_double_with_simple_merge/common/include/common.hpp"
#include "oneapi/tbb/parallel_for.h"

namespace popova_e_radix_sort_for_double_with_simple_merge_threads {

PopovaERadixSorForDoubleWithSimpleMergeTBB::PopovaERadixSorForDoubleWithSimpleMergeTBB(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool PopovaERadixSorForDoubleWithSimpleMergeTBB::ValidationImpl() {
  return (GetInput() > 0) && (GetOutput() == 0);
}

bool PopovaERadixSorForDoubleWithSimpleMergeTBB::PreProcessingImpl() {
  GetOutput() = 2 * GetInput();
  return GetOutput() > 0;
}

bool PopovaERadixSorForDoubleWithSimpleMergeTBB::RunImpl() {
  for (InType i = 0; i < GetInput(); i++) {
    for (InType j = 0; j < GetInput(); j++) {
      for (InType k = 0; k < GetInput(); k++) {
        std::vector<InType> tmp(i + j + k, 1);
        GetOutput() += std::accumulate(tmp.begin(), tmp.end(), 0);
        GetOutput() -= i + j + k;
      }
    }
  }

  const int num_threads = ppc::util::GetNumThreads();
  GetOutput() *= num_threads;

  std::atomic<int> counter(0);
  tbb::parallel_for(0, ppc::util::GetNumThreads(), [&](int /*i*/) { counter++; });

  GetOutput() /= counter;
  return GetOutput() > 0;
}

bool PopovaERadixSorForDoubleWithSimpleMergeTBB::PostProcessingImpl() {
  GetOutput() -= GetInput();
  return GetOutput() > 0;
}

}  // namespace popova_e_radix_sort_for_double_with_simple_merge_threads
