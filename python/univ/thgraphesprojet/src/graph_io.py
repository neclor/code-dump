from networkx import Graph


def load(path: str) -> Graph:
    numbers: list[int] = []

    with open(path) as file:
        for line in file:
            line = line.strip()
            if not line or line.startswith("#"): continue

            parts: list[str] = line.split()
            for part in parts:
                part = part.strip()
                if not part: continue

                try:
                    num: int = int(part)
                    numbers.append(num)
                except ValueError:

                    raise ValueError(f"Invalid file: '{part}' not a number.")

    if len(numbers) % 3 != 0: raise ValueError("Invalid file.")

    graph: Graph = Graph()
    for i in range(0, len(numbers), 3):
        graph.add_edge(numbers[i], numbers[i + 1], weight=numbers[i + 2])

    return graph


def save(graph: Graph, path: str) -> None:
    with open(path, "w") as file:
        for u, v, data in graph.edges(data=True):
            weight: int = data.get("weight", 0)
            file.write(f"{u} {v} {weight}\n")
