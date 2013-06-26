#include "fst.h"
#include "queue.h"


//static int _match(label_t il, label_t ol) {

static int _match(struct _arc * a, struct _arc * b, arc_t i, arc_t j) {
    label_t al = a[i].olabel; 

    if (al == EPS)
        if ( (i != 0 && j != 0) || (i == 0 && j == 0)) {
            return 0;
        }
    return 1;
}

void match_unsorted(
                struct _arc * a,
                struct _arc * b,
                arc_t M,
                arc_t N,
                struct _queue * q) 
{
    struct _match_item mi;

    for (arc_t i = 0; i < M; ++i)
        for (arc_t j = 0; j < N; ++j)
            if ( a[i].olabel == b[j].ilabel && _match(a, b, i, j)) {
                mi.a = a[i];
                mi.b = b[j];
                queue_enque(q, &mi);
            }
}

void match_half_sorted(
                struct _arc * a,
                struct _arc * b,
                arc_t M,
                arc_t N,
                struct _queue * q) 
{
    arc_t l, h, ll, hh;
    arc_t i,m;

    struct _match_item mi;

    for (i=0; i < M; ++i) {
        l = 0;
        h = N-1;
        while ( l <= h ) {
            m = (l + h) >> 1;
            if (a[i].olabel > b[m].ilabel) l = m + 1; 
            else  
                if (a[i].olabel < b[m].ilabel) {
                    if (m == 0) break;  // sentinel for unsigned index h
                    h = m - 1;
                }
                else {
                    ll = hh = m;
                    
                    while ((ll > l) && (a[i].olabel == b[ll-1].ilabel)) --ll;
                    while ((hh < h) && (a[i].olabel == b[hh+1].ilabel)) ++hh;

                    while (ll <= hh ) {
                        mi.a = a[i]; 
                        mi.b = b[ll];
                        if (_match(a, b, i, ll))
                            queue_enque(q, &mi);
                        ++ll;
                    }
                    break;
                }
        }
    }
}

// TODO: merge with previous method 
void match_half_sorted_rev(
                struct _arc * a,
                struct _arc * b,
                arc_t M,
                arc_t N,
                struct _queue * q) 
{
    arc_t l, h, ll, hh;
    arc_t i,m;

    struct _match_item mi;

    for (i=0; i < N; ++i) {
        l = 0;
        h = M-1;
        while ( l <= h ) {
            m = (l + h) >> 1;
            if (b[i].ilabel > a[m].olabel) l = m + 1; 
            else  
                if (b[i].ilabel < a[m].olabel) {
                    if (m == 0) break;  // sentinel for unsigned index h
                    h = m - 1;
                }
                else {
                    ll = hh = m;
                    
                    while ((ll > l) && (b[i].ilabel == a[ll-1].olabel)) --ll;
                    while ((hh < h) && (b[i].ilabel == a[hh+1].olabel)) ++hh;

                    while (ll <= hh ) {
                        mi.a = a[ll];
                        mi.b = b[i]; 
                        if (_match(a, b, ll, i))
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
                arc_t M,
                arc_t N,
                struct _queue *   q) 
{
    arc_t i = 0; 
    arc_t j = 0; 
    arc_t t;

    struct _match_item mi;

    while (i < M && j < N) {
        if ( a[i].olabel <  b[j].ilabel ) ++i;
        else
            if ( a[i].olabel > b[j].ilabel ) ++j;
            else {
                for (t=j; a[i].olabel == b[t].ilabel; ++t ) {
                    mi.a = a[i]; 
                    mi.b = b[t]; 
                    if (_match(a, b, i, t))
                        queue_enque(q, &mi);
                }
                ++i;
            }
    }
}

