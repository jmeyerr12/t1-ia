// astar.cpp
#include "astar.hpp"
#include "pq.hpp"
#include <queue>
#include <unordered_map>
#include <limits>
#include <cassert>

using namespace std;

AStarResult astar_solve(const AStarAPI& api) {
    AStarResult res;

    PQueue* open = pq_create();
    unordered_map<string, int> best_g;                 // key -> melhor g
    unordered_map<string, pair<string,string>> parent; // key -> (key_pai, ação)
    unordered_map<string, StatePtr> repo;              // chave -> clone canônico

    int tie_counter = 0;

    // inicial
    StatePtr s0 = api.initial();
    string k0; api.to_key(s0, k0);
    int g0 = 0;
    int f0 = g0 + api.heuristic(s0);

    pq_push(open, PQNode{s0, g0, f0, tie_counter++});
    best_g[k0] = 0;
    repo[k0] = api.clone(s0);

    while (!pq_empty(open)) {
        PQNode cur = pq_pop(open);
        string kcur; api.to_key(cur.s, kcur);

        // lazy-delete: ignora entradas obsoletas
        auto itg = best_g.find(kcur);
        if (itg == best_g.end() || cur.g != itg->second) {
            api.free_state(cur.s);
            continue;
        }

        res.expanded++;

        if (api.is_goal(cur.s)) {
            // reconstrói plano
            vector<string> rev;
            string key = kcur;
            while (true) {
                auto itp = parent.find(key);
                if (itp == parent.end()) break;
                rev.push_back(itp->second.second);
                key = itp->second.first;
            }
            for (int i = (int)rev.size()-1; i>=0; --i) res.plan.push_back(rev[i]);
            res.found = true;

            // limpeza
            pq_drain_and_free_states(open, api.free_state);
            pq_free(open);
            for (auto& kv : repo) api.free_state(kv.second);
            api.free_state(cur.s);
            return res;
        }

        // expandir
        vector<StatePtr> nxt_states;
        vector<int> nxt_costs;
        vector<string> nxt_actions;
        api.successors(cur.s, nxt_states, nxt_costs, nxt_actions);
        assert(nxt_states.size() == nxt_costs.size() && nxt_states.size() == nxt_actions.size());

        for (size_t i=0;i<nxt_states.size();++i) {
            StatePtr ns = nxt_states[i];
            int step = nxt_costs[i];
            const string& act = nxt_actions[i];

            string kn; api.to_key(ns, kn);
            int tentative_g = cur.g + step;

            auto it_old = best_g.find(kn);
            if (it_old == best_g.end() || tentative_g < it_old->second) {
                best_g[kn] = tentative_g;
                parent[kn] = {kcur, act};

                auto it_repo = repo.find(kn);
                if (it_repo == repo.end()) {
                    repo[kn] = api.clone(ns);
                } else {
                    api.free_state(it_repo->second);
                    repo[kn] = api.clone(ns);
                }

                int f = tentative_g + api.heuristic(ns);
                pq_push(open, PQNode{ns, tentative_g, f, tie_counter++});
            } else {
                api.free_state(ns);
            }
        }

        api.free_state(cur.s);
    }

    // sem solução
    for (auto& kv : repo) api.free_state(kv.second);
    pq_free(open);
    res.found = false;
    return res;
}
