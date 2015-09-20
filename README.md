# MPGAAstar
Written MPGAAstar code in C++. 
The code creates an initial map without knowing the walls beforehand. It then creates an initial A* path through what it knows, and 
travels through the path, observing by a given sensor range on each node in the path until a change has been observed. It then
creates a new path with what it has learned, using old optimal paths that are saved on nodes from prior paths if it runs across the node 
again. This is just the base code without any sort of Controller such as Player/Stage or other robot mainframes. 
