#include "Hashmap.h"
#include "Profiling.h"
#include "Random.h"
#include <iostream>
#include <unordered_map>


using namespace dsalgo;


void ProfilePut(int num_inserts, int num_runs) {
  std::vector<std::string> rand_elems = RandStrs(8, 16, num_inserts);

  int64_t start = 0;
  int64_t stop = 0;

  Hashmap<std::string, std::string> test;
  start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    for (const std::string& e : rand_elems) {
      test.Put(e, e);
    }
    test.Clear();
  }
  stop = Clock::Now();
  std::cout << "dsalgo Hashmap" << std::endl;
  PrintStats(stop - start, num_runs * num_inserts, "\t");

  std::unordered_map<std::string, std::string> test_std;
  start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    for (const std::string& e : rand_elems) {
      test_std.insert({e, e});
    }
    test_std.clear();
  }
  stop = Clock::Now();
  std::cout << "std::unordered_map" << std::endl;
  PrintStats(stop - start, num_runs * num_inserts, "\t");
}


void ProfilePutVariousSizes() {
  std::cout << "=== Profiling Hashmap Put Small Size ===" << std::endl;
  ProfilePut(10, 100000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Hashmap Put Medium Size ===" << std::endl;
  ProfilePut(1000, 1000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Hashmap Put Large Size ===" << std::endl;
  ProfilePut(100000, 10);
  std::cout << "\n\n\n";
}


void ProfileGet(int num_gets, int num_runs) {
  std::vector<std::string> rand_elems = RandStrs(8, 16, num_gets);

  int64_t start = 0;
  int64_t stop = 0;

  Hashmap<std::string, std::string> test;
  std::unordered_map<std::string, std::string> test_std;
  for (int i = 0; i < static_cast<int>(rand_elems.size()) / 2; ++i) {
    // insert some, and leave out others so that we profile both getting
    // elements that are there and that are not there
    bool should_insert = (RandInt(0, 1) == 0);
    if (should_insert) {
      test.Put(rand_elems[i], rand_elems[i]);
      test_std.insert({rand_elems[i], rand_elems[i]});
    }
  }

  start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    for (int j = 0; j < num_gets; ++j) {
      test.Get(rand_elems[j]);
    }
  }
  stop = Clock::Now();
  std::cout  << "dsalgo Hashmap" << std::endl;
  PrintStats(stop - start, num_runs * num_gets, "\t");

  start = Clock::Now();
  for (int i = 0; i < num_runs; ++i) {
    for (int j = 0; j < num_gets; ++j) {
      test_std.find(rand_elems[j]);
    }
  }
  stop = Clock::Now();
  std::cout << "std::unordered_map" << std::endl;
  PrintStats(stop - start, num_runs * num_gets, "\t");
}


void ProfileGetVariousSizes() {
  std::cout << "=== Profiling Hashmap Get Small Size ===" << std::endl;
  ProfileGet(10, 100000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Hashmap Get Medium Size ===" << std::endl;
  ProfileGet(1000, 1000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Hashmap Get Large Size ===" << std::endl;
  ProfileGet(100000, 10);
  std::cout << "\n\n\n";
}


void ProfileRemove(int num_removals, int num_runs) {
  std::vector<std::string> rand_elems = RandStrs(8, 16, num_removals);

  int64_t start = 0;
  int64_t stop = 0;
  int64_t accumulated_time = 0;

  Hashmap<std::string, std::string> test_master_copy;
  std::unordered_map<std::string, std::string> test_std_master_copy;
  for (const std::string& e : rand_elems) {
    test_master_copy.Put(e, e);
    test_std_master_copy.insert({e, e});
  }

  for (int i = 0; i < num_runs; ++i) {
    Hashmap<std::string, std::string> test = test_master_copy;
    start = Clock::Now();
    for (int j = 0; j < num_removals; ++j) {
      test.Remove(rand_elems[j]);
    }
    stop = Clock::Now();
    accumulated_time += (stop - start);
  }
  std::cout << "dsalgo Hashmap" << std::endl;
  PrintStats(accumulated_time, num_runs * num_removals, "\t");

  accumulated_time = 0;
  for (int i = 0; i < num_runs; ++i) {
    std::unordered_map<std::string, std::string> test_std =
        test_std_master_copy;
    start = Clock::Now();
    for (int j = 0; j < num_removals; ++j) {
      test_std.erase(test_std.find(rand_elems[j]));
    }
    stop = Clock::Now();
    accumulated_time += (stop - start);
  }
  std::cout << "std::unordered_map" << std::endl;
  PrintStats(accumulated_time, num_runs * num_removals, "\t");
}


void ProfileRandomized(int num_operations) {
  // random operations:
  //  0 or 1 = insert random string (50%)
  //  2 = get random string (25%)
  //  3 = remove random string (25%)
  std::vector<int> rand_ops = RandN(0, 3, num_operations);
  std::vector<std::string> target_elem; 
  for (int i = 0; i < num_operations; ++i) {
    switch (rand_ops[i]) {
    case 0:
    case 1:
      target_elem.push_back(RandStr(8, 16));
      break;
    case 2:
      if (!target_elem.empty()) {
        // pick a random element to get
        int rand_elem_idx = RandInt(0, target_elem.size() - 1);
        target_elem.push_back(target_elem[rand_elem_idx]);
      } else {
        target_elem.push_back(RandStr(8, 16));
      }
      break;
    case 3:
      if (!target_elem.empty()) {
        // pick a random element to delete - note that get/delete might be
        // operating on an element that was already deleted. That's okay.
        int rand_elem_idx = RandInt(0, target_elem.size() - 1);
        target_elem.push_back(target_elem[rand_elem_idx]);
      } else {
        target_elem.push_back(RandStr(8, 16));
      }
      break;
    default:
      // should not get here
      throw std::logic_error("Invalid op code " + std::to_string(rand_ops[i]));
      break;
    }
  }

  int64_t start = 0;
  int64_t stop = 0;

  Hashmap<std::string, std::string> test;
  start = Clock::Now(); 
  for (int i = 0; i < num_operations; ++i) {
    switch (rand_ops[i]) {
      // insert
      case 0:
      case 1:
        test.Put(target_elem[i], target_elem[i]);
        break;

      // get
      case 2:
        test.Get(target_elem[i]);
        break;

      // remove
      case 3:
        test.Remove(target_elem[i]);
        break;
      default:
        // should not get here
        throw std::logic_error("Invalid op code " +
            std::to_string(rand_ops[i]));
        break;
    }
  }
  stop = Clock::Now();
  std::cout << "dsalgo Hashmap" << std::endl;
  PrintStats(stop - start, num_operations, "\t");

  std::unordered_map<std::string, std::string> test_std;
  start = Clock::Now();
  for (int i = 0; i < num_operations; ++i) {
    switch (rand_ops[i]) {
      // insert
      case 0:
      case 1:
        test_std.insert({target_elem[i], target_elem[i]});
        break;

      // get
      case 2:
        test_std.find(target_elem[i]);
        break;

      // remove
      case 3:
        {
        auto elem_it = test_std.find(target_elem[i]);
        if (elem_it != test_std.end()) {
          test_std.erase(elem_it);
        }
        break;
        }
      default:
        // should not get here
        throw std::logic_error("Invalid op code " +
            std::to_string(rand_ops[i]));
        break;
    }
  }
  stop = Clock::Now();
  std::cout << "std::unordered_map" << std::endl;
  PrintStats(stop - start, num_operations, "\t");
}

void ProfileRemoveVariousSizes() {
  std::cout << "=== Profiling Hashmap Remove Small Size ===" << std::endl;
  ProfileRemove(10, 100000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Hashmap Remove Medium Size ===" << std::endl;
  ProfileRemove(1000, 1000);
  std::cout << "\n\n\n";

  std::cout << "=== Profiling Hashmap Remove Large Size ===" << std::endl;
  ProfileRemove(100000, 10);
  std::cout << "\n\n\n";
}


int main() {
  ReseedRand();
  ProfilePutVariousSizes();
  ProfileGetVariousSizes();
  ProfileRemoveVariousSizes();

  std::cout << "=== Profiling Hashmap Randomized Operations ===" << std::endl;
  ProfileRandomized(100000);
  return 0;
}

