// Submitter: solomc1(Chan, Solomon)
// I certify that I worked cooperatively on this programming
//   assignment, according to the rules for  programming

#include <string>
#include <iostream>
#include <fstream>
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "hash_graph.hpp"
#include "dijkstra.hpp"



std::string get_node_in_graph(const ics::DistGraph& g, std::string prompt, bool allow_QUIT) {
  std::string node;
  for(;;) {
    node = ics::prompt_string(prompt + " (must be in graph" + (allow_QUIT ? " or QUIT" : "") + ")");
    if ((allow_QUIT && node == "QUIT") || g.has_node(node))
      break;
  }
  return node;
}

int main() {
  try {
//	  1) Prompt the user for a file name and load and print its graph, whose edges
//	       are labeled by non-negative integer values.
//
//	  2) Prompt the user to enter a start node.
//	  3) Call the extended_dijkstra function with this information, returning a map
//	       of the minimum costs to reach each node and the information needed to
//	       construct its path.
	std::ifstream f;
	ics::HashGraph<int> hashG;
	ics::safe_open( f, "Enter Graph file Name", "flightdist.txt");
	hashG.load(f,";");
	std::cout<<hashG;
	std::string begin = get_node_in_graph(hashG,"Enter start node",true);

	ics::CostMap minCost = ics::extended_dijkstra(hashG, begin);
	std::cout<< minCost<< "\n";

	std::string input = "";
	while(input != "QUIT") {
		input = get_node_in_graph(hashG,"\nEnter stop node",true);
		if (input=="QUIT") {
			break;
		}
		ics::ArrayQueue<std::string> path = recover_path(minCost,input);
		std::cout<<"Cost is "<< minCost[input].cost <<"; path is " << path << std::endl;
	}
  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
