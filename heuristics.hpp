// heuristics.hpp
#pragma once
#include <bits/stdc++.h>
using namespace std;

// ponteiro genérico de estado (mesmo alias usado no projeto)
using StatePtr = void*;

int p1_heuristic(StatePtr s); // heurística do Problema 1 (A*): estima custo restante até k em (3,6)
int p2_heuristic(StatePtr s); // heurística do Problema 2 (A*): estima custo restante em duas fases (peão até promoção em (1,4), depois rainha até (4,1))
