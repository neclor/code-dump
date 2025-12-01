from networkx import Graph
from src.graph_file_io import GraphFileIO


def main() -> None:
    graph_file_io_test()


def graph_file_io_test() -> None:
    graph: Graph = GraphFileIO.load("test/test_input.txt")
    GraphFileIO.save(graph, "test/test_output.txt")


if __name__ == "__main__": main()
