import matplotlib.pyplot as plt
import networkx as nx
from networkx import Graph
import src.graph_io as gio
import src.graph_utils as gu
import src.other_mst as om
import src.dynam_update as du


class GraphManager:
    def __init__(self) -> None:
        self._graph: Graph = Graph()
        self._result_graph: Graph = self._graph
        self._result_is_mst: bool = False
        self._pos = None


    def load(self, path: str, show: bool = False) -> None:
        self._graph = gio.load(path)
        self._result_graph = self._graph
        self._result_is_mst = False
        self._pos = None
        if show: self._show_graph(self._result_graph)


    def save(self, path: str) -> None:
        gio.save(self._result_graph, path)


    def add_node(self, u: int) -> None:
        self._graph.add_node(u)
        self._result_graph = self._graph
        self._result_is_mst = False
        self._update_pos()
        self._show_graph(self._result_graph)


    def add_edge(self, u: int, v: int, weight: int) -> None:
        self._graph.add_edge(u, v, weight=weight)
        self._result_graph = self._graph
        self._result_is_mst = False
        self._update_pos()
        self._show_graph(self._result_graph)


    def remove_node(self, u: int) -> None:
        self._graph.remove_node(u)
        self._result_graph = self._graph
        self._result_is_mst = False
        self._update_pos()
        self._show_graph(self._result_graph)


    def remove_edge(self, u: int, v: int) -> None:
        self._graph.remove_edge(u ,v)
        self._result_graph = self._graph
        self._result_is_mst = False
        self._update_pos()
        self._show_graph(self._result_graph)


    def find_mst(self, algorithm = gu.kruskal_stepwise, stepwise: bool = False, show: bool = False) -> bool:
        for mst in algorithm(self._graph):
            if mst is None: return False
            self._result_graph = mst

            if not stepwise: continue
            if show: self._draw_graph(self._graph)
            if show: self._show_graph(mst, "orange", False)
            input()

        self._result_is_mst = True
        if show: self._show_graph(self._result_graph)

        return True


    def update(self, u: int, v: int, weight: int | None, show: bool = False) -> None:
        if not self._result_is_mst: self.find_mst()

        new_graph, new_mst = du.change_graph_and_mst(self._graph.copy(), self._result_graph.copy(), (u, v), weight)

        self._graph = new_graph
        self._result_graph = new_mst
        self._result_is_mst = True

        if show: self._show_graph(self._result_graph)


    def find_second_mst(self, show: bool = False) -> bool:
        if not self._result_is_mst: self.find_mst()

        second_mst: Graph | None = om.other_mst(self._graph, self._result_graph)
        self._result_is_mst = False

        if second_mst is None:
            self._result_graph = Graph()
            return False

        self._result_graph = second_mst

        if show: self._show_graph(self._result_graph)

        return True


    # Replaces the original graph with the result for subsequent work
    def keep(self) -> None:
        self._graph = self._result_graph
        self._update_pos()


    def show_origin_graph(self) -> None:
        self._result_graph = self._graph
        self._result_is_mst = False
        self._show_graph(self._result_graph)


    # Repositions the graph
    def repos(self) -> None:
        self._pos = nx.spring_layout(self._graph)
        self._show_graph(self._result_graph)


    def _update_pos(self) -> None:
        if self._pos is None:
            self._pos = nx.spring_layout(self._graph)
            return

        nodes: set[int] = set(self._graph.nodes())
        existing_pos: dict = {n: p for n, p in self._pos.items() if n in nodes}
        self._pos = nx.spring_layout(self._graph, pos=existing_pos, fixed=list(existing_pos.keys()))


    def _show_graph(self, graph: Graph, color: str = "#1f78b4", clear: bool = True) -> None:
        if self._pos is None:
            self._pos = nx.spring_layout(self._graph)

        if clear: plt.clf()

        self._draw_graph(graph, color)

        plt.show(block=False)
        plt.pause(0.1)


    def _draw_graph(self, graph: Graph, color: str = "#1f78b4") -> None:
        if self._pos is None: self._pos = nx.spring_layout(self._graph)

        nx.draw(graph, self._pos, with_labels=True, node_color=color, edge_color=color)
        edge_labels = nx.get_edge_attributes(graph, "weight")
        nx.draw_networkx_edge_labels(graph, self._pos, edge_labels)
