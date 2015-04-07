import networkx as nx
import matplotlib.pyplot as plt

#G = nx.random_geometric_graph(10,0.5)
#G = nx.navigable_small_world_graph(10)
#G = nx.geographical_threshold_graph(10,1)
pos = nx.get_node_attributes(G,'pos')

for n in pos:
    print pos[n]

nx.draw(G,pos)
plt.show()
