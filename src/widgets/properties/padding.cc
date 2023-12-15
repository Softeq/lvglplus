#include "widgets/properties/padding.hh"

namespace softeq
{
namespace lvglplus
{
const short Padding::defaultPadding = 0;

Padding::Padding(short value)
{
    _top = value;
    _right = value;
    _bottom = value;
    _left = value;
}

Padding::Padding(short top, short right, short bot, short left)
    : _top(top)
    , _right(right)
    , _bottom(bot)
    , _left(left)
{
    /* No padding by default */
    _column = defaultPadding;
    _row = defaultPadding;
}

Padding::Padding(short row, short column)
    : _row(row)
    , _column(column)
{
    /* No padding by default */
    _top = defaultPadding;
    _right = defaultPadding;
    _bottom = defaultPadding;
    _left = defaultPadding;
}

Padding::Padding(short top, short right, short bot, short left, short row, short column)
    : _top(top)
    , _right(right)
    , _bottom(bot)
    , _left(left)
    , _row(row)
    , _column(column)
{
}

Padding Padding::left(short value)
{
    _left = value;
    return *this;
}

Padding Padding::right(short value)
{
    _right = value;
    return *this;
}
Padding Padding::top(short value)
{
    _top = value;
    return *this;
}

Padding Padding::bottom(short value)
{
    _bottom = value;
    return *this;
}

Padding Padding::row(short value)
{
    _row = value;
    return *this;
}

Padding Padding::column(short value)
{
    _column = value;
    return *this;
}
} // namespace lvglplus
} // namespace softeq