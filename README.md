# VirtualCircuitSwitchingSimulation
how to run ?

--->for compilation : g++ main.cpp -o routing
   
   
   input topology files : top14.txt or top24.txt

   input connections files : NSFNET_100.txt and ARPANET_100.txt 

---> run using command : ./routing -top top14.txt -conn NSFNET_100.txt -rt routingfile.txt -ft forwarding.txt -path pathfile.txt -flag hop -p 0

1) for taking path cost as number of hops, type anyword other than "hop" in above command

2) for optimistic approach : use -p 0

   for pessimistic approach : use -p 1


routingfile.txt stores first and second shortest paths between every 2 nodes.

forwarding.txt stores forwarding info of each node as

                                                     interface_in   label_in   interface_out   label_out

output.txt stores info about all admitted connections as follows:-
         connectionnumber  source   destination   pathlabels    pathcost     



Used DataStructures :- 

network_topology[][][2] {
	network_topology[i][j][0] is used for storing cost between i and j
	network_topology[i][j][1] is used for storing capacity between i and j
}

cost_parent[][][2] {
	cost_parent[i][j][0] is used for storing path cost between i and j
	cost_parent[i][j][1] is used for storing last edge parent in the path between i and j
}

first_shortest_path[][][], sec_shortest_path[][][] {
   first_shortest_path[i][j][0] is first edge in the first shortest path between i and j, and [i][j][1] is second edge in that path.....
   similarly sec_shortest_path stores second shortest disjoint path between i and j.
}

forwardingtable[maximum_nodes][maximum_conn][4]{
	for node i and for some particular connection,
	                                0th index indicates interface_in
	                                1st index indicates label_in
	                                2nd index indicates interface_out
	                                3rd index indicates label_out 
}
