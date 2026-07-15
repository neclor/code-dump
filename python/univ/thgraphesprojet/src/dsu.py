class Dsu:
    def __init__(self, nodes: list) -> None:
        self._parent: dict = {node: node for node in nodes}


    def find(self, node):
        if self._parent[node] != node:
            self._parent[node] = self.find(self._parent[node])
        return self._parent[node]


    def union(self, a: int, b: int) -> None:
        parent_a = self.find(a)
        parent_b = self.find(b)

        if parent_a != parent_b:
            self._parent[parent_a] = parent_b
