// Reuse the unchanged VALTA-029 fixture implementation in this local export tool.
#define main guardian_regression_main
#include "financial_guardian.cpp"
#undef main

const char* label(Status s) {
    switch(s) {
        case Status::Allowed: return "ALLOWED";
        case Status::CapDenied: return "CAP_DENIED";
        case Status::Frozen: return "FROZEN";
        case Status::RateDenied: return "RATE_DENIED";
        case Status::Duplicate: return "DUPLICATE";
        case Status::Invalid: return "INVALID";
        case Status::Settled: return "SETTLED";
        case Status::Released: return "RELEASED";
        case Status::Uncertain: return "UNCERTAIN";
        case Status::FrozenNow: return "FROZEN_NOW";
        case Status::Thawed: return "THAWED";
    }
    throw std::logic_error("status");
}
int main() {
    using K=Kind;
    const std::array<Trace,6> scenarios{{
        {{K::Request,1,1000}},
        {{K::Request,1,100},{K::Request,2,100},{K::Request,3,100},{K::Request,4,100}},
        {{K::Request,1,100},{K::Freeze},{K::Request,2,100}},
        {{K::Request,1,200},{K::Unknown,1}},
        {{K::Request,1,100,true}},
        {{K::Request,1,100},{K::Success,1,80}}
    }};
    constexpr std::array<Money,3> caps{300,500,1000};
    std::cout << '[';
    for(std::size_t i=0;i<scenarios.size();++i) {
        auto proof=run(caps,scenarios[i]);
        check(verify(caps,scenarios[i],proof),"fixture replay");
        if(i) std::cout << ',';
        std::cout << '[';
        for(std::size_t j=0;j<proof.size();++j) {
            const auto& r=proof[j]; if(j) std::cout << ',';
            std::cout << "{\"status\":\"" << label(r.status) << "\",\"dispatched\":" << (r.dispatched?"true":"false")
                << ",\"settled\":" << r.settled << ",\"reserved\":" << r.reserved << ",\"id\":" << r.event.id << ",\"amount\":" << r.event.amount << '}';
        }
        std::cout << ']';
    }
    std::cout << "]\n";
}
