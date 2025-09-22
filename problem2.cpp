// problem2.cpp
#include <bits/stdc++.h>
#include "problem2.hpp"
using namespace std;

// helpers locais (prefixo p2_ para evitar conflito)
static inline bool p2_inside(int r, int c) { return r>=0 && r<4 && c>=0 && c<4; }
static inline bool p2_empty(char ch)       { return ch == '_'; }
static inline bool p2_wall(char ch)        { return ch == '#'; }
static inline bool p2_piece(char ch)       { return ch=='p'||ch=='q'||ch=='b'||ch=='c'||ch=='t'; }

// formata ação: "<peca> r0 c0 r1 c1" (1-based)
static string p2_action(char piece, int r0,int c0,int r1,int c1) {
    return string(1,piece) + " " +
           to_string(r0+1) + " " + to_string(c0+1) + " " +
           to_string(r1+1) + " " + to_string(c1+1);
}

// gera movimentos deslizantes (torre/bispo/rainha)
static void p2_gen_slide(const P2State* s, int r, int c,
                         const vector<pair<int,int>>& dirs,
                         vector<StatePtr>& out_states,
                         vector<int>& out_costs,
                         vector<string>& out_actions)
{
    char piece = s->g[r][c];
    for (auto [dr,dc] : dirs) {
        int nr = r + dr, nc = c + dc;
        while (p2_inside(nr,nc) && !p2_wall(s->g[nr][nc]) && !p2_piece(s->g[nr][nc])) {
            if (!p2_empty(s->g[nr][nc])) break; // destino precisa ser vazio
            P2State* ns = new P2State(*s);
            ns->g[nr][nc] = piece;
            ns->g[r][c]   = '_';
            out_states.push_back((StatePtr)ns);
            out_costs.push_back(1);
            out_actions.push_back(p2_action(piece, r,c, nr,nc));
            nr += dr; nc += dc;
        }
    }
}

// movimentos do cavalo
static void p2_gen_knight(const P2State* s, int r, int c,
                          vector<StatePtr>& out_states,
                          vector<int>& out_costs,
                          vector<string>& out_actions)
{
    static const int KDR[8] = {-2,-2,-1,-1, 1, 1, 2, 2};
    static const int KDC[8] = {-1, 1,-2, 2,-2, 2,-1, 1};
    char piece = s->g[r][c];
    for (int i=0;i<8;i++) {
        int nr = r + KDR[i], nc = c + KDC[i];
        if (!p2_inside(nr,nc)) continue;
        if (p2_wall(s->g[nr][nc])) continue;
        if (!p2_empty(s->g[nr][nc])) continue;
        P2State* ns = new P2State(*s);
        ns->g[nr][nc] = piece;
        ns->g[r][c]   = '_';
        out_states.push_back((StatePtr)ns);
        out_costs.push_back(1);
        out_actions.push_back(p2_action(piece, r,c, nr,nc));
    }
}

// movimentos do peão branco (anda pra cima 1 casa, sem captura)
static void p2_gen_pawn(const P2State* s, int r, int c,
                        vector<StatePtr>& out_states,
                        vector<int>& out_costs,
                        vector<string>& out_actions)
{
    int nr = r - 1, nc = c; // sobe (linha diminui)
    if (!p2_inside(nr,nc)) return;
    if (p2_wall(s->g[nr][nc])) return;
    if (!p2_empty(s->g[nr][nc])) return;

    P2State* ns = new P2State(*s);
    // promoção: se destino é (0,3) => vira rainha 'q'
    if (nr == 0 && nc == 3) {
        ns->g[nr][nc] = 'q';
    } else {
        ns->g[nr][nc] = 'p';
    }
    ns->g[r][c] = '_';

    out_states.push_back((StatePtr)ns);
    out_costs.push_back(1);
    out_actions.push_back(p2_action('p', r,c, nr,nc));
}

// --- callbacks ---

StatePtr p2_initial() {
    // estado inicial conforme enunciado:
    // 1: c c c c
    // 2: b b b b
    // 3: t t t t
    // 4: _ # # p
    P2State* s = new P2State;
    const char* l1 = "cccc";
    const char* l2 = "bbbb";
    const char* l3 = "tttt";
    const char* l4 = "_##p";
    for (int c=0;c<4;c++) {
        s->g[0][c]=l1[c];
        s->g[1][c]=l2[c];
        s->g[2][c]=l3[c];
        s->g[3][c]=l4[c];
    }
    return (StatePtr)s;
}

bool p2_is_goal(StatePtr sp) {
    auto* s = (P2State*)sp;
    // objetivo: rainha 'q' em (4,1) => (3,0) 0-based
    return s->g[3][0] == 'q';
}

void p2_successors(StatePtr sp,
                   vector<StatePtr>& out_states,
                   vector<int>& out_costs,
                   vector<string>& out_actions)
{
    auto* s = (P2State*)sp;
    for (int r=0;r<4;r++) {
        for (int c=0;c<4;c++) {
            char ch = s->g[r][c];
            if (!p2_piece(ch)) continue;

            switch (ch) {
                case 't': { // torre
                    static const vector<pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
                    p2_gen_slide(s, r,c, dirs, out_states, out_costs, out_actions);
                    break;
                }
                case 'b': { // bispo
                    static const vector<pair<int,int>> dirs = {{-1,-1},{-1,1},{1,-1},{1,1}};
                    p2_gen_slide(s, r,c, dirs, out_states, out_costs, out_actions);
                    break;
                }
                case 'q': { // rainha
                    static const vector<pair<int,int>> dirs = {
                        {-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1}
                    };
                    p2_gen_slide(s, r,c, dirs, out_states, out_costs, out_actions);
                    break;
                }
                case 'c': // cavalo
                    p2_gen_knight(s, r,c, out_states, out_costs, out_actions);
                    break;
                case 'p': // peão
                    p2_gen_pawn(s, r,c, out_states, out_costs, out_actions);
                    break;
                default:
                    break;
            }
        }
    }
}

void p2_to_key(StatePtr sp, string& key) {
    auto* s = (P2State*)sp;
    key.clear();
    key.reserve(16);
    for (int r=0;r<4;r++) for (int c=0;c<4;c++) key.push_back(s->g[r][c]);
}

StatePtr p2_clone(StatePtr sp) {
    auto* s = (P2State*)sp;
    return (StatePtr)(new P2State(*s));
}

void p2_free_state(StatePtr sp) {
    delete (P2State*)sp;
}

// --- debug helpers (impressão e aplicação de ação) ---
static bool p2_parse_act(const string& a, char& pc,int& r0,int& c0,int& r1,int& c1){
    istringstream iss(a);
    string p; if(!(iss>>p>>r0>>c0>>r1>>c1)) return false;
    pc = p[0]; r0--; c0--; r1--; c1--;
    return true;
}

void p2_print(const P2State* s){
    cout << "   1234\n  ######\n";
    for (int r=0;r<4;r++){
        cout << (r+1) << " #";
        for (int c=0;c<4;c++) cout << s->g[r][c];
        cout << "#\n";
    }
    cout << "  ######\n";
}

bool p2_apply_action(P2State* s, const string& act){
    char pc; int r0,c0,r1,c1;
    if(!p2_parse_act(act,pc,r0,c0,r1,c1)) return false;
    if (!p2_inside(r0,c0) || !p2_inside(r1,c1)) return false;
    if (s->g[r0][c0] != pc) return false;
    if (!p2_empty(s->g[r1][c1])) return false; // sem captura

    // promoção automática se peão chegar em (0,3)
    if (pc == 'p' && r1 == 0 && c1 == 3) {
        s->g[r1][c1] = 'q';
    } else {
        s->g[r1][c1] = pc;
    }
    s->g[r0][c0] = '_';
    return true;
}
