#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <random>
#include <stdexcept>
#include <vector>

namespace {
using Money = long long; // integer simulated cents, never actual billing
constexpr Money MaxAmount = 1000000;
enum class Kind { Request, Success, Upstream4xx, Unknown, Freeze, Thaw };
enum class Status { Allowed, CapDenied, Frozen, RateDenied, Duplicate, Invalid, Settled, Released, Uncertain, FrozenNow, Thawed };
struct Event { Kind kind; int id=0; Money amount=0; bool rate_limited=false; bool operator==(const Event&) const = default; };
struct Receipt {
    Status status; bool dispatched; Money settled, reserved; Event event;
    bool operator==(const Receipt&) const = default;
};
struct Pending { Money estimate; bool uncertain=false; };
struct Ledger {
    // One agent, one run, fixed day and month; no time rollover.
    std::array<Money,3> caps;
    Money settled=0, reserved=0;
    bool frozen=false;
    std::map<int,Pending> pending;
    std::map<int,bool> seen;
    explicit Ledger(std::array<Money,3> limits):caps(limits) {
        for(auto c:caps) if(c<0||c>MaxAmount) throw std::invalid_argument("cap");
    }
    Receipt apply(const Event& e) {
        auto out=[&](Status s,bool dispatched=false){return Receipt{s,dispatched,settled,reserved,e};};
        if(e.kind==Kind::Freeze){frozen=true;return out(Status::FrozenNow);}
        if(e.kind==Kind::Thaw){frozen=false;return out(Status::Thawed);}
        if(e.id<=0||e.amount<0||e.amount>MaxAmount)return out(Status::Invalid);
        if(e.kind==Kind::Request){
            if(e.amount==0)return out(Status::Invalid);
            if(seen.contains(e.id))return out(Status::Duplicate);
            seen.emplace(e.id,true);
            if(frozen)return out(Status::Frozen);
            if(e.rate_limited)return out(Status::RateDenied);
            for(auto c:caps)if(e.amount>c-settled-reserved)return out(Status::CapDenied);
            pending.emplace(e.id,Pending{e.amount});reserved+=e.amount;
            return out(Status::Allowed,true);
        }
        auto it=pending.find(e.id);
        if(it==pending.end())return out(Status::Invalid);
        if(e.kind==Kind::Unknown||(e.kind==Kind::Success&&e.amount>it->second.estimate)){
            it->second.uncertain=true;
            if(e.kind==Kind::Success) frozen=true;
            return out(Status::Uncertain);
        }
        // Uncertain requests require an explicit trusted resolution; never retry automatically.
        if(e.kind!=Kind::Success&&e.kind!=Kind::Upstream4xx)return out(Status::Invalid);
        if(e.kind==Kind::Upstream4xx&&e.amount!=0)return out(Status::Invalid);
        reserved-=it->second.estimate;
        if(e.kind==Kind::Success)settled+=e.amount;
        pending.erase(it);
        return out(e.kind==Kind::Success?Status::Settled:Status::Released);
    }
    bool invariant() const {
        Money sum=0;for(const auto& [id,p]:pending){if(!seen.contains(id)||p.estimate<=0)return false;sum+=p.estimate;}
        return settled>=0&&reserved>=0&&sum==reserved&&settled+reserved<=*std::min_element(caps.begin(),caps.end());
    }
};
void check(bool ok,const char* msg){if(!ok)throw std::runtime_error(msg);}
using Trace=std::vector<Event>;using Proof=std::vector<Receipt>;
Proof run(std::array<Money,3> caps,const Trace& t){Ledger l(caps);Proof p;for(const auto& e:t){p.push_back(l.apply(e));check(l.invariant(),"accounting invariant");}return p;}
bool verify(std::array<Money,3> caps,const Trace& trusted,const Proof& supplied){return run(caps,trusted)==supplied;}
void fixtures(){
    using K=Kind;using S=Status;
    auto deny=run({300,500,1000},{{K::Request,1,1000}});
    check(deny[0]==Receipt{S::CapDenied,false,0,0,{K::Request,1,1000}},"first deny");
    auto loop=run({300,500,1000},{{K::Request,1,100},{K::Request,2,100},{K::Request,3,100},{K::Request,4,100}});
    check(loop[2].reserved==300&&loop[3].status==S::CapDenied,"loop cap");
    auto f=run({300,500,1000},{{K::Request,1,100},{K::Freeze},{K::Request,2,100},{K::Success,1,80},{K::Thaw},{K::Request,3,100}});
    check(f[2].status==S::Frozen&&f[3].settled==80&&f[5].dispatched,"freeze preserves in-flight settlement");
    auto e=run({300,500,1000},{{K::Request,1,100,true},{K::Request,2,100},{K::Upstream4xx,2},{K::Request,3,200},{K::Unknown,3},{K::Request,4,101},{K::Success,3,250},{K::Success,3,180},{K::Success,3,180},{K::Request,3,1}});
    check(e[0].status==S::RateDenied&&!e[0].dispatched&&e[1].dispatched&&e[2].reserved==0,"rate vs upstream");
    check(e[4].status==S::Uncertain&&e[4].reserved==200&&e[5].status==S::CapDenied,"unknown retains hold");
    check(e[6].status==S::Uncertain&&e[7].settled==180&&e[8].status==S::Invalid&&e[9].status==S::Duplicate,"resolution and duplicate");
    check(run({500,100,1000},{{K::Request,1,101}})[0].status==S::CapDenied,"day cap");
    check(run({500,1000,100},{{K::Request,1,101}})[0].status==S::CapDenied,"month cap");
    check(run({300,500,1000},{{K::Request,1,-1},{K::Request,2,0},{K::Request,3,MaxAmount+1}})[2].status==S::Invalid,"invalid amount");
}
}
int main(){try{
    fixtures();std::mt19937 rng(2901);std::size_t events=0,negative=0;constexpr std::array<Money,3> caps{300,500,1000};
    for(int i=0;i<1000;++i){Trace t;for(int j=0;j<60;++j){auto k=static_cast<Kind>(rng()%6);t.push_back({k,1+int(rng()%20),k==Kind::Upstream4xx?0:Money(rng()%401),rng()%7==0});}
        auto p=run(caps,t);check(verify(caps,t,p),"replay");events+=t.size();
        auto q=p;q[0].reserved++;check(!verify(caps,t,q),"modified reserved");
        q=p;q[0].dispatched=!q[0].dispatched;check(!verify(caps,t,q),"modified dispatch");
        q=p;q[0].settled++;check(!verify(caps,t,q),"modified settled");
        q=p;q.pop_back();check(!verify(caps,t,q),"truncated proof");
        q=p;q[0].event.id++;check(!verify(caps,t,q),"modified request identity");negative+=5;
    }
    std::cout<<"PASS 9 fixture checks; 1000 traces; "<<events<<" events; "<<negative<<" negative proof checks\n";
    return 0;
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
