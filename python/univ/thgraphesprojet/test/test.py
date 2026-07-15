from networkx import Graph
from src import graph_io as gio
from dyn_upd import test_dynamic_update
from other import test_other_mst


def main() -> None:
    test_dynamic_update()
    test_other_mst()
    # graph_file_io_test()


def graph_file_io_test() -> None:
    graph: Graph = gio.load("test/test_input.txt")
    gio.save(graph, "test/test_output.txt")


if __name__ == "__main__": main()
