# DSA in C: Units 3 and 4

All programs are standalone C files. Build one with `gcc -Wall file.c -o out -lm`, or run `make` inside `Unit3/` or `Unit4/` to build every program in that folder. See [Theory_to_Code_Notes.md](Theory_to_Code_Notes.md) for how to go from theory to code.

Files are numbered per unit. **Demo** programs run a fixed example and print the results. **Menu** programs are interactive and cover features the demos do not.

## Unit 3: Trees, heaps and intro to graphs

| No. | File | Type | Topic |
|---|------|------|-------|
| 01 | [Unit3/01_bst.c](Unit3/01_bst.c) | Demo | BST: insert, search, delete, all traversals, height, LCA, floor/ceil, kth smallest, is-BST |
| 02 | [Unit3/02_tree_array.c](Unit3/02_tree_array.c) | Demo | Array representation of a binary tree and of a BST |
| 03 | [Unit3/03_expression_tree.c](Unit3/03_expression_tree.c) | Demo | Expression trees from infix, prefix and postfix; evaluation |
| 04 | [Unit3/04_threaded_bst.c](Unit3/04_threaded_bst.c) | Demo | Double-threaded BST: insert, delete, stackless traversals |
| 05 | [Unit3/05_heap_priority_queue.c](Unit3/05_heap_priority_queue.c) | Demo | Min/max heap with callback, heap sort, priority queue |
| 06 | [Unit3/06_callbacks_map_filter_reduce.c](Unit3/06_callbacks_map_filter_reduce.c) | Demo | Function-pointer callbacks: map, filter, reduce |
| 07 | [Unit3/07_running_median.c](Unit3/07_running_median.c) | Demo | Running median with two heaps |
| 08 | [Unit3/08_bst_dynamic_menu.c](Unit3/08_bst_dynamic_menu.c) | Menu | BST menu program: successor, predecessor, range print, BST sort |
| 09 | [Unit3/09_bst_array_sequential_menu.c](Unit3/09_bst_array_sequential_menu.c) | Menu | BST in an array, including deletion |
| 10 | [Unit3/10_priority_queue_menu.c](Unit3/10_priority_queue_menu.c) | Menu | Priority queue with change-priority and arrival-order ties |
| 11 | [Unit3/11_avl_tree.c](Unit3/11_avl_tree.c) | Menu | AVL tree with rotations |
| 12 | [Unit3/12_splay_tree.c](Unit3/12_splay_tree.c) | Menu | Splay tree |
| 13 | [Unit3/13_graph_adjacency_matrix.c](Unit3/13_graph_adjacency_matrix.c) | Menu | Graph properties and adjacency matrix |
| 14 | [Unit3/14_graph_adjacency_list.c](Unit3/14_graph_adjacency_list.c) | Menu | Graph properties and adjacency list |
| 15 | [Unit3/15_graph_dfs.c](Unit3/15_graph_dfs.c) | Menu | DFS with discovery/finish times and edge classification |
| 16 | [Unit3/16_graph_bfs.c](Unit3/16_graph_bfs.c) | Menu | BFS with levels and distances |
| 17 | [Unit3/17_bst_array_indexed.c](Unit3/17_bst_array_indexed.c) | Demo | BST in an array of records with integer child indices: node/leaf counts, copy, equality |

## Unit 4: Graph applications, suffix tree and hashing

| No. | File | Type | Topic |
|---|------|------|-------|
| 01 | [Unit4/01_graph_adj_matrix.c](Unit4/01_graph_adj_matrix.c) | Demo | Adjacency matrix: BFS, DFS, components, paths, cycle detection |
| 02 | [Unit4/02_graph_adj_list.c](Unit4/02_graph_adj_list.c) | Demo | Adjacency list: BFS, DFS, components, paths, topological sort, bipartite, SCC |
| 03 | [Unit4/03_graph_weighted_algos.c](Unit4/03_graph_weighted_algos.c) | Demo | Dijkstra, Prim, Kruskal, Floyd-Warshall, Bellman-Ford |
| 04 | [Unit4/04_hashing.c](Unit4/04_hashing.c) | Demo | Linear, quadratic and double hashing, chaining, rehashing |
| 05 | [Unit4/05_trie.c](Unit4/05_trie.c) | Demo | Trie: insert, search, delete, prefix, autocomplete |
| 06 | [Unit4/06_suffix_tree.c](Unit4/06_suffix_tree.c) | Demo | Compressed suffix tree and suffix trie, pattern search |
| 07 | [Unit4/07_hash_functions.c](Unit4/07_hash_functions.c) | Menu | Division, multiplication, mid-square and folding hash functions |
