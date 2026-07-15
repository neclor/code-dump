from networkx import Graph
from src.dsu import Dsu


def kruskal_stepwise(graph: Graph):
    if not _is_connected(graph):
        yield None
        return

    dsu: Dsu = Dsu(list(graph.nodes()))

    edges: list[tuple] = list(graph.edges(data="weight"))
    edges.sort(key=lambda edge: edge[2])

    result: Graph = Graph()
    for node in graph.nodes():
        result.add_node(node)
    yield result

    for u, v, weight in edges:
        if dsu.find(u) == dsu.find(v):
            continue

        result.add_edge(u, v, weight=weight)
        dsu.union(u, v)

        yield result

        if result.number_of_edges() == len(graph) - 1: return


def prim_stepwise(graph: Graph):
    if not _is_connected(graph):
        yield None
        return

    start = next(iter(graph.nodes()))
    edges: list[tuple] = list(graph.edges(start, data="weight"))

    result: Graph = Graph()
    result.add_node(start)
    yield result

    while edges:
        u, v, weight = min(edges, key=lambda edge: edge[2])
        if u == v:
            edges.remove((u, v, weight))
            continue
        new_node = u if v in result else v

        for new_edge in graph.edges(new_node, data="weight"):
            if new_edge in edges:
                edges.remove(new_edge)
            elif (new_edge[1], new_edge[0], new_edge[2]) in edges:
                edges.remove((new_edge[1], new_edge[0], new_edge[2]))
            else:
                edges.append(new_edge)

        result.add_edge(u, v, weight=weight)
        yield result

        if result.number_of_edges() == len(graph) - 1: return



def reverse_delete_stepwise(graph: Graph):
    if not _is_connected(graph):
        yield None
        return

    edges: list[tuple] = list(graph.edges(data="weight"))
    edges.sort(key=lambda edge: edge[2], reverse=True)

    result: Graph = graph.copy()
    yield result

    for u, v, weight in edges:
        result.remove_edge(u, v)
        if _is_connected(result):
            yield result
        else: result.add_edge(u, v, weight=weight)


ALGORITHMS: dict = {
    "kruskal": kruskal_stepwise,
    "prim": prim_stepwise,
    "reverse": reverse_delete_stepwise,
}


def _is_connected(graph: Graph) -> bool:
    if len(graph) <= 1: return True

    start = next(iter(graph.nodes()))

    visited: set = set()
    visited.add(start)

    stack: list = [start]
    while stack:
        v = stack.pop()
        for n in graph.neighbors(v):
            if n not in visited:
                stack.append(n)
                visited.add(n)

    return len(visited) == len(graph)


def _has_cycle(graph: Graph) -> bool:
    if len(graph) <= 1: return False

    visited: set = set()
    for node in graph.nodes():
        if node in visited: continue
        visited.add(node)

        stack: list[tuple] = [(node, None)]
        while stack:
            v, p = stack.pop()
            for n in graph.neighbors(v):
                if n not in visited:
                    stack.append((n, v))
                    visited.add(n)
                elif n != p:
                    return True

    return False


def total_weight(graph: Graph) -> int:
    return sum(data["weight"] for _, _, data in graph.edges(data=True))

def
