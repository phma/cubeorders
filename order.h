/******************************************************/
/*                                                    */
/* order.h - hypercube corner order                   */
/*                                                    */
/******************************************************/
/* Copyright 2022 Pierre Abbat.
 * This file is part of Cubeorders.
 *
 * Cubeorders is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Cubeorders is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cubeorders. If not, see <http://www.gnu.org/licenses/>.
 */

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
