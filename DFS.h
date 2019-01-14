template <class E ,class T> class DFS : public Searcher<E,T> {
public:

    int getNumberOfNodesEvaluated() {
        return 1;
    }

    Solution <E> search(Searchable <T> *searchable) {

        Solution <E> solution;
        std::set <State<T>> visited_nodes;
        std::stack <State<T>> route; // route to target
        std::vector <State<T>> adj_nodes;

        State <T> current_node = searchable->getInitialState();
        //  solution.addSolutionElement(current_node);

        while (!searchable->isGoalState(current_node)) {
            adj_nodes = searchable->getAllPossibleStates(current_node);

            bool found_non_visited = false;

            for (auto node : adj_nodes) {
                if (visited_nodes.find(node) == visited_nodes.end()) {
                    route.push(current_node);
                    visited_nodes.insert(node);
                    current_node = node;
                    found_non_visited = true;

                    break;
                }
            }

            if (found_non_visited) {
                continue;
            }

            if (route.empty()) {
                return nullptr;
            }
            current_node = route.top();
            route.pop();
        }
        // push goal node
        route.push(current_node);

        int stackSize = route.size();
        for (int i = 0; i < stackSize; ++i) {
            solution.addSolutionElement(route.top());
            route.pop();
        }
        return solution;

    }
}