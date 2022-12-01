/******************************************************/
/*                                                    */
/* order.h - hypercube corner order                   */
/*                                                    */
/******************************************************/

#include <quadlods.h>
#include <vector>

class Order
{
public:
  std::vector<uint32_t> seq;
};

void init(int n);
Order gen();
