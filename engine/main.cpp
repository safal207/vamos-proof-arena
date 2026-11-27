#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace vamos {
struct Action { int mission; std::string value; };
struct Proof { int mission, cost; bool hidden; };
struct Result {
    int credits = 15, mission = 0, cost = 0;
    char node = 'S';
    bool valid = true;
    std::vector<Proof> proofs;
};

const std::map<char, std::map<char, int>> graph{
    {'S',{{'A',2},{'B',3}}}, {'A',{{'C',3},{'D',2}}},
    {'B',{{'D',1},{'E',1}}}, {'C',{{'P',3}}},
    {'D',{{'P',2}}}, {'E',{{'P',1}}}
};

// Replay untrusted actions, never accept a client-supplied score or receipt.
Result verify(const std::vector<Action>& actions) {
    Result r;
    bool archive = false;
    if (actions.size() > 15) { r.valid = false; return r; }
    for (const auto& a : actions) {
        if (a.mission != r.mission || r.mission >= 3) {
            r.valid = false; break;
        }
        int cost = 0;
        bool solved = false;
        char next = r.node;
        if (r.mission == 0) {
            const auto it = graph.find(r.node);
            if (a.value.size() != 1 || it == graph.end()) {
                r.valid = false; break;
            }
            const auto edge = it->second.find(a.value[0]);
            if (edge == it->second.end()) { r.valid = false; break; }
            next = edge->first; cost = edge->second;
            solved = next == 'P';
        } else {
            const std::array<std::array<std::string,3>,2> choices{{
                {"retry","freeze_check_quota","raise_cap"},
                {"allowed_charge_10","denied_charge_0","denied_charge_3"}
            }};
            const auto& options = choices.at(r.mission - 1);
            const auto it = std::find(options.begin(), options.end(), a.value);
            if (it == options.end()) { r.valid = false; break; }
            solved = it == options.begin() + 1;
            cost = solved ? 1 : 2;
        }
        if (cost > r.credits) { r.valid = false; break; }
        r.credits -= cost; r.cost += cost; r.node = next;
        archive = archive || next == 'E';
        if (solved) {
            r.proofs.push_back({r.mission + 1, r.cost, r.mission == 0 && archive && r.cost == 5});
            ++r.mission; r.cost = 0; r.node = 'S'; archive = false;
        }
    }
    return r;
}

struct Entry { std::string id; Result result; };
void rank(std::vector<Entry>& rows) {
    std::erase_if(rows, [](const auto& e){ return !e.result.valid || e.result.mission != 3; });
    std::sort(rows.begin(), rows.end(), [](const auto& a, const auto& b){
        return std::tie(b.result.credits, a.id) < std::tie(a.result.credits, b.id);
    });
}

void require(bool ok, const char* label) {
    if (!ok) throw std::runtime_error(label);
    std::cout << "PASS " << label << '\n';
}

void selftest() {
    const std::vector<Action> optimal{{0,"B"},{0,"E"},{0,"P"},{1,"freeze_check_quota"},{2,"denied_charge_0"}};
    auto best = verify(optimal);
    require(best.valid && best.mission == 3 && best.credits == 8, "optimal replay");
    require(best.proofs.size() == 3 && best.proofs[0].hidden && best.proofs[0].cost == 5, "hidden proof");
    auto wrong = optimal; wrong.insert(wrong.begin()+3,{1,"retry"});
    auto slower = verify(wrong);
    require(slower.valid && slower.credits == 6 && slower.proofs[1].cost == 3, "wrong answer charged");
    require(!verify({{0,"P"}}).valid, "invalid edge rejected");
    require(!verify({{1,"freeze_check_quota"}}).valid, "out of order rejected");
    auto extra = optimal; extra.push_back({2,"denied_charge_0"});
    require(!verify(extra).valid, "post-completion action rejected");
    auto exhausted = optimal; exhausted.erase(exhausted.begin()+3,exhausted.end());
    for(int i=0;i<6;++i) exhausted.push_back({1,"retry"});
    auto limited = verify(exhausted);
    require(!limited.valid && limited.credits == 0 && limited.proofs.size() == 1, "budget fail closed");
    auto unknown = optimal; unknown[3].value = "forged";
    require(!verify(unknown).valid, "unknown action rejected");
    require(!verify(std::vector<Action>(16,{0,"A"})).valid, "oversized trace rejected");
    std::vector<Entry> rows{{"slow",slower},{"z",best},{"a",best},{"invalid",verify(extra)},{"partial",verify({})}};
    rank(rows);
    require(rows.size()==3 && rows[0].id=="a" && rows[1].id=="z" && rows[2].id=="slow", "ranking and deterministic tie break");
    require(verify(optimal).credits == best.credits, "deterministic replay");
}
}

int main() {
    try { vamos::selftest(); }
    catch(const std::exception& e) { std::cerr << "FAIL " << e.what() << '\n'; return 1; }
}
