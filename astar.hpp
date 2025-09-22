// astar.hpp
#pragma once
#include <vector>
#include <string>
#include <functional>

using namespace std;

// ponteiro genérico para estado (cada problema define sua struct e faz cast)
using StatePtr = void*;

// funções que cada problema precisa fornecer pro A*
struct AStarAPI {
    StatePtr (*initial)();   // cria estado inicial
    bool (*is_goal)(StatePtr s);
    void (*successors)(StatePtr s,
                       vector<StatePtr>& out_states,
                       vector<int>& out_costs,
                       vector<string>& out_actions);
    int (*heuristic)(StatePtr s);
    void (*to_key)(StatePtr s, string& key); // chave única do estado
    StatePtr (*clone)(StatePtr s);
    void (*free_state)(StatePtr s);
};

// resultado do A*
struct AStarResult {
    bool found = false;
    vector<string> plan; // ações do início ao objetivo
    int expanded = 0;    // estados expandidos
};

// resolve usando A*
AStarResult astar_solve(const AStarAPI& api);
