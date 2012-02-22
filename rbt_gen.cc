#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <stack>
#include <vector>
using namespace std;

typedef int Item;
typedef int Info;

class Node {
  int serialgen() {
    static int g_serial = 0;
    return g_serial++;
  }

 public:
  Node(Item k, Info i, bool r, Node *ll, Node *rr) {
    key = k; info = i; red = r;
    left = ll; right = rr;
    serial = serialgen();
    is_nil = false;
  }
  ~Node() {
    if (left && !left->is_nil) delete left;
    if (right && !right->is_nil) delete right;
  }

  static Node *nilNode() {
    Node *node = new Node(0, 0, false, NULL, NULL);
    node->left = node->right = node;
    node->is_nil = true;
    return node;
  }

  Item key; Info info;
  bool red;
  Node *left, *right;
  int serial;
  bool is_nil;
};

class Tree {
 public:
  Tree() {
    z = Node::nilNode();
    head = new Node(-1, 0, false, NULL, z);
  }
  ~Tree() {
    delete head;
    delete z;
  }

  // Info search(Item k) {}
  void insert(Item k, Info i) {
    x = p = g = head;
    while (x != z) {
      gg = g; g = p; p = x;
      x = (k < x->key) ? x->left : x->right;
      if (x->left->red && x->right->red) split(k);
    }
    x = new Node(k, i, true, z, z);
    if (k < p->key) p->left = x;
    else p->right = x;
    split(k); head->right->red = false;
  }

  Node *rotate(Item k, Node *y) {
    Node *c, *gc;
    c = (k < y->key) ? y->left : y->right;
    if (k < c->key) {
      gc = c->left; c->left = gc->right; gc->right = c;
    } else {
      gc = c->right; c->right = gc->left; gc->left = c;
    }
    if (k < y->key) y->left = gc;
    else y->right = gc;
    return gc;
  }

  void split(Item k) {
    x->red = true; x->left->red = x->right->red = false;
    if (p->red) {
      g->red = true;
      if (k < g->key != k < p->key) p = rotate(k, g);
      x = rotate(k, gg);
      x->red = false;
    }
  }

  void dot(const char *prefix, int file_id) {
    // "PREFIX_%03d.dot"
    int name_size = strlen(prefix) + 4 + 1;
    char name[name_size], name_dot[name_size + 4], name_gif[name_size + 4];
    snprintf(name, name_size, "%s_%03d", prefix, file_id);
    snprintf(name_dot, name_size + 4, "%s.dot", name);
    snprintf(name_gif, name_size + 4, "%s.gif", name);

    FILE *fp = fopen(name_dot, "w");
    if (!fp) return;

    fputs("graph sample {\n", fp);
    fputs("  graph [ center = \"false\", ordering = out, ranksep = 0.2, nodesep = 0.5 ];\n", fp);
    fputs("  node [ fontname = \"Courier\", fontsize = 14, shape = circle, width = 0.2, height = 0.2, margin = 0.01 ];\n", fp);
    fputs("  edge [ color = black, weight = 1 ];\n", fp);

    stack<Node*> st;
    Node *n = head->right; // ignore the head (-1)
    while ((n && n != z) || !st.empty()) {
      if (n && n != z) {
        // if (n->key >= 0) {
        fprintf(fp, "  N%d [ label = \"%d\" ]\n", n->serial, n->key);

        if (n->left && n->left != z) {
          if (n->left->red)
            fprintf(fp, "  N%d -- N%d [ style = bold, color = red ];\n", n->serial, n->left->serial);
          else
            fprintf(fp, "  N%d -- N%d;\n", n->serial, n->left->serial);
        } else {
          fprintf(fp, "  L%d [ shape = box, width = 0.1, height = 0.1, label = \"\" ];\n", n->serial);
          fprintf(fp, "  N%d -- L%d;\n", n->serial, n->serial);
        }
        st.push(n);
        n = n->left; // if (n == z) n = NULL;
      } else {
        n = st.top(); st.pop();
        if (n->right && n->right != z) {
          if (n->right->red)
            fprintf(fp, "  N%d -- N%d [ style = bold, color = red ];\n", n->serial, n->right->serial);
          else
            fprintf(fp, "  N%d -- N%d;\n", n->serial, n->right->serial);
        } else {
          fprintf(fp, "  R%d [ shape = box, width = 0.1, height = 0.1, label = \"\" ];\n", n->serial);
          fprintf(fp, "  N%d -- R%d;\n", n->serial, n->serial);
        }
        n = n->right;
      }
    }
    fputs("}\n", fp);

    fclose(fp);
    /*
    char cmd[100];
    snprintf(cmd, 100, "dot -Tgif %s -o %s", name_dot, name_gif);
    system(cmd);
    */
  }

  Node *head, *z;
  Node *x, *p, *g, *gg;
};

int main() {
  Tree tree;

  int file_id = 0;

  srand(clock());

  tree.dot("rbt", file_id++);

  for (int i=0; i<100; i++) {
    //int val = 'A' + rand() % 26;
    //int val = 'A' + i % 26;
    tree.insert(i, i);
    tree.dot("rbt", file_id++);
  }

  return 0;
}
