import sys
import argparse as ap

from src.graph_manager import GraphManager
from src.interactive_cli_handler import InteractiveCliHandler
import src.graph_utils as gu


def main() -> None:
    parser: ap.ArgumentParser = ap.ArgumentParser(prog="Graphoni", description="Util for finding MST of graph")
    _add_arguments(parser)
    _check_for_args(parser)
    _process_args(parser)


def _add_arguments(parser: ap.ArgumentParser) -> None:
    parser.add_argument("input", nargs="?", type=str, help="input file")
    parser.add_argument("output", nargs="?", type=str, help="output file")
    parser.add_argument("-a", "--algorithm", type=str, choices=list(gu.ALGORITHMS.keys()), default="kruskal", help="choose algorithm")
    parser.add_argument("-s", "--second", action="store_true", help="find second mst")
    parser.add_argument("-i", "--interactive", action="store_true", help="run interactive mode")
    parser.add_argument("-v", "--version", action="version", version="Graphoni 1.0.0 Pavlov Aleksandr, Lardinois Joachim",  help="show version")


def _check_for_args(parser: ap.ArgumentParser) -> None:
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit()


def _process_args(parser: ap.ArgumentParser) -> None:
    graph_manager: GraphManager = GraphManager()
    args: ap.Namespace = parser.parse_args()

    if args.input is not None:
        try:
            graph_manager.load(args.input)
        except Exception as e:
            print(f"Error: {e}")
            sys.exit()

    graph_manager.find_mst(gu.ALGORITHMS[args.algorithm])

    if args.second:
        try:
            if not graph_manager.find_second_mst(): print("There is no second MST")
        except Exception as e:
            print(f"Error: {e}")
            sys.exit()

    if args.output is not None:
        try:
            graph_manager.save(args.output)
        except Exception as e:
            print(f"Error: {e}")
            sys.exit()

    if args.interactive:
        interactive_cli: InteractiveCliHandler = InteractiveCliHandler(graph_manager)
        interactive_cli.run()


if __name__ == "__main__": main()
