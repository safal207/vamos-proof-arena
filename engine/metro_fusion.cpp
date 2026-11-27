#include <openssl/sha.h>
#include <array>
#include <cmath>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
using G=std::array<std::array<int,7>,7>;
using M=std::array<std::array<double,7>,7>;
using Path=std::array<int,4>;
constexpr std::array<Path,4> paths{{{0,1,3,6},{0,1,4,6},{0,2,4,6},{0,2,5,6}}};
void check(bool b,const char* msg){if(!b) throw std::runtime_error(msg);}
G sample(std::mt19937& rng,int phase){G g{};auto edge=[&](int u,int v,int c){g[u][v]=c+int(rng()%3);};edge(0,1,1);edge(0,2,3);edge(1,3,3);edge(1,4,2);edge(2,4,1);edge(2,5,1);edge(3,6,5);edge(4,6,phase==2?2:5);edge(5,6,phase==1?12:1);return g;}
int cost(const G& g,const Path& p){if(p[0]!=0||p[3]!=6)return -1;int c=0;for(int i=1;i<4;++i){int u=p[i-1],v=p[i];if(u<0||v>=7||v<=u||g[u][v]<=0||g[u][v]>100)return -1;c+=g[u][v];}return c;}
std::string sha(const std::string& s){unsigned char d[SHA256_DIGEST_LENGTH];SHA256(reinterpret_cast<const unsigned char*>(s.data()),s.size(),d);std::string out;const char* h="0123456789abcdef";for(auto x:d){out+=h[x>>4];out+=h[x&15];}return out;}
std::string graph_bytes(const G& g){std::string s;for(auto& row:g)for(int v:row)s+=std::to_string(v)+",";return s;}
struct Proof{int id,budget;Path path;int spent;std::string root;};
std::string digest(const G& g,const Proof& p){std::string s="vamos.route.v1|"+std::to_string(p.id)+"|"+std::to_string(p.budget)+"|"+graph_bytes(g)+"|";for(int v:p.path)s+=std::to_string(v)+",";return sha(s+"|"+std::to_string(p.spent));}
Proof issue(const G& g,int id,int budget,Path path){int c=cost(g,path);check(id>0&&budget>0&&c>0&&c<=budget,"issue denied");Proof p{id,budget,path,c,{}};p.root=digest(g,p);return p;}
// Trusted challenge, attempt identity and budget supplied independently of the receipt.
bool replay(const G& g,int id,int budget,const Proof& p){if(id<=0||budget<=0||p.id!=id||p.budget!=budget||p.root!=digest(g,p))return false;int c=cost(g,p.path);return c>0&&c<=budget&&c==p.spent;}
struct Memory{M weights{};std::array<double,4> trail{};void observe(const G& g,double alpha){for(int u=0;u<7;++u)for(int v=u+1;v<7;++v)if(g[u][v]>0){double& w=weights[u][v];w=w==0?g[u][v]:(1-alpha)*w+alpha*g[u][v];}for(double& t:trail)t*=0.85;int best=0;for(int i=1;i<4;++i)if(cost(g,paths[i])<cost(g,paths[best]))best=i;trail[best]+=1.0/cost(g,paths[best]);}int choose(bool use_trail)const{int best=0;double low=1e9;for(int j=0;j<4;++j){double score=0;for(int i=1;i<4;++i)score+=weights[paths[j][i-1]][paths[j][i]];if(use_trail)score-=2*trail[j];if(score<low){low=score;best=j;}}return best;}};
int main(){try{check(sha("abc")=="ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad","SHA vector");std::mt19937 train(2701),test(2702);Memory frozen;for(int i=0;i<100;++i)frozen.observe(sample(train,0),1.0/(i+1));Memory live=frozen;std::array<long,4> total{};std::set<int> accepted;int tamper=0;for(int i=0;i<1000;++i){int phase=i/250%3;G g=sample(test,phase);std::array<int,4> pick{frozen.choose(false),live.choose(false),live.choose(true),0};for(int j=1;j<4;++j)if(cost(g,paths[j])<cost(g,paths[pick[3]]))pick[3]=j;for(int k=0;k<4;++k){int id=4*i+k+1;auto p=issue(g,id,24,paths[pick[k]]);check(replay(g,id,24,p),"valid replay");check(accepted.insert(id).second,"unique attempt");check(p.spent>=cost(g,paths[pick[3]]),"oracle lower bound");total[k]+=p.spent;if(k==2){auto q=p;q.spent++;check(!replay(g,id,24,q),"mutated cost");q.root=digest(g,q);check(!replay(g,id,24,q),"rehashed false cost");q=p;q.path[1]=6;q.root=digest(g,q);check(!replay(g,id,24,q),"invalid route");q=p;q.id++;q.root=digest(g,q);check(!replay(g,id,24,q),"wrong identity");q=p;q.budget=100;q.root=digest(g,q);check(!replay(g,id,24,q),"wrong budget");G changed=g;changed[0][1]++;check(!replay(changed,id,24,p),"changed challenge");check(!replay(g,id,1,p),"budget rejection");check(!accepted.insert(id).second,"duplicate in local registry");tamper+=8;}}live.observe(g,0.2);}std::cout<<"1000 scenarios; fixed="<<total[0]<<" ewma="<<total[1]<<" trail="<<total[2]<<" oracle="<<total[3]<<"\n"<<accepted.size()<<" replayed receipts; "<<tamper<<" negative checks\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
