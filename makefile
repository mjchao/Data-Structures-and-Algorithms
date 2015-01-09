OBJS = main.cpp UnitTest.o ArrayListTests.o LinkedListTests.o LinkedStackTests.o LinkedQueueTests.o ArrayQueueTests.o HashMapTests.o PriorityQueueTests.o BPTreeTests.o
CC = g++
CFLAGS = -c -std=c++0x
LFLAGS = -g 

datastructs : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o datastructs

UnitTest.o : UnitTest.h UnitTest.cpp
	$(CC) $(CFLAGS) UnitTest.cpp

ArrayListTests.o : ArrayListTests.h ArrayListTests.cpp ArrayList.h
	$(CC) $(CFLAGS) ArrayListTests.cpp

LinkedListTests.o : LinkedListTests.h LinkedListTests.cpp LinkedList.h
	$(CC) $(CFLAGS) LinkedListTests.cpp

LinkedStackTests.o : LinkedStackTests.h LinkedStackTests.cpp LinkedStack.h
	$(CC) $(CFLAGS) LinkedStackTests.cpp

LinkedQueueTests.o : LinkedQueueTests.h LinkedQueueTests.cpp LinkedQueue.h
	$(CC) $(CFLAGS) LinkedQueueTests.cpp

ArrayQueueTests.o : ArrayQueueTests.h ArrayQueueTests.cpp ArrayQueue.h
	$(CC) $(CFLAGS) ArrayQueueTests.cpp

HashMapTests.o : HashMapTests.h HashMapTests.cpp HashMap.h
	$(CC) $(CFLAGS) HashMapTests.cpp

PriorityQueueTests.o : PriorityQueueTests.h PriorityQueueTests.cpp PriorityQueue.h
	$(CC) $(CFLAGS) PriorityQueueTests.cpp

BPTreeTests.o : BPTreeTests.h BPTreeTests.cpp BPTree.h
	$(CC) $(CFLAGS) BPTreeTests.cpp

clean:
	rm -r *.o *~ datastructs datastructs.dSYM

tar: 
	tar cfv datastruct.tar UnitTest.h UnitTest.cpp ArrayListTests.h ArrayListTests.cpp ArrayList.h LinkedListTest.h LinkedListTests.cpp LinkedList.h LinkedStackTests.h LinkedStackTests.cpp LinkedStack.h LinkedQueueTests.h LinkedQueueTests.cpp LinkedQueue.h ArrayQueueTests.h ArrayQueueTests.cpp ArrayQueue.h HashMapTests.h HashMapTests.cpp HashMap.h PriorityQueueTests.h PriorityQueueTests.cpp PriorityQueue.h BPTreeTests.h BPTreeTests.cpp BPTree.h
