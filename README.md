fslib
=====

Finite state transducer library. Minimalist pure C implementation. This project is mostly inspired by OpenFST library (http://www.openfst.org) by Michael Riley and Mehriar Mohri. The main focus here is performance and simplicity. 

<img src="examples/lev.png" height="250" width="500">

Setup
-----

C99 compiler is required. Just run:

```bash
git clone https://github.com/c0stya/fslib.git
cd fslib && make
```

To run compile and run tests:

```bash
make tests
```

Usage
-----

### Compilation

Let's create an automaton that accepts "(01)\*" regular language. We need a *fscompile* command:

```bash
echo  "\
0 1 0 0
1 0 1 1
0" | fscompile > automaton.t
```

Each line contains either transition or final state.

Transition line format:
* source state
* destination state
* input label
* output label
* (optional) weight

Final state line contains only a number of a state.

### Printing

To print it just invoke *fsprint* command:
```bash
fsprint automaton.t
```
or as usual pipe:
```bash
cat automaton.t | fsprint
```

### Composition

To compose transducers one should use the *fscompose* command:

```bash
fscompose aut0.t aut1.t > aut3.t
```

It could be done in the pipe style with more then two automata:
```bash
cat aut0.t | fscompose - aut1.t | fscompose - aut2.t > aut3.t
```

Warning: a proper sorting of input/output arcs could be significantly increase the performance.

### Shortest path

Use *fsshort* command to get the a transducer with the shortest path:

```bash
fsshort automaton.t > shortest_path.t
```

### Drawing

We could draw automaton using *fsdraw* command. To actually render it you need *dot* utility from the Graphviz package.  Make sure you have Graphviz (http://www.graphviz.org) package properly installed.

To render a transducer to a png file use the following:

```bash
fsdraw automaton.t | dot -Tpng > automaton.png
```

