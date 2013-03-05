CC=c99 -g -Wall

all: tests

test_fst: sr.o fst.o
	$(CC) sr.o fst.o test_fst.c -o test_fst

test_io: sr.o fst.o 
	$(CC) sr.o fst.o test_io.c -o test_io

test_print: fst.o queue.o print.o
	$(CC) fst.o queue.o print.o test_print.c -o test_print

test_queue: queue.o 
	$(CC) queue.o test_queue.c -o test_queue

test_bitset: bitset.o 
	$(CC) bitset.o test_bitset.c -o test_bitset

test_hash: hash.o 
	$(CC) hash.o test_hash.c -o test_hash

test_iter: sr.o queue.o fst.o bitset.o iter.o 
	$(CC) sr.o queue.o fst.o bitset.o iter.o test_iter.c -o test_iter

test_compile: test.o sr.o fst.o compile.o 
	$(CC) test.o sr.o fst.o compile.o test_compile.c -o test_compile

test_sort: compile.o test.o sr.o fst.o sort.o
	$(CC) test.o compile.o sr.o fst.o sort.o test_sort.c -o test_sort

test_stack: compile.o test.o sr.o fst.o 
	$(CC) test.o compile.o sr.o fst.o test_stack.c -o test_stack

test_trim: queue.o bitset.o iter.o sr.o fst.o trim.o test.o compile.o print.o
	$(CC) compile.o test.o queue.o bitset.o iter.o sr.o fst.o trim.o print.o test_trim.c -o test_trim 

test_compose: sr.o fst.o hash.o queue.o compose.o test.o compile.o print.o match.o
	$(CC) sr.o fst.o hash.o queue.o compose.o test.o match.o compile.o print.o test_compose.c -o test_compose

test_heap: heap.o 
	$(CC) heap.o test_heap.c -o test_heap

test_shortest: sr.o heap.o fst.o shortest.o print.o queue.o
	$(CC)   sr.o heap.o fst.o shortest.o print.o queue.o test_shortest.c -o test_shortest

tests: clean test_fst test_queue test_bitset test_print test_iter test_io test_compile test_sort test_hash test_stack test_trim test_compose test_heap test_shortest
	./test_fst
	./test_queue
	./test_bitset
	./test_print
	./test_iter
	./test_io
	./test_compile
	./test_sort
	./test_hash
	./test_stack
	./test_trim
	./test_heap
	./test_shortest
	./test_compose

fscompile: sr.o fst.o compile.o
	$(CC) sr.o fst.o compile.o fscompile.c -o fscompile

fsprint: sr.o fst.o print.o queue.o 
	$(CC) sr.o fst.o print.o queue.o fsprint.c -o fsprint

fsstat: sr.o fst.o 
	$(CC) sr.o fst.o fsstat.c -o fsstat

fssort: sr.o fst.o sort.o
	$(CC) sr.o fst.o sort.o fssort.c -o fssort

#fsunion: sr.o fst.o union.o
#	$(CC) sr.o fst.o union.o fsunion.c -o fsunion

fsdraw: sr.o fst.o draw.o
	$(CC) sr.o fst.o draw.o fsdraw.c -o fsdraw

fscompose: sr.o hash.o match.o queue.o fst.o compose.o trim.o bitset.o iter.o
	$(CC) sr.o hash.o match.o queue.o fst.o compose.o trim.o bitset.o iter.o fscompose.c -o fscompose

fsshort: sr.o heap.o fst.o shortest.o print.o queue.o
	$(CC)  sr.o heap.o fst.o shortest.o print.o queue.o fsshort.c -o fsshort

tools: clean fscompile fsprint fsstat fssort fsdraw fscompose fsshort
	
#tools: fsdraw fscompile fsprint fscompose fscompose2 fsstat fssort iter_fst iter_cst

fst.o:
	$(CC) -c fst.c

queue.o:
	$(CC) -c queue.c

hash.o:
	$(CC) -c hash.c

bitset.o:
	$(CC) -c bitset.c

iter.o:
	$(CC) -c iter.c

print.o:
	$(CC) -c print.c

sr.o:
	$(CC) -c sr.c

compile.o:
	$(CC) -c compile.c

test.o:
	$(CC) -c test.c

sort.o:
	$(CC) -c sort.c

union.o:
	$(CC) -c union.c

draw.o:
	$(CC) -c draw.c

trim.o:
	$(CC) -c trim.c

compose.o:
	$(CC) -c compose.c

match.o:
	$(CC) -c match.c	

heap.o:
	$(CC) -c heap.c

shortest.o:
	$(CC) -c shortest.c

clean:
	rm -f *.o
	rm -f test_fst
	rm -f test_queue
	rm -f test_bitset
	rm -f test_print
	rm -f test_iter
	rm -f test_io
	rm -f test_compile
	rm -f test_sort
	rm -f test_hash
	rm -f test_stack
	rm -f test_trim
	rm -f test_heap
	rm -f test_shortest
	rm -f test_compose

