from networkx import Graph


debuging = False


# Takes as inputs a graph, its mst, and what to change, then applies that
# change to the graph, and updates the given mst to match the new graph.
#
# WARNING: Modifies the given graph and mst. if you don't want that, give
# copies of your graphs to the function, and get its return graphs,
# ex: `new_g, new_mst = change_graph_and_mst(g.copy(), mst.copy(), (1,2,3.0))`
#
# WARNING: This function assumes every edge has a "weight" attribute.
#
# This function also returns the graph and its mst, for easier chaining if you
# used copies for the arguments.
#
# If mst didn't already have depth values for its nodes, attributes them.
def change_graph_and_mst(graph: Graph, mst: Graph, edge: tuple[int, int], new_weight: int | None) -> tuple[Graph, Graph]:
    global debuging

    if mst.nodes[1].get("depth") is None:
        attribute_depths(mst, 1, 0)

    if type(new_weight) is int:
        previous_data = graph.get_edge_data(edge[0], edge[1])

        # If the edge isn't in the graph, returns None.
        if previous_data is None:
            graph.add_edge(*edge, weight=new_weight)
            new_edge(graph, mst, edge, new_weight)

        elif new_weight > previous_data["weight"]:
                graph.add_edge(*edge, weight=new_weight)
                increased_weight(graph, mst, edge)

        else:
            graph.add_edge(*edge, weight=new_weight)
            decreased_weight(graph, mst, edge, new_weight)

    else:
        graph.remove_edge(*edge)
        removed_edge(graph, mst, edge)

    if debuging:
        print("Done")

    return (graph, mst)


# Modifies `mst` to match the MST of `graph`, which was modified by increasing `edge`'s weight.
#
# WARNING: Modifies `mst` (but not graph)
#
# Assumptions:
# - mst is the MST of graph before it was modified
# - Each node in mst has its depth in its data
#
# The depth of a node in this case is its distance to an arbitrary root node in
# the tree, as long as it's the same root for all depths in the tree.
def increased_weight(graph: Graph, mst: Graph, edge: tuple[int, int]):
    removed_edge(graph, mst, edge)


# Modifies `mst` to match the MST of `graph`, which was modified by decreasing `edge`'s weight.
#
# WARNING: Modifies `mst` (but not graph)
#
# Assumptions:
# - mst is the MST of graph before it was modified
# - Each node in mst has its depth in its data
#
# The depth of a node in this case is its distance to an arbitrary root node in
# the tree, as long as it's the same root for all depths in the tree.
def decreased_weight(graph: Graph, mst: Graph, edge: tuple[int, int], new_weight: int):
    my_print("Decreased")
    if edge in mst.edges():
        mst.add_edge(*edge, weight=new_weight)
        my_print("In MST, not touching")
        return
    new_edge(graph, mst, edge, new_weight)


# Modifies `mst` to match the MST of `graph`, which was modified by adding `edge`.
#
# WARNING: Modifies `mst` (but not graph)
#
# Assumptions:
# - mst is the MST of graph - edge
# - Each node in mst has its depth in its data
#
# The depth of a node in this case is its distance to an arbitrary root node in
# the tree, as long as it's the same root for all depths in the tree.
def new_edge(graph: Graph, mst: Graph, edge: tuple[int, int], new_weight: int):
    my_print("New Edge")
    biggest_edge: tuple[int, int] = edge
    biggest_weight: int | None = None

    for a, b in get_cycle_by_new_edge(mst, edge):
        ab_weight = graph.get_edge_data(a, b)["weight"]
        if (biggest_weight is None) or ab_weight > biggest_weight:
            biggest_edge = (a, b)
            biggest_weight = ab_weight

    mst.add_edge(edge[0], edge[1], weight=new_weight)
    mst.remove_edge(*biggest_edge)

    for node in mst.nodes:
        del mst.nodes[node]["depth"]

    attribute_depths(mst, 1, 0)


# Modifies `mst` to match the MST of `graph`, which was modified by removing `edge`.
#
# WARNING: Modifies `mst` (but not graph)
#
# Assumptions:
# - mst is the MST of graph + edge
# - Each node in mst has its depth in its data
#
# The depth of a node in this case is its distance to an arbitrary root node in
# the tree, as long as it's the same root for all depths in the tree.
def removed_edge(graph: Graph, mst: Graph, edge: tuple[int, int]) -> None | Graph:
    if edge not in mst.edges():
        my_print("Not in MST, not touching.")
        return None
    if get_depth(mst, edge[0]) > get_depth(mst, edge[1]):
        sub_root = edge[0]
    else:
        sub_root = edge[1]
    mst.remove_edge(*edge)
    smallest_edge: tuple[int, int] = edge
    smallest_weight: int | None = None
    edges_between = edges_between_subtrees_reset_depth(graph, mst, sub_root)
    if len(edges_between) == 0:
        return None
    for a, b in edges_between:
        ab_weight = graph.get_edge_data(a, b)["weight"]
        if (smallest_weight is None) or ab_weight < smallest_weight:
            smallest_edge = (a, b)
            smallest_weight = ab_weight
    my_print("Found smallest edge")
    attribute_depths(mst, smallest_edge[0], mst.nodes[smallest_edge[1]]["depth"] + 1)
    my_print("Reatributed depths")
    mst.add_edge(*smallest_edge, weight=smallest_weight)
    return mst


# Returns all the edges in the cycle formed by the addition of `add_edge` in the tree.
def get_cycle_by_new_edge(tree: Graph, added_edge: tuple[int, int]) -> list[tuple[int, int]]:
    edges = [added_edge]
    if get_depth(tree, added_edge[0]) > get_depth(tree, added_edge[1]):
        deep, shallow = added_edge
    else:
        shallow, deep = added_edge
    edges = [(deep, shallow)]

    while get_depth(tree, deep) > get_depth(tree, shallow):
        higher = go_up_tree(tree, deep)
        edges.append((deep, higher))
        deep = higher

    while deep != shallow:
        deep_ = go_up_tree(tree, deep)
        shallow_ = go_up_tree(tree, shallow)
        edges.append((deep, deep_))
        edges.append((shallow, shallow_))
        deep, shallow = deep_, shallow_

    return edges


def go_up_tree(tree: Graph, node: int) -> int:
    for neigh in tree.neighbors(node):
        if get_depth(tree, neigh) < get_depth(tree, node): return neigh
    return node


# Returns all edges in a graph linking 2 trees covering it, and removes all of
# the sub-tree's depth values, to be replaced when reconnecting them.
#
# for all edges (a,b) returned, a is from the sub-tree, b is from elsewhere.
#
# Assumptions:
# - `trees` contains 2 trees, covering all of `graph` with both of them
# - `sub_root` is a root of a tree in `trees` (it has the lowest depth of its
# respective tree)
def edges_between_subtrees_reset_depth(graph: Graph, trees: Graph, sub_root: int):
    edges: list[tuple[int, int]] = []
    sub_tree = deeper_nodes(trees, sub_root)
    for node in sub_tree:
        for neigh in graph.neighbors(node):
            if neigh not in sub_tree:
                edges.append((node, neigh))
        del trees.nodes[node]["depth"]
    return edges


# Returns all nodes deeper in the tree, based on their depth attribute.
def deeper_nodes(tree: Graph, node: int) -> list[int]:
    deeper_neighbours: list[int] = []
    depth = tree.nodes[node]["depth"]
    for neighbour in tree.neighbors(node):
        if tree.nodes[neighbour]["depth"] > depth:
            deeper_neighbours.append(neighbour)
    all_tree: list[int] = [node]  # `all_tree` as in all sub-tree
    for neigh in deeper_neighbours:
        for deeper in deeper_nodes(tree, neigh):
            all_tree.append(deeper)
    return all_tree


def get_depth(tree: Graph, node: int) -> int:
    return tree.nodes[node]["depth"]


def my_print(msg: str):
    global debuging
    if debuging: print(msg, end=", ")


# Reatributes the depth values to a sub-tree in the graph.
# Only rewrites the depth of nodes with no depth attribute.
def attribute_depths(tree: Graph, root: int, init_depth: int):
    to_visit = [root]
    depth = init_depth
    while len(to_visit) > 0:
        for node in to_visit:
            tree.add_node(node, depth=depth)
        new_to_visit = []
        for node in to_visit:
            for neigh in tree.neighbors(node):
                if tree.nodes[neigh].get("depth") is None:
                    new_to_visit.append(neigh)
        to_visit = new_to_visit
        depth += 1
