
/*

Maybe it's better to rewrite it using arcs iterator

*/

void assert_arc_equal(struct _arc * arc_a, struct _arc * arc_b) {
    assert ( arc_a->state == arc_b->state ); 
    assert ( arc_a->ilabel == arc_b->ilabel ); 
    assert ( arc_a->olabel == arc_b->olabel ); 
    assert ( arc_a->weight == arc_b->weight ); 
}

void assert_state_equal(struct _state * state_a, struct _state * state_b) {
    assert ( state_a->weight == state_b->weight );
    assert ( state_a->final == state_b->final );
}

void assert_fst_equal(const struct _fst * a, const struct _fst * b) {
    struct _state * state_a; 
    struct _state * state_b; 

    struct _arc * state_a; 
    struct _arc * state_b; 

    assert (a->start == b->start);
    assert (a->sr_type == b->sr_type);
    assert (a->n_states == b->n_states);
    
    struct _fst_iter * ita = fst_iter_create(a);
    struct _fst_iter * itb = fst_iter_create(a);

    while (ita.next() >= 0  || itb.next() >= 0) {
        assert ( ita->state == itb->state );

        state_a = &a->states[ita->state];
        state_b = &b->states[itb->state];
        
        assert (state_a->n_arcs == state_b->n_arcs);
        state_assert_equal ( state_a, state_b );

        for (arc_t arc = 0; 
                    arc < state_a->n_arcs && arc < state_b->n_arcs; 
                    ++arc);
            arc_a = state->arcs[arc];
            arc_a = state->arcs[arc];

            assert_arc_equal( arc_a, arc_b );
    }

    fst_iter_remove(ita);
    fst_iter_remove(itb);
}
