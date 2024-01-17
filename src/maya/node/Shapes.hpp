// System includes
#include <array>
#include <vector>

using namespace std;



//
// Cube
//
array<array<float,3>,8> pointsCube = {{
  {-0.5, -0.5,  0.5},
  { 0.5, -0.5,  0.5},
  {-0.5,  0.5,  0.5},
  { 0.5,  0.5,  0.5},
  {-0.5,  0.5, -0.5},
  { 0.5,  0.5, -0.5},
  {-0.5, -0.5, -0.5},
  { 0.5, -0.5, -0.5}
}};
array<array<float,3>,2> bboxCube = {{
  {-0.5, -0.5, -0.5}, {0.5, 0.5, 0.5}
}};
vector<pair<int,int>> indiciesCube = {{
  {0, 1},
  {1, 3},
  {3, 2},
  {2, 0},
  {0, 6},
  {6, 7},
  {7, 1},
  {7, 5},
  {5, 3},
  {5, 4},
  {4, 2},
  {4, 6}
}};




//
// Square
//
array<array<float,3>,4> pointsSquare = {{
  {-0.5, 0.0,  0.5},
  { 0.5, 0.0,  0.5},
  {-0.5, 0.0, -0.5},
  { 0.5, 0.0, -0.5}
}};
array<array<float,3>,2> bboxSquare = {{
  {-0.5, 0.0, -0.5}, {0.5, 0.0, 0.5}
}};
vector<pair<int,int>> indiciesSquare = {{
  {0, 1},
  {1, 3},
  {3, 2},
  {2, 0},
}};




//
// Cylinder
//
array<array<float,3>,64> pointsCylinder = {{
  {0.49, -0.5, -0.098},
  {0.462, -0.5, -0.191},
  {0.416, -0.5, -0.278},
  {0.354, -0.5, -0.354},
  {0.278, -0.5, -0.416},
  {0.191, -0.5, -0.462},
  {0.098, -0.5, -0.49},
  {0.0, -0.5, -0.5},
  {-0.098, -0.5, -0.49},
  {-0.191, -0.5, -0.462},
  {-0.278, -0.5, -0.416},
  {-0.354, -0.5, -0.354},
  {-0.416, -0.5, -0.278},
  {-0.462, -0.5, -0.191},
  {-0.49, -0.5, -0.098},
  {-0.5, -0.5, -0.0},
  {-0.49, -0.5, 0.098},
  {-0.462, -0.5, 0.191},
  {-0.416, -0.5, 0.278},
  {-0.354, -0.5, 0.354},
  {-0.278, -0.5, 0.416},
  {-0.191, -0.5, 0.462},
  {-0.098, -0.5, 0.49},
  {-0.0, -0.5, 0.5},
  {0.098, -0.5, 0.49},
  {0.191, -0.5, 0.462},
  {0.278, -0.5, 0.416},
  {0.354, -0.5, 0.354},
  {0.416, -0.5, 0.278},
  {0.462, -0.5, 0.191},
  {0.49, -0.5, 0.098},
  {0.5, -0.5, 0.0},
  {0.49, 0.5, -0.098},
  {0.462, 0.5, -0.191},
  {0.416, 0.5, -0.278},
  {0.354, 0.5, -0.354},
  {0.278, 0.5, -0.416},
  {0.191, 0.5, -0.462},
  {0.098, 0.5, -0.49},
  {0.0, 0.5, -0.5},
  {-0.098, 0.5, -0.49},
  {-0.191, 0.5, -0.462},
  {-0.278, 0.5, -0.416},
  {-0.354, 0.5, -0.354},
  {-0.416, 0.5, -0.278},
  {-0.462, 0.5, -0.191},
  {-0.49, 0.5, -0.098},
  {-0.5, 0.5, -0.0},
  {-0.49, 0.5, 0.098},
  {-0.462, 0.5, 0.191},
  {-0.416, 0.5, 0.278},
  {-0.354, 0.5, 0.354},
  {-0.278, 0.5, 0.416},
  {-0.191, 0.5, 0.462},
  {-0.098, 0.5, 0.49},
  {-0.0, 0.5, 0.5},
  {0.098, 0.5, 0.49},
  {0.191, 0.5, 0.462},
  {0.278, 0.5, 0.416},
  {0.354, 0.5, 0.354},
  {0.416, 0.5, 0.278},
  {0.462, 0.5, 0.191},
  {0.49, 0.5, 0.098},
  {0.5, 0.5, 0.0},
}};
array<array<float,3>,2> bboxCylinder = {{
  {-0.5, 0.5, -0.5}, {0.5, 0.5, 0.5}
}};
vector<pair<int,int>> indiciesCylinder = {{
  {0, 1},
  {1, 2},
  {2, 3},
  {3, 4},
  {4, 5},
  {5, 6},
  {6, 7},
  {7, 8},
  {8, 9},
  {9, 10},
  {10, 11},
  {11, 12},
  {12, 13},
  {13, 14},
  {14, 15},
  {15, 16},
  {16, 17},
  {17, 18},
  {18, 19},
  {19, 20},
  {20, 21},
  {21, 22},
  {22, 23},
  {23, 24},
  {24, 25},
  {25, 26},
  {26, 27},
  {27, 28},
  {28, 29},
  {29, 30},
  {30, 31},
  {31, 0},
  {32, 33},
  {33, 34},
  {34, 35},
  {35, 36},
  {36, 37},
  {37, 38},
  {38, 39},
  {39, 40},
  {40, 41},
  {41, 42},
  {42, 43},
  {43, 44},
  {44, 45},
  {45, 46},
  {46, 47},
  {47, 48},
  {48, 49},
  {49, 50},
  {50, 51},
  {51, 52},
  {52, 53},
  {53, 54},
  {54, 55},
  {55, 56},
  {56, 57},
  {57, 58},
  {58, 59},
  {59, 60},
  {60, 61},
  {61, 62},
  {62, 63},
  {63, 32},
  {63, 31},
  {39, 7},
  {47, 15},
  {55, 23},
}};



//
// Circle
//
array<array<float,3>,32> pointsCircle = {{
  {0.49, 0.0, -0.098},
  {0.462, 0.0, -0.191},
  {0.416, 0.0, -0.278},
  {0.354, 0.0, -0.354},
  {0.278, 0.0, -0.416},
  {0.191, 0.0, -0.462},
  {0.098, 0.0, -0.49},
  {0.0, 0.0, -0.5},
  {-0.098, 0.0, -0.49},
  {-0.191, 0.0, -0.462},
  {-0.278, 0.0, -0.416},
  {-0.354, 0.0, -0.354},
  {-0.416, 0.0, -0.278},
  {-0.462, 0.0, -0.191},
  {-0.49, 0.0, -0.098},
  {-0.5, 0.0, -0.0},
  {-0.49, 0.0, 0.098},
  {-0.462, 0.0, 0.191},
  {-0.416, 0.0, 0.278},
  {-0.354, 0.0, 0.354},
  {-0.278, 0.0, 0.416},
  {-0.191, 0.0, 0.462},
  {-0.098, 0.0, 0.49},
  {-0.0, 0.0, 0.5},
  {0.098, 0.0, 0.49},
  {0.191, 0.0, 0.462},
  {0.278, 0.0, 0.416},
  {0.354, 0.0, 0.354},
  {0.416, 0.0, 0.278},
  {0.462, 0.0, 0.191},
  {0.49, 0.0, 0.098},
  {0.5, 0.0, 0.0},
}};
array<array<float,3>,2> bboxCircle = {{
  {-0.5, 0.0, -0.5}, {0.5, 0.0, 0.5}
}};
vector<pair<int,int>> indiciesCircle = {{
  {0, 1},
  {1, 2},
  {2, 3},
  {3, 4},
  {4, 5},
  {5, 6},
  {6, 7},
  {7, 8},
  {8, 9},
  {9, 10},
  {10, 11},
  {11, 12},
  {12, 13},
  {13, 14},
  {14, 15},
  {15, 16},
  {16, 17},
  {17, 18},
  {18, 19},
  {19, 20},
  {20, 21},
  {21, 22},
  {22, 23},
  {23, 24},
  {24, 25},
  {25, 26},
  {26, 27},
  {27, 28},
  {28, 29},
  {29, 30},
  {30, 31},
  {31, 0},
}};


