# DSA in C: Units 3 and 4

All programs are standalone C files. Build one with `gcc -Wall file.c -o out -lm`, or run `make` inside `Unit3/` or `Unit4/` for the core programs (the `extras/` folders are built by hand). See [Theory_to_Code_Notes.md](Theory_to_Code_Notes.md) for how to go from theory to code.

"Core" files are the ones with a demo `main` that prints results. "Extra" files are menu-driven programs that cover features the core files do not.

## Unit 3: Trees, heaps and intro to graphs

| # | File | Type | Topic |
|---|------|------|-------|
| 1 | [Unit3/01_bst.c](Unit3/01_bst.c) | Core | BST: insert, search, delete, all traversals, height, LCA, floor/ceil, kth smallest, is-BST |
| 2 | [Unit3/02_tree_array.c](Unit3/02_tree_array.c) | Core | Array representation of a binary tree and of a BST |
| 3 | [Unit3/03_expression_tree.c](Unit3/03_expression_tree.c) | Core | Expression trees from infix, prefix and postfix; evaluation |
| 4 | [Unit3/04_threaded_bst.c](Unit3/04_threaded_bst.c) | Core | Double-threaded BST: insert, delete, stackless traversals |
| 5 | [Unit3/05_heap_priority_queue.c](Unit3/05_heap_priority_queue.c) | Core | Min/max heap with callback, heap sort, priority queue |
| 6 | [Unit3/06_callbacks_map_filter_reduce.c](Unit3/06_callbacks_map_filter_reduce.c) | Core | Function-pointer callbacks: map, filter, reduce |
| 7 | [Unit3/07_running_median.c](Unit3/07_running_median.c) | Core | Running median with two heaps |
| 8 | [Unit3/extras/01_bst_dynamic.c](Unit3/extras/01_bst_dynamic.c) | Extra | BST menu program: successor, predecessor, range print, BST sort |
| 9 | [Unit3/extras/02_bst_array_sequential.c](Unit3/extras/02_bst_array_sequential.c) | Extra | BST in an array, including deletion |
| 10 | [Unit3/extras/07_priority_queue_heap.c](Unit3/extras/07_priority_queue_heap.c) | Extra | Priority queue with change-priority and arrival-order ties |
| 11 | [Unit3/extras/08_avl_tree_only_theory.c](Unit3/extras/08_avl_tree_only_theory.c) | Extra | AVL tree with rotations |
| 12 | [Unit3/extras/09_splay_tree_only_theory.c](Unit3/extras/09_splay_tree_only_theory.c) | Extra | Splay tree |
| 13 | [Unit3/extras/10_graph_adjacency_matrix.c](Unit3/extras/10_graph_adjacency_matrix.c) | Extra | Graph properties and adjacency matrix |
| 14 | [Unit3/extras/11_graph_adjacency_list.c](Unit3/extras/11_graph_adjacency_list.c) | Extra | Graph properties and adjacency list |
| 15 | [Unit3/extras/12_graph_dfs.c](Unit3/extras/12_graph_dfs.c) | Extra | DFS with discovery/finish times and edge classification |
| 16 | [Unit3/extras/13_graph_bfs.c](Unit3/extras/13_graph_bfs.c) | Extra | BFS with levels and distances |

## Unit 4: Graph applications, suffix tree and hashing

| # | File | Type | Topic |
|---|------|------|-------|
| 17 | [Unit4/01_graph_adj_matrix.c](Unit4/01_graph_adj_matrix.c) | Core | Adjacency matrix: BFS, DFS, components, paths, cycle detection |
| 18 | [Unit4/02_graph_adj_list.c](Unit4/02_graph_adj_list.c) | Core | Adjacency list: BFS, DFS, components, paths, topological sort, bipartite, SCC |
| 19 | [Unit4/03_graph_weighted_algos.c](Unit4/03_graph_weighted_algos.c) | Core | Dijkstra, Prim, Kruskal, Floyd-Warshall, Bellman-Ford |
| 20 | [Unit4/04_hashing.c](Unit4/04_hashing.c) | Core | Linear, quadratic and double hashing, chaining, rehashing |
| 21 | [Unit4/05_trie.c](Unit4/05_trie.c) | Core | Trie: insert, search, delete, prefix, autocomplete |
| 22 | [Unit4/06_suffix_tree.c](Unit4/06_suffix_tree.c) | Core | Compressed suffix tree and suffix trie, pattern search |
| 23 | [Unit4/extras/15_hash_functions.c](Unit4/extras/15_hash_functions.c) | Extra | Division, multiplication, mid-square and folding hash functions |

## Not included

`03_bst_array_indexed.c` (BST with index-linked array records) is left out because the source file does not compile as supplied.
