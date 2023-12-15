#ifndef LVGL_PLUS_POINT_HH
#define LVGL_PLUS_POINT_HH

struct Point
{
    short x = 0;
    short y = 0;
    Point(short x_, short y_)
    {
        x = x_;
        y = y_;
    }
    Point()
    {
    }
};

#endif // LVGL_PLUS_POINT_HH
