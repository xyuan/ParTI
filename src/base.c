/*
    This file is part of SpTOL.

    SpTOL is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    SpTOL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with SpTOL.
    If not, see <http://www.gnu.org/licenses/>.
*/

#include <SpTOL.h>

size_t sptMaxSizeArray(
  size_t const * const indices,
  size_t const size)
{
  size_t max = indices[0];
  for(size_t i=1; i < size; ++i) {
    if(indices[i] > max) {
      max = indices[i];
    }
  }
  return max;
}