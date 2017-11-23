#include "dr_convexHull.h"

namespace driderSDK
{

ConvexHull::ConvexHull() {
}

ConvexHull::~ConvexHull() {
}

void
ConvexHull::GrahamScan(std::vector<Vector2D>& points,
  std::vector<Int32>& connectionsOut) {
  const Int32 pointsSz = points.size();

  // There must be at least 3 points
  if (pointsSz < 3) {
    return;
  }

  connectionsOut.clear();

  // Find the bottommost poInt32
  Int32 minYPoint = 0;
  float minYVal = points[minYPoint].y, minXVal = points[minYPoint].y;
  for (Int32 i = 1; i < pointsSz; i++) {
    // Pick the bottommost or the leftmost point in case of y tie
    if ((points[i].y < minYVal) ||
        (minYVal == points[i].y && points[i].x < minXVal)) {
      minYPoint = i;
      minYVal = points[minYPoint].y;
      minXVal = points[minYPoint].x;
    }
  }

  // Place the bottom-most point at first position
  std::swap(points[0], points[minYPoint]);

  // Sort n-1 points with respect to the first point. A point p1 comes
  // before p2 in sorted ouput if p2 has larger polar angle (in
  // counterclockwise direction) than p1.
  gP0 = points[0];
  polarSort(points, 1, pointsSz - 1);

  // Create an empty "stack" and push first three points to it.
  connectionsOut.push_back(0);
  connectionsOut.push_back(1);
  connectionsOut.push_back(2);
  Int32 iConnectionsOutTrueBack;

  std::vector<Vector2D> vecConnectionsOut;
  vecConnectionsOut.push_back(points[0]);
  vecConnectionsOut.push_back(points[1]);
  vecConnectionsOut.push_back(points[2]);
  Vector2D vecConnectionsOutTrueBack;

  // Process remaining n-3 points
  for (Int32 i = 3; i < pointsSz; ++i) {
    // Keep removing the top while the angle formed by points
    // next-to-top, top, and points[i] makes a non-left turn.
    iConnectionsOutTrueBack = connectionsOut.back();
    connectionsOut.pop_back();
    vecConnectionsOutTrueBack = vecConnectionsOut.back();
    vecConnectionsOut.pop_back();

    while (vecConnectionsOut.back().orientation(vecConnectionsOutTrueBack,
                                                points[i]) != 2) {
      iConnectionsOutTrueBack = connectionsOut.back();
      connectionsOut.pop_back();
      vecConnectionsOutTrueBack = vecConnectionsOut.back();
      vecConnectionsOut.pop_back();
    }

    vecConnectionsOut.push_back(vecConnectionsOutTrueBack);
    vecConnectionsOut.push_back(points[i]);
    connectionsOut.push_back(iConnectionsOutTrueBack);
    connectionsOut.push_back(i);
  }
}

void
ConvexHull::JarvisMarch(const std::vector<Vector2D>& points,
                        std::vector<Int32>& connectionsOut) {
  const Int32 pointsSz = points.size();

  // There must be at least 3 points
  if (pointsSz < 3) {
    return;
  }

  //Result
  connectionsOut.clear();

  // Find the leftmost point
  Int32 leftmostPoint = 0;
  for (Int32 testPoint = 1; testPoint < pointsSz; ++testPoint) {
    if (points[testPoint].x < points[leftmostPoint].x) {
      leftmostPoint = testPoint;
    }
  }

  // Start from leftmost point, keep moving counterclockwise
  // until reach the start point again
  Int32 lastConnection = leftmostPoint, nextConnection;

  do {
    // Search for a point 'nextConnection' such that
    // orientation(lastConnection, i, nextConnection) is
    // counterclockwise for all points 'i'
    //nextConnection = (lastConnection + 1) % pointsSz;
    nextConnection = 0;
    for (Int32 testPoint = 0; testPoint < pointsSz; ++testPoint) {
      if (points[lastConnection].orientation(points[testPoint],
                                             points[nextConnection]) == 2) {
        nextConnection = testPoint;
      }
    }

    // Add nextConnection to result as a next point of lastConnection
    connectionsOut.push_back(nextConnection);
    // Set lastConnection as nextConnection for next iteration
    lastConnection = nextConnection;
  } while (lastConnection != leftmostPoint);
}

void
ConvexHull::polarSort(std::vector<Vector2D>& points, Int32 low, Int32 high) {
  if (low < high) {
    Int32 partitioningIndex = partition(points, low, high);

    // Separately sort elements before and after partition
    polarSort(points, low, partitioningIndex - 1);
    polarSort(points, partitioningIndex + 1, high);
  }
}

Int32
ConvexHull::partition(std::vector<Vector2D>& points, Int32 low, Int32 high) {
  Int32 smallesElement = low - 1, highestElement = high - 1;

  for (Int32 j = low; j <= highestElement; ++j) {
    if (gP0.compare(points[j], points[high]) == -1) {
      std::swap(points[++smallesElement], points[j]);
    }
  }

  std::swap(points[smallesElement + 1], points[high]);

  return ++smallesElement;
}

}