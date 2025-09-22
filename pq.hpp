// pq.hpp
#pragma once
#include <bits/stdc++.h>
using namespace std;

using StatePtr = void*; // mesmo alias do projeto

// item que vai para a fila
struct PQNode {
    StatePtr s;  // estado
    int g;       // custo acumulado
    int f;       // f = g + h
    int tie;     // desempate estável (ordem de inserção)
};

// tipo opaco da fila
struct PQueue;

// cria e destrói a fila
PQueue* pq_create();
void    pq_free(PQueue* pq);

// operações básicas
bool    pq_empty(const PQueue* pq);
void    pq_push(PQueue* pq, const PQNode& item);
PQNode  pq_pop(PQueue* pq); // remove o menor (por f)

// esvazia a fila liberando os StatePtr com o callback fornecido
void    pq_drain_and_free_states(PQueue* pq, void (*free_state)(StatePtr));
