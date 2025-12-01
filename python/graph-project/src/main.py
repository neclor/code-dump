from networkx import Graph
from graph_file_io import GraphFileIO


def main() -> None:
    init()
    run()


def init() -> None:
    print(r"""
   _____                 _                 _
  / ____|               | |               (_)
 | |  __ _ __ __ _ _ __ | |__   ___  _ __  _
 | | |_ | '__/ _` | '_ \| '_ \ / _ \| '_ \| |
 | |__| | | | (_| | |_) | | | | (_) | | | | |
  \_____|_|  \__,_| .__/|_| |_|\___/|_| |_|_|
                  | |
                  |_|
""")


def run() -> None:
    while True:
        while True:
            input_path: str = input("Enter the path to the input file: ").strip()
            try:
                graph: Graph = GraphFileIO.load(input_path)
                break
            except Exception as e:
                print(f"Error: {e}")




        pass


if __name__ == "__main__": main()
