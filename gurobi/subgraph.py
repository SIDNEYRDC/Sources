class Graph(object):
    def __init__(self, vertices):
        self.vertices = frozenset(vertices)
        # add edge logic here and to methods, etc. etc.

    def subgraphs(self):
        cache = set()
        def helper(graph):
            yield graph
            for element in graph:
                #if {{REMOVING ELEMENT WOULD DISCONNECT GRAPH}}:
                    # you fill in above function; easy if
                    # there is 0 or 1 ring in molecule
                    # (keep track if molecule has ring, e.g.
                    #  self.numRings, maybe even more data)
                    # if you know there are 0 rings the operation
                    #  takes O(1) time
                    continue
                subgraph = Graph(graph.vertices-{element})
                if not subgraph in cache:
                    cache.add(subgraph)
                    for s in helper(subgraph):
                        yield s
        for graph in helper(self):
            yield graph

    def __eq__(self, other):
        return self.vertices == other.vertices
    def __hash__(self):
        return hash(self.vertices)
    def __iter__(self):
        return iter(self.vertices)
    def __repr__(self):
        return 'Graph(%s)' % repr(set(self.vertices))
    
    
if __name__ == "__main__":
    print 'bla bla bla'