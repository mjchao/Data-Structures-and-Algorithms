g++ -o datastructs -g -std=c++0x main.cpp UnitTest.cpp ArrayListTests.cpp LinkedListTests.cpp LinkedStackTests.cpp LinkedQueueTests.cpp ArrayQueueTests.cpp HashMapTests.cpp PriorityQueueTests.cpp BPTreeTests.cpp
valgrind --leak-check=full ./datastructs
