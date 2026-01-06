//
// Created by matija on 1/4/26.
//

#ifndef CATAN_DIRECTIONS_H
#define CATAN_DIRECTIONS_H

// directions for accessing adjacent elements and orientation
enum class PointDirection {
    Top,
    RightTop,
    RightBottom,
    Bottom,
    LeftBottom,
    LeftTop,
    End
};
enum class SideDirection {
    TopRight,
    Right,
    BottomRight,
    BottomLeft,
    Left,
    TopLeft,
    End
};


#endif //CATAN_DIRECTIONS_H

// TODO next direction iter? and direction arrays?