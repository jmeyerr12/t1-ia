// problem1.cpp
#include <bits/stdc++.h>
#include "problem1.hpp"
using namespace std;

// --- Utilidades ---
static inline bool inside(int r, int c) { return (r>=0 && r<3 && c>=0 && c<6); }
static inline bool cell_is_empty(char ch) { return ch == '_'; }
static inline bool is_wall(char ch)     { return ch == '#'; }
static inline bool is_piece(char ch)    { return ch=='k'||ch=='b'||ch=='c'||ch=='t'; }

// Move textual no formato pedido: "<peca> r0 c0 r1 c1" (1-based)
static string action_str(char piece, int r0,int c0,int r1,int c1) {
    return string(1,piece) + " " +
           to_string(r0+1) + " " + to_string(c0+1) + " " +
           to_string(r1+1) + " " + to_string(c1+1);
}

// Gera movimentos de deslizantes (bispo/torre) com bloqueio
static void gen_slide_moves(const P1State* s, int r, int c,
                            const vector<pair<int,int>>& dirs,
                            vector<StatePtr>& out_states,
                            vector<int>& out_costs,
                            vector<string>& out_actions)
{
    char piece = s->g[r][c];
    for (auto [dr,dc] : dirs) {
        int nr = r + dr, nc = c + dc;
        while (inside(nr,nc) && !is_wall(s->g[nr][nc]) && !is_piece(s->g[nr][nc])) {
            // destino precisa ser vazio
            if (!cell_is_empty(s->g[nr][nc])) break;
            // cria novo estado aplicando o movimento
            P1State* ns = new P1State(*s);
            ns->g[nr][nc] = piece;
            ns->g[r][c]   = '_';
            out_states.push_back((StatePtr)ns);
            out_costs.push_back(1);
            out_actions.push_back(action_str(piece, r,c, nr,nc));
            // continua deslizando na mesma direção
            nr += dr; nc += dc;
        }
    }
}

// Gera movimentos do cavalo (tanto 'k' quanto 'c')
static void gen_knight_moves(const P1State* s, int r, int c,
                             vector<StatePtr>& out_states,
                             vector<int>& out_costs,
                             vector<string>& out_actions)
{
    static const int KDR[8] = {-2,-2,-1,-1, 1, 1, 2, 2};
    static const int KDC[8] = {-1, 1,-2, 2,-2, 2,-1, 1};
    char piece = s->g[r][c];
    for (int i=0;i<8;i++) {
        int nr = r + KDR[i], nc = c + KDC[i];
        if (!inside(nr,nc)) continue;
        if (is_wall(s->g[nr][nc])) continue;
        if (!cell_is_empty(s->g[nr][nc])) continue; // sem captura, só destino vazio
        P1State* ns = new P1State(*s);
        ns->g[nr][nc] = piece;
        ns->g[r][c]   = '_';
        out_states.push_back((StatePtr)ns);
        out_costs.push_back(1);
        out_actions.push_back(action_str(piece, r,c, nr,nc));
    }
}

// --- Callbacks exigidos pela A* ---

StatePtr p1_initial() {
    // Estado consistente com o enunciado (ajustado):
    // L1: k b b b b t
    // L2: c c c c t t
    // L3: # # # # t _
    P1State* s = new P1State;
    const char* l1 = "kbbbbt";
    const char* l2 = "cccctt";
    const char* l3 = "####t_"; // 3x6
    for (int c=0;c<6;c++) { s->g[0][c]=l1[c]; s->g[1][c]=l2[c]; s->g[2][c]=l3[c]; }
    return (StatePtr)s;
}

bool p1_is_goal(StatePtr sp) {
    auto* s = (P1State*)sp;
    // objetivo: 'k' em (3,6) -> índices 0-based: (2,5)
    return s->g[2][5] == 'k';
}

void p1_successors(StatePtr sp,
                   vector<StatePtr>& out_states,
                   vector<int>& out_costs,
                   vector<string>& out_actions)
{
    auto* s = (P1State*)sp;

    for (int r=0;r<3;r++) {
        for (int c=0;c<6;c++) {
            char ch = s->g[r][c];
            if (!is_piece(ch)) continue;

            switch (ch) {
                case 't': {
                    // Torre: ortogonais
                    static const vector<pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
                    gen_slide_moves(s, r,c, dirs, out_states, out_costs, out_actions);
                    break;
                }
                case 'b': {
                    // Bispo: diagonais
                    static const vector<pair<int,int>> dirs = {{-1,-1},{-1,1},{1,-1},{1,1}};
                    gen_slide_moves(s, r,c, dirs, out_states, out_costs, out_actions);
                    break;
                }
                case 'k': // cavalo preto
                case 'c': // cavalo branco
                    gen_knight_moves(s, r,c, out_states, out_costs, out_actions);
                    break;
                default:
                    break;
            }
        }
    }
}

void p1_to_key(StatePtr sp, string& key) {
    auto* s = (P1State*)sp;
    key.clear();
    key.reserve(18);
    for (int r=0;r<3;r++) for (int c=0;c<6;c++) key.push_back(s->g[r][c]);
}

StatePtr p1_clone(StatePtr sp) {
    auto* s = (P1State*)sp;
    return (StatePtr)(new P1State(*s));
}

void p1_free_state(StatePtr sp) {
    delete (P1State*)sp;
}

void p1_print(const P1State* s){
    cout << "   123456\n  ########\n";
    for(int r=0;r<3;r++){
        cout << (r+1) << " #";
        for(int c=0;c<6;c++) cout << s->g[r][c];
        cout << "#\n";
    }
    cout << "  ########\n";
}

static bool parse_act(const string& a, char& pc,int& r0,int& c0,int& r1,int& c1){
    // formato: "<peca> r0 c0 r1 c1"  (1-based)
    istringstream iss(a);
    string p; if(!(iss>>p>>r0>>c0>>r1>>c1)) return false;
    pc = p[0]; r0--; c0--; r1--; c1--; return true;
}

bool p1_apply_action(P1State* s, const string& act){
    char pc; int r0,c0,r1,c1;
    if(!parse_act(act,pc,r0,c0,r1,c1)) return false;
    if (r0<0||r0>=3||c0<0||c0>=6||r1<0||r1>=3||c1<0||c1>=6) return false;
    if (s->g[r0][c0]!=pc) return false;
    if (s->g[r1][c1] != '_') return false; // sem captura
    s->g[r1][c1] = pc;
    s->g[r0][c0] = '_';
    return true;
}
