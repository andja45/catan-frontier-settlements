//
// Created by matija on 1/4/26.
//

#ifndef CATAN_DIRECTIONS_H
#define CATAN_DIRECTIONS_H
#include <utility>

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


namespace directions {
    inline SideDirection flipOrientation(PointDirection dir) {
        int sideCount=static_cast<int>(SideDirection::End);
        int sideStart=static_cast<int>(SideDirection::Left);
        int pointIndex=static_cast<int>(dir);
        return static_cast<SideDirection>((sideCount+sideStart-pointIndex)%sideCount);
    }

    inline PointDirection flipOrientation(SideDirection dir) {
        int pointCount=static_cast<int>(PointDirection::End);
        int pointStart=static_cast<int>(PointDirection::LeftBottom);
        int sideIndex=static_cast<int>(dir);
        return static_cast<PointDirection>((pointCount-sideIndex+pointStart)%pointCount);
    }
    inline PointDirection beginPoint() {
        return PointDirection::Top;
    }
    inline SideDirection beginSide() {
        return SideDirection::TopRight;
    }

    inline PointDirection nextPoint(PointDirection dir) {
        int count=static_cast<int>(PointDirection::End);
        return static_cast<PointDirection>((static_cast<int>(dir)+1)%count);
    }
    inline PointDirection prevPoint(PointDirection dir) {
        int count=static_cast<int>(PointDirection::End);
        return static_cast<PointDirection>((static_cast<int>(dir)+count-1)%count);
    }

    inline SideDirection prevSide(SideDirection dir) {
        int count=static_cast<int>(SideDirection::End);
        return static_cast<SideDirection>((static_cast<int>(dir)+count-1)%count);
    }

    inline SideDirection nextSide(SideDirection dir) {
        int count=static_cast<int>(SideDirection::End);
        return static_cast<SideDirection>((static_cast<int>(dir)+1)%count);
    }

    inline PointDirection oppositePoint(PointDirection dir) {
        int count=static_cast<int>(PointDirection::End);
        for (int i=0;i<count/2;i++) {
            dir=nextPoint(dir);
        }
        return dir;
    }
    inline SideDirection oppositeSide(SideDirection dir) {
        int count=static_cast<int>(SideDirection::End);
        for (int i=0;i<count/2;i++) {
            dir=nextSide(dir);
        }
        return dir;
    }

    inline SideDirection nextSide(PointDirection dir) {
        return static_cast<SideDirection>(static_cast<int>(dir));
    }
    inline PointDirection nextPoint(SideDirection dir) {
        return static_cast<PointDirection>(static_cast<int>(dir));
    }

    inline std::pair< PointDirection, PointDirection> adjacentPoints(SideDirection dir) {
        return {
            nextPoint(prevSide(dir)),
            nextPoint(dir)
        };
    }
    inline std::pair< SideDirection, SideDirection> adjacentSides(PointDirection dir) {
        return {
            nextSide(prevPoint(dir)),
            nextSide(dir)
        };
    }

    inline SideDirection sideBetween(PointDirection a, PointDirection b) {
        PointDirection start=a;

        auto [s1,s2]=adjacentSides(a);
        auto [s3,s4]=adjacentSides(b);
        if (s1==s3 || s1==s4) {
            return s1;
        }
        return s2;
    }


}

#endif //CATAN_DIRECTIONS_H

