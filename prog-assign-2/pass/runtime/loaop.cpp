#include "loaop.hpp"
#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

struct Node {
  int pos;
  int size;
  Node(int _pos, int _size) : pos(_pos), size(_size) {}
  bool operator<(const Node &rhs) const {
    return tie(pos, size) < tie(rhs.pos, rhs.size);
  }
};

vector<Node> memorys;

bool isPossibleInsert(bool isStart, int pos) {
  auto it = lower_bound(memorys.begin(), memorys.end(), Node(pos, 0));
  if (it->pos == pos) {
    return false;
  } else {
    if (it == memorys.begin()) {
      return true;
    } else {
      auto pv = prev(it);
      if (pv->pos + pv->size > pos) {
        return false;
      } else {
        return true;
      }
    }
  }
}

bool insert(int pos, int size) {
  auto it = lower_bound(memorys.begin(), memorys.end(), Node(pos, 0));
  if (isPossibleInsert(true, pos) &&
      isPossibleInsert(false, pos + size - 1)) { // todo: -1을 사이즈로 수정
    cout << "suc" << endl;
    memorys.insert(it, Node(pos, size));
    return true;
  } else {
    cout << "error: " << pos << " " << size << endl;
    return false;
  }
}

bool isAccessible(int pos, int size) {
  auto it = lower_bound(memorys.begin(), memorys.end(), Node(pos, 0));
  if (it->pos == pos) {
    if (it->size < size) {
      return false;
    } else {
      return true;
    }
  } else if (it == memorys.begin()) {
    return false;
  } else {
    auto pv = prev(it);
    if (pv->pos + pv->size < pos + size) {
      return false;
    } else {
      return true;
    }
  }
}

bool erase(int pos) {
  auto it = lower_bound(memorys.begin(), memorys.end(), Node(pos, 0));
  if (it != memorys.end() && it->pos == pos) {
    memorys.erase(it);
    return true;
  } else {
    return false;
  }
}


// TODO: Change function prototype if necessary.
int logop(int type, int pos, int size) {
  cout << type << " pos: " << pos << "size: " << size << endl;
  // TODO: Produce warning if the integer overflow is found.
  return 0;
}
