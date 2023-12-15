#include "widgets/spinner.hh"
#include <string>

namespace softeq
{
namespace lvglplus
{

const short SpinnerParams::defaultHeight = 100;
const short SpinnerParams::defaultWidth = 100;
const std::string SpinnerParams::defaultText;
using namespace std::placeholders;

Spinner::Spinner(const SpinnerParamsBuilder &params)
    : Widget("Spinner", params.getParams(),
             std::bind(lv_spinner_create, _1, params.getSpinnerParams().spinTime, params.getSpinnerParams().arcLength))
    , IndicatorOwner(lvglObject.getObject())
{
    auto spinnerParams = params.getSpinnerParams();
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::noChild);
}

Spinner::Spinner()
    : Spinner(SpinnerParamsBuilder())
{
}

void Spinner::render()
{
}

Spinner::~Spinner() = default;

} // namespace lvglplus
} // namespace softeq