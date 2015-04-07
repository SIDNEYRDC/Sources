import networkx as nx
import matplotlib.pyplot as plt

G=nx.waxman_graph(10,1,1,1,(-4, 4, -4, 4))
# position is stored as node attribute data for random_geometric_graph
pos=nx.get_node_attributes(G,'pos')

nx.write_adjlist(G, "test.adjlist")

# find node near center (0.5,0.5)
dmin=1
ncenter=0
for n in pos:
    print pos[n]
    x,y=pos[n]
    d=(x-0.5)**2+(y-0.5)**2
    if d<dmin:
        ncenter=n
        dmin=d
        
print 'is connected: %s' % (nx.is_connected(G))
print 'is bi-connected: %s' % (nx.is_biconnected(G))

nx.draw(G,pos)
plt.show()

# color by path length from node near center
#p=nx.single_source_shortest_path_length(G,ncenter)

#plt.figure(figsize=(8,8))
#nx.draw_networkx_edges(G,pos,nodelist=[ncenter],alpha=0.4)
#nx.draw_networkx_nodes(G,pos,nodelist=p.keys(),
                       #node_size=80,
                       #node_color=p.values(),
                       #cmap=plt.cm.Reds_r)

#plt.xlim(-10,10)
#plt.ylim(-10,10)
#plt.axis('off')
#plt.savefig('random_geometric_graph.png')
#plt.show()

