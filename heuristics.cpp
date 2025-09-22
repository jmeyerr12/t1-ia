// heuristics.cpp
#include <bits/stdc++.h>
#include "heuristics.hpp"
using namespace std;

// ===== P1 =====
struct P1State { char g[3][6]; };
static inline bool p1_inside(int r,int c){ return r>=0 && r<3 && c>=0 && c<6; }
static inline bool p1_wall(char ch){ return ch=='#'; }

int p1_heuristic(StatePtr sp) {
    auto* s = (P1State*)sp;
    int kr=-1,kc=-1;
    for (int r=0;r<3;r++) for (int c=0;c<6;c++)
        if (s->g[r][c]=='k'){ kr=r; kc=c; }
    if (kr==-1) return 0;

    static bool built=false;
    static int dist[3][6];
    if (!built) {
        for (int r=0;r<3;r++) for (int c=0;c<6;c++) dist[r][c]=INT_MAX/4;
        queue<pair<int,int>> q;
        dist[2][5]=0;
        q.push({2,5});
        static const int KDR[8]={-2,-2,-1,-1,1,1,2,2};
        static const int KDC[8]={-1,1,-2,2,-2,2,-1,1};
        while(!q.empty()){
            auto [r,c]=q.front(); q.pop();
            for (int i=0;i<8;i++){
                int nr=r+KDR[i], nc=c+KDC[i];
                if(!p1_inside(nr,nc)) continue;
                if(p1_wall(s->g[nr][nc])) continue;
                if(dist[nr][nc] > dist[r][c]+1){
                    dist[nr][nc]=dist[r][c]+1;
                    q.push({nr,nc});
                }
            }
        }
        built=true;
    }
    int h = dist[kr][kc];
    if (h >= INT_MAX/8) h = 0;
    char tg = s->g[2][5];
    if (tg != '_' && tg != 'k') h += 1;
    return h;
}

// ===== P2 =====
struct P2State { char g[4][4]; };

static inline bool p2_inside(int r,int c){ return r>=0 && r<4 && c>=0 && c<4; }
static inline bool p2_wall(char ch){ return ch=='#'; }

// distâncias mínimas de rainha com paredes, a partir do alvo (3,0)
static int qdist[4][4];
static bool qdist_built = false;

static void build_qdist_with_walls(const P2State* s) {
    for (int r=0;r<4;r++) for (int c=0;c<4;c++) qdist[r][c] = INT_MAX/4;
    deque<pair<int,int>> dq;
    qdist[3][0] = 0;
    dq.push_back({3,0});
    static const int DR[8] = {-1,1,0,0,-1,-1,1,1};
    static const int DC[8] = {0,0,-1,1,-1, 1,-1,1};
    while(!dq.empty()){
        auto [r,c] = dq.front(); dq.pop_front();
        int d = qdist[r][c];
        for (int k=0;k<8;k++){
            int nr = r + DR[k], nc = c + DC[k];
            while (p2_inside(nr,nc) && !p2_wall(s->g[nr][nc])) {
                if (qdist[nr][nc] > d + 1) {
                    qdist[nr][nc] = d + 1;
                    dq.push_back({nr,nc});
                }
                nr += DR[k]; nc += DC[k];
            }
        }
    }
    qdist_built = true;
}

int p2_heuristic(StatePtr sp) {
    auto* s = (P2State*)sp;

    int qr=-1,qc=-1, pr=-1,pc=-1;
    for (int r=0;r<4;r++) for (int c=0;c<4;c++) {
        if (s->g[r][c]=='q'){ qr=r; qc=c; }
        if (s->g[r][c]=='p'){ pr=r; pc=c; }
    }

    // ---------- fase 1: ainda há peão ----------
    if (pr != -1) {
        int h = 0;

        // se há parede acima, não estimamos subida (evita superestimar)
        bool wall_above = false;
        for (int r = pr-1; r >= 0; --r) {
            if (p2_wall(s->g[r][pc])) { wall_above = true; break; }
        }
        if (!wall_above) {
            h += pr; // passos verticais mínimos até a linha 0
            // soma nº de bloqueios na coluna (entre pr-1..1, exclui célula de promoção)
            int blockers = 0;
            for (int r = pr-1; r >= 1; --r) {
                char ch = s->g[r][pc];
                if (ch != '_' && !p2_wall(ch)) blockers++;
            }
            h += blockers;
            // +1 se a casa de promoção (0,3) estiver ocupada por algo ≠ '_' e ≠ 'p'
            char promo = s->g[0][3];
            if (promo != '_' && promo != 'p') h += 1;
        }
        // se tinha parede, h permanece 0 (admissível)
        return h;
    }

    // ---------- fase 2: já temos rainha ----------
    if (!qdist_built) build_qdist_with_walls(s);
    if (qr == -1) return 0;
    if (qr == 3 && qc == 0) return 0;

    int h = qdist[qr][qc];
    if (h >= INT_MAX/8) h = 0;

    // +1 se casa final (3,0) ocupada ≠ '_' e ≠ 'q'
    char tgt = s->g[3][0];
    if (tgt != '_' && tgt != 'q') h += 1;

    // se qdist==1 e o alvo é atingível por um raio, penaliza se houver peça no raio
    if (h == 1) {
        int dr = (3 - qr), dc = (0 - qc);
        if (dr) dr /= abs(dr);
        if (dc) dc /= abs(dc);
        int r = qr + dr, c = qc + dc;
        bool blocked = false;
        while (r != 3 || c != 0) {
            char ch = s->g[r][c];
            if (ch != '_' && !p2_wall(ch)) { blocked = true; break; }
            r += dr; c += dc;
        }
        if (blocked) h += 1;
    }

    return h;
}
