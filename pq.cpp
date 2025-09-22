// pq.cpp
#include "pq.hpp"

struct PQueue {
    struct Cmp {
        bool operator()(const PQNode& a, const PQNode& b) const {
            if (a.f != b.f) return a.f > b.f; // menor f primeiro
            if (a.g != b.g) return a.g < b.g; // preferir g maior (mais profundo)
            return a.tie > b.tie;             // estável
        }
    };
    std::priority_queue<PQNode, std::vector<PQNode>, Cmp> q;
};

PQueue* pq_create() { return new PQueue(); }
void    pq_free(PQueue* pq) { delete pq; }

bool pq_empty(const PQueue* pq) { return pq->q.empty(); }

void pq_push(PQueue* pq, const PQNode& item) { pq->q.push(item); }

PQNode pq_pop(PQueue* pq) {
    PQNode top = pq->q.top();
    pq->q.pop();
    return top;
}

void pq_drain_and_free_states(PQueue* pq, void (*free_state)(StatePtr)) {
    while (!pq->q.empty()) {
        auto it = pq->q.top(); pq->q.pop();
        if (free_state) free_state(it.s);
    }
}
