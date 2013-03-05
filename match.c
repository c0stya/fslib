#include "fst.h"
#include "queue.h"


//static int _match(label_t il, label_t ol) {

/*
static int _match(struct _arc a, struct _arc b) {
    // Assume 0 is an epsilon
    return a.olabel == b.ilabel || a.olabel == EPS || b.ilabel == EPS;
} 

static int _match_eps(struct _arc a, struct _arc b, state_t f) {
    assert ( f != -1 );
    label_t ol = a.olabel;
    label_t il = b.ilabel;

    if (ol == il && ol != EPS && ol != EPS_L)
        return 0;
    if (ol == EPS && il == EPS && f == 0)
        return 0;
    if (ol == EPS_L && il == EPS && f != 2)
        return 1;
    if (ol == EPS && il == EPS_L && f != 1)
        return 2;

    return -1;
}
*/

static int _match(struct _arc a, struct _arc b, state_t f) {
    assert ( f != -1 );
    label_t ol = a.olabel;
    label_t il = b.ilabel;

    if (ol == il && ol != EPS && ol != EPS_L)
        return 0;
    if (ol == EPS && il == EPS_L && f == 0)
        return 0;
    if (ol == EPS_L && il == EPS)
        return 1;

    return -1;
}

void match_unsorted(
                struct _arc * a,
                struct _arc * b,
                state_t f,
                arc_t M,
                arc_t N,
                struct _queue * q) 
{
    state_t nf;
    struct _match_item mi;

    //printf("Match unsorted\n");
    for (arc_t i = 0; i < M; ++i)
        for (arc_t j = 0; j < N; ++j)
            if ( (nf = _match(a[i], b[j], f)) != -1) {
                mi.a = a[i];
                mi.b = b[j];
                mi.f = nf;
                queue_enque(q, &mi);
            }
}

void match_half_sorted(
                struct _arc * a,
                struct _arc * b,
                state_t f,
                arc_t M,
                arc_t N,
                struct _queue * q) 
{
    arc_t l, h, ll, hh;
    arc_t i,m;
    state_t nf; // new filter state

    struct _match_item mi;
    //fprintf(stderr, "Match half sorted\n");

    for (i=0; i < M; ++i) {
        l = 0;
        h = N-1;
        while ( l <= h ) {
            m = (l + h) >> 1;
            if (a[i].olabel > b[m].ilabel) l = m + 1; 
            else 
                if (a[i].olabel < b[m].ilabel) h = m - 1;
                else {
                    ll = hh = m;

                    while ( (nf = _match(a[i], b[ll], f)) != -1 && ll > l ) --ll;
                    while ( (nf = _match(a[i], b[hh], f)) != -1 && hh < h ) ++hh;

                    while (ll <= hh ) {
                        mi.a = a[i]; 
                        mi.b = b[ll];
                        mi.f = nf;
                        queue_enque(q, &mi);
                        ++ll;
                    }
                    break;
                }
        }
    }
}

/* Linear time arcs matching for two SORTED arrays of arcs */
void match_full_sorted(
                struct _arc * a,
                struct _arc * b,
                state_t f,
                arc_t M,
                arc_t N,
                struct _queue *   q) 
{
    arc_t i = 0; 
    arc_t j = 0; 
    arc_t t;

    state_t nf;
    struct _match_item mi;

    while (i < M && j < N) {
        if ( a[i].ilabel <  b[j].olabel ) ++i;
        else
            if ( a[i].ilabel > b[j].olabel ) ++j;
            else {
                for (t=j; (nf = _match(a[i], b[t], f)) != -1; ++t ) {
                    mi.a = a[i]; 
                    mi.b = b[t]; 
                    mi.f = nf;
                    queue_enque(q, &mi);
                }
                ++i;
            }
    }
}

