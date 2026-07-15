from networkx import Graph
from src.graph_utils import total_weight
from src.dynam_update import attribute_depths, removed_edge


# Tries to find a different mst from the given one.
# Returns the smallest mst different from the given one.
def other_mst(graph: Graph, mst: Graph) -> Graph | None:
    best_graph = None
    previous_total = total_weight(mst)
    for edge in mst.edges():
        previous_weight = graph.edges[edge]["weight"]
        graph.remove_edge(*edge)
        mst_copy = mst.copy()
        attribute_depths(mst_copy, 1, 0)
        mst_copy = removed_edge(graph, mst_copy, edge)
        if (mst_copy is not None) and (
            (best_graph is None) or total_weight(best_graph) > total_weight(mst_copy)
        ):
            best_graph = mst_copy
            if total_weight(best_graph) == previous_total:
                return best_graph
        graph.add_edge(*edge, weight=previous_weight)
    return best_graph
