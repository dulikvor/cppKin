#include <string>

#include "benchmark/benchmark.h"
#include "src/cppkin.h"
#include "src/TransportType.h"

using namespace std;

static void BM_SpanCreation(benchmark::State& state){
    for(auto _ : state){
        cppkin::CreateTrace("BenchMark_Span");
    }
}

static void BM_EventCreation(benchmark::State& state){
    cppkin::CreateTrace("BenchMark_Span");
    for(auto _ : state){
        cppkin::CreateTrace("BenchMark_Event");
    }
}

static void BM_EventCreationSubmit(benchmark::State& state){
    for(auto _ : state){
        cppkin::CreateTrace("BenchMark_Span");
        cppkin::SubmitSpan();
    }
}

int main(int argc, char** argv)
{
    cppkin::CppkinParams cppkinParams;
    cppkinParams.AddParam(cppkin::ConfigTags::TRANSPORT_TYPE, (cppkin::TransportType(cppkin::TransportType::Stub)).ToString());
    cppkinParams.AddParam(cppkin::ConfigTags::HOST_ADDRESS, string("127.0.0.1"));
    cppkinParams.AddParam(cppkin::ConfigTags::PORT, 9410);
    cppkinParams.AddParam(cppkin::ConfigTags::SERVICE_NAME, string("Cluster_Manager"));
    cppkinParams.AddParam(cppkin::ConfigTags::DEBUG, false);
    cppkinParams.AddParam(cppkin::ConfigTags::SAMPLE_COUNT, 1);
	cppkinParams.AddParam(cppkin::ConfigTags::ENCODING_TYPE, cppkin::EncodingType(cppkin::EncodingType::Thrift).ToString());
    cppkin::Init(cppkinParams);

    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    ::benchmark::RunSpecifiedBenchmarks();
  }

BENCHMARK(BM_SpanCreation);
BENCHMARK(BM_EventCreation);
BENCHMARK(BM_EventCreationSubmit);
BENCHMARK(BM_EventCreationSubmit)->Threads(10);
