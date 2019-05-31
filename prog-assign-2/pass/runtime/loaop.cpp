#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

struct Node {
  long long pos;
  long long size;
  Node(long long _pos, long long _size) : pos(_pos), size(_size) {}
  bool operator<(const Node &rhs) const {
    return tie(pos, size) < tie(rhs.pos, rhs.size);
  }
};

vector<Node> memorys;
extern "C" {

bool isPossibleInsert(bool isStart, long long pos) {
  if (memorys.empty()) {
    return true;
  }
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

bool insert(long long pos, long long size) {
  auto it = lower_bound(memorys.begin(), memorys.end(), Node(pos, 0));
  if (isPossibleInsert(true, pos) &&
      isPossibleInsert(false, pos + size - 1)) { // todo: -1을 사이즈로 수정
    memorys.insert(it, Node(pos, size));
    return true;
  } else {
    return false;
  }
}

bool isAccessible(long long pos, long long size) {
  if (memorys.empty()) {
    return false;
  }
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

bool erase(long long pos) {
  auto it = lower_bound(memorys.begin(), memorys.end(), Node(pos, 0));
  if (it != memorys.end() && it->pos == pos) {
    memorys.erase(it);
    return true;
  } else {
    return false;
  }
}

// TODO: Change function prototype if necessary.
int logop(void *type, int pos, int size) {
  cout << hex << type << dec << " pos: " << pos << "size: " << size << endl;
  // TODO: Produce warning if the integer overflow is found.
  return 0;
}

void checkMemory(int lineNum, int type, void *memory, long long size) {
  bool isSafe = false;
  long long pointer = reinterpret_cast<long long>(memory);
  size /= 8;
  /*
  cout << hex << type << " " << pointer << " " << dec << size << endl;
  return;
  */
  switch(type) {
    // insert
    case 1:
      isSafe = insert(pointer, size);
      break;

    // acess
    case 2:
      isSafe = isAccessible(pointer, size);
      break;

    // erase
    case 3:
      break;

    default:
      break;
  }
  if (!isSafe) {
    cout << lineNum << "\terror" << endl;
    exit(-1);
  }
}
}
