#include <vector>
#include <cmath>
struct Node
{
   int parent;
   int rank;
   int size;
};

struct Forest
{
   std::vector< Node >;
   int num_nodes; // q of px in image
   int size_of(int i);
   int find(int n);
   void merge(int a, int b);
};

