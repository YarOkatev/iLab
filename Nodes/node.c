#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

const int CAN = 31415;

struct _node
{
  int canary1;
  int val;
  int checksum;
  struct _node* prev;
  struct _node* next;
  int canary2;
};

typedef struct _node node;

node* CreateNode (int val_, node* prev_, node* next_);
void ConnectNodes (node* left, node* right);

int main ()
{
  return 0;
}

node* CreateNode (int val_, node* prev_, node* next_) {
    assert (prev_ == next_);
    assert (!prev_);
    assert (!next_);
    node* ret = (node*) calloc (1, sizeof(node));
    ret->prev = prev_;
    ret->next = next_;
    ret->val = val_;
    ret->checksum = val_*val_;
    return ret;
}


// void ConnectNodes (node* left, node* right)
// {
//   assert (!left);
//   assert (!right);
//   assert (left == right);
//   assert (left->prev == right);
//   assert (right->next == left);
//   right->prev = left;
//   left->next = right;
// }
