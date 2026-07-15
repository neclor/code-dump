import sys
from src.graph_manager import GraphManager
import src.graph_utils as gu


class InteractiveCliHandler:
    def __init__(self, graph_manager: GraphManager) -> None:
        self._commands: dict = {
            "load": self._load,
            "save": self._save,
            "add": self._add,
            "remove": self._remove,
            "mst": self._mst,
            "update": self._update,
            "second": self._second,
            "keep": self._keep,
            "origin": self._origin,
            "repos": self._repos,
            "help": self._show_help,
            "exit": self._exit,
        }
        self.graph_manager = graph_manager

        self._show_logo()
        self._show_help()


    def run(self) -> None:
        while True:
            self._try_execute(self._read_input())


    def _read_input(self) -> list[str]:
        while True:
            args: list[str] = []
            try:
                args = input("> ").strip().split()
            except KeyboardInterrupt:
                self._exit()
            if not args: continue
            return args


    def _try_execute(self, args: list[str]) -> None:
        cmd: str = args[0].lower()
        if cmd not in self._commands:
            print(f"Error: Command '{cmd}' doesn't exist.")
            return
        method = self._commands[cmd]
        try:
            method(args)
        except Exception as e:
            print(f"Error '{cmd}': {e}")


    def _load(self, args: list[str]) -> None:
        if len(args) != 2: raise ValueError("Usage: load <path>")
        self.graph_manager.load(args[1], True)


    def _save(self, args: list[str]) -> None:
        if len(args) != 2: raise ValueError("Usage: save <path>")
        self.graph_manager.save(args[1])


    def _add(self, args: list[str]) -> None:
        match len(args):
            case 2:
                self._add_node(args)
            case 4:
                self._add_edge(args)
            case _:
                raise ValueError("Usage: add <u> (<v> <weight>)")


    def _add_node(self, args: list[str]) -> None:
        try:
            u: int = int(args[1])
        except ValueError as e:
            raise ValueError(f"Invalid argument: {e}")
        self.graph_manager.add_node(u)


    def _add_edge(self, args: list[str]) -> None:
        try:
            u, v, weight = map(int, args[1:4])
        except ValueError as e:
            raise ValueError(f"Invalid argument: {e}")
        self.graph_manager.add_edge(u, v, weight)


    def _remove(self, args: list[str]) -> None:
        match len(args):
            case 2:
                self._remove_node(args)
            case 3:
                self._remove_edge(args)
            case _:
                raise ValueError("Usage: add <u> (<v>)")


    def _remove_node(self, args: list[str]) -> None:
        try:
            u: int = int(args[1])
        except ValueError as e:
            raise ValueError(f"Invalid argument: {e}")
        self.graph_manager.remove_node(u)


    def _remove_edge(self, args: list[str]) -> None:
        try:
            u, v = map(int, args[1:3])
        except ValueError as e:
            raise ValueError(f"Invalid argument: {e}")
        self.graph_manager.remove_edge(u, v)

    # Searches for a minimum spanning tree
    def _mst(self, args: list[str]) -> None:
        if not (0 < len(args) <= 3): raise ValueError("Usage: mst (<algorithm> -s)")

        stepwise: bool = "-s" in args
        if stepwise: args.remove("-s")

        if len(args) > 2: raise ValueError("Usage: mst (<algorithm> -s)")

        algorithm = gu.ALGORITHMS["kruskal"]
        if len(args) == 2:
            algorithm = gu.ALGORITHMS.get(args[1].lower(), None)
            if algorithm is None: raise ValueError(f"Algorithm '{args[1]}' doesn't exist")

        if self.graph_manager.find_mst(algorithm, stepwise, True):
            print("MST found")
        else:
            print("Impossible to find MST")


    def _update(self, args: list[str]) -> None:
        if not (3 <= len(args) <= 4): raise ValueError("Usage: update <u> <v> (<weight>)")

        try:
            u, v = map(int, args[1:3])
            weight: int | None = None
            if len(args) == 4:
                weight = int(args[3])
        except ValueError as e:
            raise ValueError(f"Invalid argument: {e}")

        self.graph_manager.update(u, v, weight, True)
        print("Graph updated")


    def _second(self, args: list[str]) -> None:
        if len(args) > 1: raise ValueError("Usage: second")

        if self.graph_manager.find_second_mst(True):
            print("Second MST found")
        else:
            print("There is no second MST")


    # Replaces the original graph with the result for subsequent work
    def _keep(self, args: list[str] = []) -> None:
        if len(args) > 1: raise ValueError("Usage: keep")
        self.graph_manager.keep()


    # Shows the original graph
    def _origin(self, args: list[str] = []) -> None:
        if len(args) > 1: raise ValueError("Usage: origin")
        self.graph_manager.show_origin_graph()


    # Repositions the graph
    def _repos(self, args: list[str] = []) -> None:
        if len(args) > 1: raise ValueError("Usage: repos")
        self.graph_manager.repos()


    def _show_help(self, args: list[str] = []) -> None:
        if len(args) > 1: raise ValueError("Usage: help")
        print(f"""Commands:
    load <path>                - Load a graph from specified file
    save <path>                - Save the result graph to specified file
    add <u> (<v> <weight>)     - Add node or edge to a graph
    remove <u> (<v>)           - Remove node or edge from a graph
    mst (<algorithm> -s)       - Finds minimum spanning tree of a graph
                                   <algorithm> {list(gu.ALGORITHMS.keys())} Uses the specified algorithm (kruskal by default)
                                   -s     Finds MST step by step
    update <u> <v> (<weight>)  - Updates graph and MST by adding or removing edge
    second                     - Finds second MST
    keep                       - Replaces the original graph with the result graph for subsequent work
    origin                     - Show original graph
    repos                      - Repositions the graph
    help                       - Show this help message
    exit                       - Exit the program
------------------------------------------------------------------------""")


    def _exit(self, args: list[str] = []) -> None:
        if len(args) > 1: raise ValueError("Usage: exit")
        sys.exit()


    def _show_logo(self) -> None:
        print(r"""
   _____                 _                 _
  / ____|               | |               (_)
 | |  __ _ __ __ _ _ __ | |__   ___  _ __  _
 | | |_ | '__/ _` | '_ \| '_ \ / _ \| '_ \| |
 | |__| | | | (_| | |_) | | | | (_) | | | | |
  \_____|_|  \__,_| .__/|_| |_|\___/|_| |_|_|
                  | |
                  |_|""")
