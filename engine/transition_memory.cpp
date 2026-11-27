#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <vector>
#include <algorithm>

namespace {
constexpr int N=7, S=0, P=6, INF=1000000;
using Graph=std::array<std::array<int,N>,N>;
using Path=std::vector<int>;
struct Cell { int count=0; int sum=0; };
struct Memory {
    std::array<std::array<Cell,N>,N> cells{};
    void observe(const Graph& g) {
        for(int u=0;u<N;++u) for(int v=u+1;v<N;++v)
            if(g[u][v]>0) { ++cells[u][v].count; cells[u][v].sum+=g[u][v]; }
    }
    Graph estimate() const {
        Graph g{};
        for(int u=0;u<N;++u) for(int v=u+1;v<N;++v) {
            const auto& c=cells[u][v];
            if(c.count) g[u][v]=(100*c.sum+c.count/2)/c.count;
        }
        return g;
    }
};
Graph sample(std::mt19937& rng, bool shift=false) {
    Graph g{};
    auto edge=[&](int u,int v,int base){g[u][v]=base+static_cast<int>(rng()%3);};
    edge(0,1,1); edge(0,2,3); edge(1,3,3); edge(1,4,2);
    edge(2,4,1); edge(2,5,1); edge(3,6,5); edge(4,6,5);
    edge(5,6,shift?12:1);
    return g;
}
Path shortest(const Graph& g) {
    std::array<int,N> dist{}, next{};
    dist.fill(INF); next.fill(-1); dist[P]=0;
    for(int u=P-1;u>=S;--u) for(int v=u+1;v<N;++v)
        if(g[u][v]>0 && dist[v]!=INF && g[u][v]+dist[v]<dist[u]) {
            dist[u]=g[u][v]+dist[v]; next[u]=v;
        }
    if(dist[S]==INF) return {};
    Path path{S};
    while(path.back()!=P) path.push_back(next[path.back()]);
    return path;
}
Path greedy(const Graph& g) {
    Path path{S};
    while(path.back()!=P) {
        int u=path.back(), next=-1, best=INF;
        for(int v=u+1;v<N;++v) if(g[u][v]>0 && g[u][v]<best) {best=g[u][v]; next=v;}
        if(next<0) return {};
        path.push_back(next);
    }
    return path;
}
int cost(const Graph& g,const Path& p) {
    if(p.empty() || p.front()!=S || p.back()!=P || p.size()>N) return INF;
    int total=0;
    for(std::size_t i=1;i<p.size();++i) {
        int u=p[i-1],v=p[i];
        if(u<0||v>=N||v<=u||g[u][v]<=0) return INF;
        total+=g[u][v];
    }
    return total;
}
int exhaustive(const Graph& g,int u=S) {
    if(u==P) return 0;
    int best=INF;
    for(int v=u+1;v<N;++v) if(g[u][v]>0) best=std::min(best,g[u][v]+exhaustive(g,v));
    return best;
}
void check(bool ok,const char* message) {if(!ok) throw std::runtime_error(message);}
struct Stats {int greedy=0,memory=0,oracle=0, wins=0, ties=0, losses=0;};
Stats evaluate(const Graph& estimated,std::uint32_t seed,bool shift) {
    std::mt19937 rng(seed); Stats s;
    auto a=greedy(estimated),b=shortest(estimated);
    for(int i=0;i<100;++i) {
        auto g=sample(rng,shift);
        int x=cost(g,a),y=cost(g,b),z=cost(g,shortest(g));
        check(x<INF&&y<INF&&z==exhaustive(g),"validity/oracle mismatch");
        check(z<=x&&z<=y,"oracle lower bound");
        s.greedy+=x; s.memory+=y; s.oracle+=z;
        if(y<x) ++s.wins; else if(y==x) ++s.ties; else ++s.losses;
    }
    return s;
}
void report(const char* label,const Stats& s) {
    std::cout<<label<<" greedy="<<s.greedy<<" memory="<<s.memory<<" oracle="<<s.oracle
             <<" wins="<<s.wins<<" ties="<<s.ties<<" losses="<<s.losses<<'\n';
}
}
int main() {
    try {
        Memory m; std::mt19937 training(2601);
        for(int i=0;i<100;++i) m.observe(sample(training));
        auto g=m.estimate();
        check(m.cells[0][1].count==100 && m.cells[0][6].count==0,"matrix counts");
        check(shortest(Graph{}).empty(),"unreachable graph");
        check(cost(g,{0,6})==INF && cost(g,{0,1,0,6})==INF,"invalid paths rejected");
        auto normal=evaluate(g,2602,false),shift=evaluate(g,2603,true);
        check(normal.wins+normal.ties+normal.losses==100,"normal count");
        check(shift.wins+shift.ties+shift.losses==100,"shift count");
        report("held-out",normal); report("distribution-shift",shift);
        Memory refreshed; std::mt19937 calibration(2604);
        for(int i=0;i<20;++i) refreshed.observe(sample(calibration,true));
        auto adapted=evaluate(refreshed.estimate(),2603,true);
        report("shift-after-calibration",adapted);
        check(adapted.memory<shift.memory,"calibration improves shifted case");
        std::cout<<"PASS matrix, paths, and 200 exhaustive oracle comparisons\n";
    } catch(const std::exception& e) {std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}
}
