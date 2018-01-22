#pragma once
#include <vector>
#include "dr_core_prerequisites.h"
#include <dr_vector2d.h>

namespace driderSDK
{

/**
* Vector with 2 elements (x, y)
*
* Sample usage:
* Logger sampleName;
* if (!sampleName.isStarted()) {
*   sampleName.startUp();
*   sampleName.reset();
* }
*/

class DR_CORE_EXPORT ConvexHull
{
 public: //Public Methods
  /**
  * TEST::defaultConstructor
  * Default constructor.
  */
  ConvexHull();

  /**
  * TEST::defaultDestructor
  * Default destructor.
  */
  ~ConvexHull();

  /**
  * Sorts the given vector using the Graham Algorithm.
  * This algorithm is a lot faster than Jarvis March, uses an
  * euristhic to sort.
  * This methods could fail when there're a lot of points.
  *
  * @param points.
  *   The vector of points to be analized and sorted.
  *
  * @param connectionsOut
  *   A vector where the index of the convex hull will be placed.
  */
  void
  GrahamScan(std::vector<Vector2D>& points,
             std::vector<Int32>& connectionsOut);

  /**
  * Computes a convex hull given the vector of points, using the
  * Jarvix March Algorithm.
  * This algorithm is a brute force, may be slow, but never fails.
  *
  * @param points.
  *   The vector of points to be analized and sorted.
  *
  * @param connectionsOut
  *   A vector where the index of the convex hull will be placed.
  */
  void
  JarvisMarch(const std::vector<Vector2D>& points,
              std::vector<Int32>& connectionsOut);

 protected: //Protected Methods

  /*
  * The main function that implements QuickSort
  * 
  * @param arr[]
  *   Array to be sorted,
  * @param low
  *   Starting index,
  * @param high
  *   Ending index
  */
  void
  polarSort(std::vector<Vector2D>& points, Int32 low, Int32 high);

  /*
  * This function takes last element as pivot, places the pivot
  * element at its correct position in sorted array, and places all
  * smaller (smaller than pivot) to left of pivot and all greater
  * elements to right of pivot.
  *
  * @param points
  *   Vector to be sorted.
  *
  * @param low
  *   Lowest index of the sorted vector.
  *
  * @param high
  *   Highest index of the sorted vector.
  *
  * @return
  *   The new something.
  */
  Int32
  partition(std::vector<Vector2D>& points, Int32 low, Int32 high);

  Vector2D gP0;
};

}