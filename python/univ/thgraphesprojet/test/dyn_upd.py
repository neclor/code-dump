from networkx import Graph
from networkx import minimum_spanning_tree
from src import graph_io
from src import dynam_update
from src.dynam_update import change_graph_and_mst


def get_total_sum(graph: Graph):
    total = 0
    for _, _, weight in graph.edges(data="weight"):
        total += weight
    return total


def print_graph(name: str, graph: Graph):
    print(
        name,
        graph.nodes(data="depth"),
        {(a, b): d["weight"] for a, b, d in graph.edges(data=True)},
        get_total_sum(graph),
    )
    return


def test(expected, actual):
    if (
        get_total_sum(expected) == get_total_sum(actual)
        and len(expected.edges()) == len(actual.edges())
        and len(expected.nodes()) == len(actual.nodes())
    ):
        print("passed")
    else:
        print_graph("expected: ", expected)
        print_graph("actual:   ", actual)


def test_(graph, mst, edge, weight, msg):
    print(edge, weight, msg)
    change_graph_and_mst(graph, mst, edge, weight)
    test(minimum_spanning_tree(graph), mst)


def test_dynamic_update():
    print("\nTesting change_graph_and_mst\n")
    dynam_update.debuging = False
    graph: Graph = graph_io.load("test/graph.txt")
    mst: Graph = minimum_spanning_tree(graph)
    test_(graph, mst, (1, 3), 100, "Increasing an edge's weight...")
    test_(graph, mst, (1, 3), 1000, "Increasing an edge's weight (with no effect)...")
    test_(graph, mst, (7, 9), None, "Removing an edge in MST...")
    test_(graph, mst, (1, 3), None, "Removing an edge out of MST...")
    test_(graph, mst, (1, 3), 1000, "Adding an edge out of MST...")
    test_(graph, mst, (7, 9), 1, "Adding an edge in MST...")
    test_(graph, mst, (1, 3), 100, "Decreasing an edge out of mst (no changes)...")
    test_(graph, mst, (1, 3), 3, "Decreasing an edge out of mst...")
    print("\nDoing everything again...\n")
    test_(graph, mst, (1, 3), 100, "Increasing an edge's weight...")
    test_(graph, mst, (1, 3), 1000, "Increasing an edge's weight (with no effect)...")
    test_(graph, mst, (7, 9), None, "Removing an edge in MST...")
    test_(graph, mst, (1, 3), None, "Removing an edge out of MST...")
    test_(graph, mst, (1, 3), 1000, "Adding an edge out of MST...")
    test_(graph, mst, (7, 9), 1, "Adding an edge in MST...")
    test_(graph, mst, (1, 3), 100, "Decreasing an edge out of mst (no changes)...")
    test_(graph, mst, (1, 3), 3, "Decreasing an edge out of mst...")
    return
