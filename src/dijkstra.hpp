// Submitter: solomc1(Chan, Solomon)
// I certify that I worked cooperatively on this programming
//   assignment, according to the rules for  programming

#ifndef DIJKSTRA_HPP_
#define DIJKSTRA_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>                    //Biggest int: std::numeric_limits<int>::max()
#include "array_queue.hpp"
#include "array_stack.hpp"
#include "heap_priority_queue.hpp"
#include "hash_graph.hpp"

namespace ics {

class Info {
	public:
  Info() {}
  Info(std::string a_node) : node(a_node) {}

		bool operator == (const Info& rhs) const
		{return cost == rhs.cost && from == rhs.from;}

		bool operator !=( const Info& rhs) const
    {return !(*this == rhs);}

		friend std::ostream& operator<<(std::ostream& outs, const Info& i) {
		  outs << "Info[" << i.cost << "," << i.from << "]";
			return outs;
		}

		//Public instance variable definitions
    std::string node = "?";
    int         cost = std::numeric_limits<int>::max();
    std::string from = "?";
};

bool gt_info (const Info& a, const Info& b) {return a.cost < b.cost;}

typedef ics::HashGraph<int>                                  DistGraph;
typedef ics::HeapPriorityQueue<Info, gt_info>                CostPQ;
typedef ics::HashMap<std::string, Info, DistGraph::hash_str> CostMap;
typedef ics::pair<std::string, Info>                         CostMapEntry;

//Return a queue whose front is the start node (implicit in answer_map) and whose
//  rear is the end node
ArrayQueue<std::string> recover_path(const CostMap& answer_map, std::string end_node) {
	ArrayQueue<std::string> answerQueue;
	std::vector<std::string> reversePath;
	// FILL IN reversePath
		//.pushback
	reversePath.push_back(end_node);
	Info pathVal = answer_map[end_node];
	std::string key = pathVal.from;
	while (key != "?"){
		reversePath.push_back(key);
		pathVal = answer_map[key];
		key = pathVal.from;
	}
	for(int i = reversePath.size()-1; i >= 0; i--) {
		answerQueue.enqueue(reversePath[i]);
	}
	return answerQueue;
}

//Return the final_map as specified in the lecture-node description of
//  extended Dijkstra algorithm
CostMap extended_dijkstra(const DistGraph& g, std::string start_node) {
	CostMap answer_map;
	CostMap info_map;
	CostPQ info_pq;
	for (auto key : g.all_nodes()) {
		Info i = Info(key.first);
		if(key.first == start_node) {
			i.cost = 0;
		}
		info_pq.enqueue(i);
		info_map.put(key.first, i);
	}

	while(!info_map.empty()) {
		Info i = info_pq.dequeue();
		if (i.cost == std::numeric_limits<int>::max()){
			break;
		}
		if (answer_map.has_key(i.node)) {
			continue;
		}
		std::string min_node = i.node;
		int min_cost = i.cost;
		// i.node is the key, i is the value
		info_map.erase(i.node);
		answer_map.put(i.node, i);
		for(auto/*std::string*/ d : g.out_nodes(min_node)) {
			if (!answer_map.has_key(d)) {
				Info& di = info_map[d]; // info_map.operator[](d)
				int minD = 0;
				if(g.has_edge(min_node,d)) {
					minD = g.edge_value(min_node, d);
				}
				int current_path_cost = min_cost + minD; // the cost of the path from start_node to min_node to d
				if(di.cost == std::numeric_limits<int>::max() || di.cost > current_path_cost) {
					di.cost = current_path_cost;
					di.from = min_node;
					info_pq.enqueue(di);
				}
			}
		}
	}
	return answer_map;
}

} /* namespace ics */

#endif /* DIJKSTRA_HPP_ */
