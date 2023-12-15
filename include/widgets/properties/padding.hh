#ifndef LVGL_PADDINGS_HH
#define LVGL_PADDINGS_HH

namespace softeq
{
namespace lvglplus
{

class Padding
{
public:
    Padding() = default;
    explicit Padding(short value);
    Padding(short row, short column);
    Padding(short top, short right, short bot, short left);
    Padding(short top, short right, short bot, short left, short row, short column);

    Padding left(short value);
    Padding right(short value);
    Padding top(short value);
    Padding bottom(short value);
    Padding row(short value);
    Padding column(short value);

    const short &getTop() const
    {
        return _top;
    }
    const short &getRight() const
    {
        return _right;
    }
    const short &getBottom() const
    {
        return _bottom;
    }
    const short &getLeft() const
    {
        return _left;
    }
    const short &getRow() const
    {
        return _row;
    }
    const short &getColumn() const
    {
        return _column;
    }

    ~Padding() = default;

    static const short defaultPadding;

private:
    short _top{defaultPadding};
    short _right{defaultPadding};
    short _bottom{defaultPadding};
    short _left{defaultPadding};
    short _row{defaultPadding};
    short _column{defaultPadding};
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_PADDINGS_HH
