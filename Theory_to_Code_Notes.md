# From Theory to C Code: Intuition Notes (Units 3 & 4)

## 0. The general method (use it for every topic)

1. **Draw the picture first.** Sketch 5-7 nodes. Every pointer is an arrow, every array a row of boxes.
2. **Name the invariant.** One sentence that must stay true after every operation (e.g. "left < node < right"). Code exists to preserve it.
3. **Pick the representation.** What struct fields make the invariant cheap to check and fix?
4. **Write the "shape" of the operation.** Almost every operation is one of:
   - *walk down* (search/insert) -> loop with a pointer
   - *visit everything* (traversal) -> recursion or explicit stack/queue
   - *fix up locally* (heapify, rotate, split edge) -> small helper
5. **Handle the three edge cases:** empty structure, one element, the operation touching the root/ends.
6. **Trace by hand** on your sketch, then run with `-fsanitize=address,undefined`.

Recursion = the structure's own definition. A tree is "a node plus two trees", so tree code is "handle node, call on children". The base case is the empty tree (`NULL`).

Stack vs queue is the whole difference between DFS and BFS: swap the container, the traversal order flips. Recursion *is* a hidden stack.

---

## 1. Binary Search Tree (`Unit3/01_bst.c`)

**Theory:** left subtree < node < right subtree.
**Intuition:** each comparison throws away half the remaining candidates, like binary search laid out as pointers.

| Theory | Code |
|---|---|
| Search: go left or right | `while (r && r->key != k) r = k < r->key ? r->left : r->right;` |
| Insert: search until you fall off, attach there | recursive `r->left = insert(r->left, k)`; returning the (possibly new) subtree root is what "attaches" it |
| Delete, 0 children | free it, return `NULL` to parent |
| Delete, 1 child | free it, return the child (parent's pointer gets spliced past it) |
| Delete, 2 children | copy inorder successor (min of right subtree) into node, then delete that successor (which has <= 1 child) |
| Inorder gives sorted order | `inorder(l); visit; inorder(r)` |

**Key trick:** `r->left = f(r->left)` pattern. The function returns the new root of the subtree, so insert/delete never need parent pointers.

**Height** is `-1` for empty so a single node has height 0. Choose a convention and be consistent.

**is_bst gotcha:** checking only `left < node < right` locally is wrong; pass down `(lo, hi)` bounds.

---

## 2. Array representation of trees (`02_tree_array.c`)

**Theory:** a complete binary tree needs no pointers; position encodes relationship.
1-indexed: parent `i/2`, left `2i`, right `2i+1`. 0-indexed: parent `(i-1)/2`, left `2i+1`, right `2i+2`.

**Intuition:** level 0 has 1 slot, level 1 has 2, level 2 has 4... the array is the tree read row by row. Left child of `i` is "skip the rest of my row, then count my position twice".

**Trade-off:** perfect for complete trees (no wasted space). A skewed tree wastes exponential space. That's why heaps use arrays and general BSTs don't.

**Code detail:** a separate `used[]` array distinguishes a stored `0` from an empty slot. Overflow check on insert (`i > MAX`) is the array version of "out of memory".

---

## 3. Expression trees (`03_expression_tree.c`)

**Theory:** leaves are operands, internal nodes are operators. Traversals give notations:
- inorder -> infix (needs parentheses), preorder -> prefix, postorder -> postfix.

**Postfix -> tree = a stack machine.**
- operand: push a leaf
- operator: pop right, pop left (order matters!), push `node(op, left, right)`
- at the end the stack holds exactly one tree.

**Infix -> postfix (shunting yard):** operands go straight to output. For an operator, first pop operators of *higher or equal* precedence (equal only if left-associative, `^` is right-assoc), then push it. `(` pushes, `)` pops until `(`. Flush at the end.

**Evaluate = postorder:** evaluate children, then apply the node's operator. Same shape as every tree recursion.

---

## 4. Threaded BST (`04_threaded_bst.c`)

**Theory:** in a BST, ~half the pointers are `NULL`. Reuse them as *threads* pointing to the inorder predecessor (left) and successor (right). Then inorder traversal needs no stack or recursion.

**Intuition:** a thread is a shortcut "where would recursion return to?". A flag (`lthread`, `rthread`) says whether a pointer is a real child or a thread.

| Idea | Code |
|---|---|
| Successor, has right child | leftmost node of right subtree |
| Successor, right is thread | just follow `right` |
| Insert as left child of `p` | new node inherits `p`'s old left (thread) as its left; its right thread points to `p`; then `p->left = new`, `p->lthread = 0` |

**Delete** is the fiddly one: after removing a node with one child, the *thread that pointed at the deleted node* (from the rightmost of its left subtree, or leftmost of its right subtree) must be redirected. Draw it before coding.

---

## 5. Heap and priority queue (`05_heap_priority_queue.c`)

**Theory:** complete binary tree + heap property (parent >= children for max-heap). Stored in an array.

**Two primitives do everything:**
- **sift up** (after insert at the end): while parent is out of order, swap with it.
- **sift down** (after replacing the root with the last element): swap with the *more extreme* child until in order.

Insert/extract are O(log n) because the tree height is log n.

**Bottom-up build is O(n), not O(n log n):** sift down from index `n/2-1` back to 0. Leaves (the second half) are already valid heaps; most nodes are near the bottom and sift very little.

**Heap sort:** build max-heap, then repeatedly swap root with the last unsorted slot, shrink heap by 1, sift down. The sorted region grows from the back.

**Callback idea:** `min` vs `max` heap differ only in one comparison. Pass `ok(parent, child)` as a function pointer and the same code serves both. This is the key abstraction.

**Priority queue** = heap where the item carries a priority and ordering uses that field.

---

## 6. Callbacks: map / filter / reduce (`06_...`)

**Theory:** parameters make *data* flexible; function pointers make *behaviour* flexible.

```c
typedef int (*map_fn)(int);          // "a function from int to int"
void mymap(const int *in, int *out, int n, map_fn f) { ... f(in[i]) ... }
```
Read the typedef inside-out: pointer to function taking `int`, returning `int`.

- map: same length, transform each
- filter: shorter output, keep when predicate is true (return the count!)
- reduce: fold everything into one value with an accumulator and an initial value

---

## 7. Running median (`07_running_median.c`)

**Intuition:** the median sits at the boundary between the smaller half and the larger half. You only need the *largest of the small half* and the *smallest of the large half*, and heaps give exactly those in O(1).

- `lo` = max-heap (root = biggest of lower half), `hi` = min-heap.
- Insert into the proper side, then **rebalance** so `size(lo) - size(hi)` is 0 or 1.
- Odd count -> root of `lo`. Even -> average of both roots.

The balance step is the invariant. Every insert is O(log n).

---

## 8. Graph representations (Unit 4)

| | Adjacency matrix | Adjacency list |
|---|---|---|
| Space | O(V^2) | O(V + E) |
| Edge test | O(1) | O(degree) |
| List neighbours | O(V) | O(degree) |
| Best for | dense graphs | sparse graphs |

Matrix: `m[u][v] = 1`; undirected means set both `[u][v]` and `[v][u]` (symmetric). List: array of linked lists, `head[u]` holds `u`'s neighbours.

**Visited array** is what separates graph traversal from tree traversal. Trees have no cycles; graphs do, so mark nodes or loop forever.

---

## 9. BFS and DFS (`01_`, `02_` in Unit4)

**BFS = queue, explores by distance (layers).**
```
enqueue s, mark s
while queue not empty:
    u = dequeue; visit u
    for each neighbour v not marked: mark v, enqueue v
```
Mark when *enqueuing*, not when dequeuing, or nodes get queued twice. Because it expands layer by layer, the first time you reach a node is via a shortest (fewest-edge) path.

**DFS = stack (or recursion), goes deep then backtracks.**
Recursive: mark, visit, recurse into each unvisited neighbour. Iterative: same with an explicit stack (push neighbours in reverse to keep order the same).

**When to use which:** shortest path in an unweighted graph -> BFS. Cycle detection, topological order, components, backtracking enumeration -> DFS.

---

## 10. Applications built on traversal

- **Connected components:** loop over all vertices; whenever one is unvisited, start a traversal and count++. One traversal paints one whole component.
- **Shortest path (BFS):** keep `parent[v]` when discovering `v`. To print the path, walk `t -> parent -> ... -> s`, then reverse.
- **All paths (DFS backtracking):** mark on entry, **unmark on exit**. The unmark is what lets other paths reuse the node. That one line is the whole idea of backtracking.
- **Cycle, undirected:** a visited neighbour that is *not your parent* means a cycle.
- **Cycle, directed:** three colours (white/grey/black). An edge to a **grey** node (still on the recursion stack) is a back edge = cycle.
- **Topological sort:** valid only for DAGs.
  - Kahn: repeatedly remove nodes with indegree 0.
  - DFS: push a node *after* all its descendants finish; reverse that order.
- **Bipartite:** BFS 2-colouring; conflict if an edge joins equal colours (equivalently, no odd cycle).
- **SCC (Kosaraju):** DFS recording finish order, transpose the graph, DFS in reverse finish order; each DFS tree is one SCC.

## 11. Weighted graphs (`03_...`)

- **Dijkstra:** greedy. Always finalise the unfinalised vertex with the smallest tentative distance; then *relax* its edges (`dist[v] = min(dist[v], dist[u] + w)`). Correct only for non-negative weights.
- **Bellman-Ford:** relax *every* edge V-1 times; a V-th improvement means a negative cycle.
- **Floyd-Warshall:** DP over "allowed intermediate vertices": `d[i][j] = min(d[i][j], d[i][k] + d[k][j])`. The `k` loop must be outermost.
- **Prim:** grow one tree; repeatedly add the cheapest edge leaving it (like Dijkstra, but key = edge weight).
- **Kruskal:** sort edges, add each if it joins two different components. **Union-find** answers "same component?" almost O(1) (path compression + union by rank).

---

## 12. Hashing (`04_hashing.c`)

**Theory:** `index = h(key) % m` gives O(1) average access; the hard part is **collisions**.

**Intuition:** a hash function is a deterministic "scramble" so keys spread evenly. Choose table size `m` prime to avoid patterns (e.g. all keys even).

| Scheme | Probe sequence | Weakness |
|---|---|---|
| Linear probing | `h, h+1, h+2...` | primary clustering (long runs grow) |
| Quadratic | `h + i^2` | secondary clustering; guaranteed to find a slot only if `m` prime and load < 0.5 |
| Double hashing | `h1 + i*h2`, `h2 = R - (k % R)` | needs `h2 != 0` and coprime with `m` (prime `m` ensures it) |
| Separate chaining | linked list per bucket | extra pointers, but load can exceed 1 |

**Deletion in open addressing needs tombstones.** If you mark a slot EMPTY, searches stop early at the hole and miss later keys on the same probe path. Mark it DELETED: search continues past it, insert may reuse it.

**Load factor** `alpha = count / m`. Performance degrades as `alpha -> 1` (open addressing) so **rehash**: allocate a bigger prime-sized table and *re-insert every key* (positions change because `m` changed, so you cannot just copy).

---

## 13. Trie (`05_trie.c`)

**Theory:** a tree where each edge is a character; the path from the root spells a prefix. A flag `end` marks a complete word.

**Intuition:** words sharing a prefix share a path, so prefix queries cost O(length of the prefix), independent of how many words are stored.

- insert: follow/create one child per character; set `end` on the last node.
- search: follow children; success only if you reach the end *and* `end == 1` ("app" is not found just because "apple" exists).
- starts-with: reaching the node is enough.
- list all words: DFS visiting children in `a..z` order gives lexicographic output for free.
- delete: unmark `end`, then free nodes **on the way back up** only while they have no children and are not an `end` of another word. Recursion returns "can I be freed?".

Memory trade-off: 26 pointers per node is wasteful; hash maps or compressed tries fix that.

---

## 14. Suffix tree (`06_suffix_tree.c`)

**Theory:** a compressed trie of *all suffixes* of a text (terminated with a unique `$`). A substring of the text is exactly a prefix of some suffix, so substring search = walk down from the root.

**Intuition for compression:** a plain suffix trie has O(n^2) nodes. Collapse every chain of single-child nodes into one edge; now there are O(n) nodes, and each edge label is stored as a `(start, end)` pair of indices into the text instead of a copy.

**Why `$`:** without it a suffix can end in the middle of an edge (e.g. `ana` inside `anana`), so it wouldn't be a leaf. `$` makes every suffix its own leaf.

**Insert a suffix:** walk matching characters along edges. If the mismatch is in the middle of an edge, **split** it: create a middle node for the shared part, hang the old remainder and the new leaf below it. That split is the only non-obvious step.

**What the tree answers:**
- contains pattern: walk it, O(|pattern|)
- occurrences: all leaves under the node where the walk ended (leaf stores its suffix index)
- longest repeated substring: deepest *internal* node (it has >= 2 leaves below)
- number of distinct substrings: sum of edge lengths (minus the `$`)

---

## 15. Common C pitfalls checklist

- `malloc` returns uninitialised memory: use `calloc` for pointer arrays (trie children, adjacency heads).
- Every `malloc` needs a matching `free` path; write the `destroy` function when you write the struct.
- Forgetting to return the updated root from recursive insert/delete.
- Off-by-one between 0-indexed and 1-indexed heap formulas. Pick one and write the three formulas at the top.
- Mark-visited timing (BFS: on enqueue).
- Negative `%` in C: `-3 % 5` is `-3`. Normalise hash indexes with `if (r < 0) r += m`.
- `static` helpers keep internal functions private to the file; the header exposes only the API.
- Compile with `-Wall -Wextra -g -fsanitize=address,undefined`. Most pointer bugs show up immediately.

## 16. Study routine that builds the intuition

1. Re-derive each operation on paper with a 5-node example **before** reading the code.
2. Close the file and rewrite the function from the invariant alone.
3. Break it on purpose (remove the unmark in `all_paths`, mark DELETED as EMPTY) and watch what fails. Seeing the bug explains the rule better than reading it.
4. Print intermediate state (`ht_print`, `g_print`) to connect pictures with memory.
