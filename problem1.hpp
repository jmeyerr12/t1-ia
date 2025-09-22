// problem1.hpp
#pragma once
#include <bits/stdc++.h>
#include "astar.hpp"
#include "heuristics.hpp"             
using namespace std;

// estado interno do problema 1: grade 3x6
struct P1State {
    char g[3][6];
};

StatePtr p1_initial();                        // cria o estado inicial do tabuleiro
bool     p1_is_goal(StatePtr s);              // verifica se k está na posição objetivo
void     p1_successors(StatePtr s,            // gera todos os sucessores possíveis
                       vector<StatePtr>& out_states,
                       vector<int>& out_costs,
                       vector<string>& out_actions);
void     p1_to_key(StatePtr s, string& key);  // gera chave única em string pro hashing
StatePtr p1_clone(StatePtr s);                // cria cópia independente do estado
void     p1_free_state(StatePtr s);           // libera memória do estado

void p1_print(const P1State* s);              // imprime o tabuleiro (debug)
bool p1_apply_action(P1State* s, const string& act); // aplica ação "<p r0 c0 r1 c1>"
