// problem2.hpp
#pragma once
#include <bits/stdc++.h>
#include "astar.hpp"
using namespace std;

// estado do problema 2: grade 4x4 (área útil)
struct P2State {
    char g[4][4];
};

StatePtr p2_initial();                        // cria o estado inicial do P2
bool     p2_is_goal(StatePtr s);              // verifica se q está na meta (4,1)
void     p2_successors(StatePtr s,            // gera sucessores (movimentos válidos)
                       vector<StatePtr>& out_states,
                       vector<int>& out_costs,
                       vector<string>& out_actions);
void     p2_to_key(StatePtr s, string& key);  // chave única do estado
StatePtr p2_clone(StatePtr s);                // clona estado
void     p2_free_state(StatePtr s);           // libera estado
void p2_print(const P2State* s);                         // imprime o 4x4 com moldura
bool p2_apply_action(P2State* s, const string& act);     // aplica "<p r0 c0 r1 c1>"
