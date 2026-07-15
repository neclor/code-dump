import time
from networkx import Graph
import networkx as nx
import random
import matplotlib.pyplot as plt

from src.graph_manager import GraphManager
import src.graph_utils as gu


def main() -> None:

    start_time = time.time()

    G: Graph = Graph()
    while True:
        G = nx.erdos_renyi_graph(100, 1)
        if nx.is_connected(G):
            break
    for u, v in G.edges():
        G[u][v]['weight'] = random.randint(1, 100)

    end_time = time.time()
    execution_time = end_time - start_time
    print(f"generation: {execution_time:.4f}s")


    graph_manager: GraphManager = GraphManager()
    graph_manager._graph = G
    graph_manager._result_graph = G

    start_time = time.time()

    graph_manager.find_mst()

    end_time = time.time()
    execution_time = end_time - start_time
    print(f"kruskal: {execution_time:.4f}s")

    start_time = time.time()

    graph_manager.find_mst(gu.prim_stepwise)

    end_time = time.time()
    execution_time = end_time - start_time
    print(f"prim: {execution_time:.4f}s")

    start_time = time.time()

    graph_manager.find_mst(gu.reverse_delete_stepwise)

    end_time = time.time()
    execution_time = end_time - start_time
    print(f"reversal: {execution_time:.4f}s")


if __name__ == "__main__": main()
