/******************************************************/
/*                                                    */
/* order.h - hypercube corner order                   */
/*                                                    */
/******************************************************/

#include <quadlods.h>
#include <vector>
#include <string>

class Order
{
public:
  std::vector<uint32_t> seq;
};

void init(int n);
std::string gen();
