// main.cpp
#include <bits/stdc++.h>
#include "astar.hpp"
#include "problem1.hpp"
#include "problem2.hpp"
#include "heuristics.hpp"

using namespace std;

static bool debug_mode = false; //  -d: replay visual
static bool stats_mode = false;   // -x: mostra "expandidos"

static void run_problem_1() {
    AStarAPI api = { &p1_initial, &p1_is_goal, &p1_successors, &p1_heuristic,
                     &p1_to_key, &p1_clone, &p1_free_state };

    AStarResult res = astar_solve(api);
    if (!res.found) { cout << "sem-solucao\n"; return; }
    for (auto& a : res.plan) cout << a << "\n";
    if (stats_mode) cerr << "expandidos=" << res.expanded << "\n";

    if (debug_mode) {
        P1State* cur = (P1State*)p1_initial();
        cerr << "\nReplay P1:\n";
        p1_print(cur);
        for (auto& a : res.plan) {
            p1_apply_action(cur, a);
            cerr << a << "\n";
            p1_print(cur);
        }
        p1_free_state(cur);
    }
}

static void run_problem_2() {
    AStarAPI api = { &p2_initial, &p2_is_goal, &p2_successors, &p2_heuristic,
                     &p2_to_key, &p2_clone, &p2_free_state };

    AStarResult res = astar_solve(api);
    if (!res.found) { cout << "sem-solucao\n"; return; }
    for (auto& a : res.plan) cout << a << "\n";
    if (stats_mode) cerr << "expandidos=" << res.expanded << "\n";

    if (debug_mode) {
        P2State* cur = (P2State*)p2_initial();
        cerr << "\nReplay P2:\n";
        p2_print(cur);
        for (auto& a : res.plan) {
            p2_apply_action(cur, a);
            cerr << a << "\n";
            p2_print(cur);
        }
        p2_free_state(cur);
    }
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int problem = 1; // default

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-d") debug_mode = true;
        else if (arg == "-x") stats_mode = true;
        else problem = atoi(argv[i]);
    }

    if (problem == 1)      run_problem_1();
    else if (problem == 2) run_problem_2();
    else                   cout << "Uso: ./run [1|2] [-d] [-x]\n";
    return 0;
}
