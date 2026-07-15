from networkx import Graph
from networkx import minimum_spanning_tree
from src import graph_io
from src.other_mst import other_mst
from src.graph_utils import total_weight


def test(graph: Graph) -> None:
    mst: Graph = minimum_spanning_tree(graph)
    expected = total_weight(mst)

    other: Graph | None =  other_mst(graph, mst)
    actual = total_weight(other) if other is not None else 0
    if expected == actual:
        print("passed")
    else:
        print("Expected ", expected, ", got", actual, sep="")


def test_other_mst() -> None:
    print("\nTesting other_mst\n")
    graph: Graph = graph_io.load("test/graph.txt")
    test(graph)
    return
