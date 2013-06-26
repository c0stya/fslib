CC=c99 -g -Wall

IPATH=/usr/local/bin

all: tests

test_fst: sr.o fst.o
	$(CC) sr.o fst.o test_fst.c -o test_fst

test_io: sr.o fst.o 
	$(CC) sr.o fst.o test_io.c -o test_io

test_print: symt.o hash.o fst.o queue.o print.o
	$(CC) symt.o hash.o fst.o queue.o print.o test_print.c -o test_print

test_queue: queue.o 
	$(CC) queue.o test_queue.c -o test_queue

test_bitset: bitset.o 
	$(CC) bitset.o test_bitset.c -o test_bitset

test_hash: hash.o 
	$(CC) hash.o test_hash.c -o test_hash

test_iter: sr.o queue.o fst.o bitset.o iter.o 
	$(CC) sr.o queue.o fst.o bitset.o iter.o test_iter.c -o test_iter

test_compile: test.o sr.o fst.o hash.o symt.o compile.o 
	$(CC) test.o sr.o fst.o hash.o symt.o compile.o test_compile.c -o test_compile

test_sort: compile.o hash.o test.o sr.o fst.o symt.o sort.o
	$(CC) test.o hash.o compile.o sr.o fst.o sort.o symt.o test_sort.c -o test_sort

test_stack: compile.o symt.o hash.o test.o sr.o fst.o 
	$(CC) test.o symt.o hash.o compile.o sr.o fst.o test_stack.c -o test_stack

test_trim: queue.o bitset.o iter.o sr.o fst.o trim.o test.o symt.o hash.o compile.o print.o
	$(CC) compile.o test.o queue.o bitset.o iter.o sr.o fst.o symt.o hash.o trim.o print.o test_trim.c -o test_trim 

test_compose: sr.o fst.o hash.o queue.o compose.o test.o symt.o compile.o print.o match.o sort.o
	$(CC) sr.o fst.o hash.o queue.o compose.o test.o match.o symt.o compile.o print.o sort.o test_compose.c -o test_compose

test_heap: heap.o 
	$(CC) heap.o test_heap.c -o test_heap

test_symt: test_symt.txt symt.o hash.o
	$(CC) symt.o hash.o test_symt.c -o test_symt

test_shortest: sr.o symt.o hash.o heap.o fst.o shortest.o print.o queue.o
	$(CC)   sr.o symt.o hash.o heap.o fst.o shortest.o print.o queue.o test_shortest.c -o test_shortest

tests: clean test_fst test_queue test_bitset test_print test_iter test_io test_compile test_sort test_hash test_stack test_trim test_compose test_heap test_shortest test_symt
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
	./test_symt

fscompile: sr.o fst.o hash.o symt.o compile.o
	$(CC) sr.o fst.o hash.o symt.o compile.o fscompile.c -o fscompile

fsprint: sr.o hash.o symt.o fst.o print.o queue.o 
	$(CC) sr.o hash.o symt.o fst.o print.o queue.o fsprint.c -o fsprint

fsextr: sr.o hash.o symt.o fst.o print.o queue.o iter.o bitset.o
	$(CC) sr.o hash.o symt.o fst.o print.o queue.o iter.o bitset.o fsextr.c -o fsextr

fsstat: sr.o fst.o 
	$(CC) sr.o fst.o fsstat.c -o fsstat

fssort: sr.o fst.o sort.o
	$(CC) sr.o fst.o sort.o fssort.c -o fssort

#fsunion: sr.o fst.o union.o
#	$(CC) sr.o fst.o union.o fsunion.c -o fsunion

fsdraw: sr.o hash.o symt.o fst.o draw.o
	$(CC) sr.o hash.o symt.o fst.o draw.o fsdraw.c -o fsdraw

fscompose: sr.o hash.o match.o queue.o fst.o compose.o trim.o bitset.o iter.o
	$(CC) sr.o hash.o match.o queue.o fst.o compose.o trim.o bitset.o iter.o fscompose.c -o fscompose

fsshort: sr.o symt.o hash.o heap.o fst.o shortest.o print.o queue.o
	$(CC)  sr.o heap.o symt.o hash.o fst.o shortest.o print.o queue.o fsshort.c -o fsshort

tools: clean fscompile fsprint fsstat fssort fsdraw fscompose fsshort fsextr
	
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

symt.o:
	$(CC) -c symt.c

test_symt.txt:
	echo "one 1\ntwo 2" > test_symt.txt

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
	rm -f test_symt.txt

install: tools
	cp ./fscompile ${IPATH}
	cp ./fscompose ${IPATH}
	cp ./fssort    ${IPATH}
	cp ./fsprint   ${IPATH}
	cp ./fsshort   ${IPATH}
	cp ./fsdraw    ${IPATH}
	cp ./fsstat    ${IPATH}
	cp ./fsextr    ${IPATH}


