#ifndef LVGL_PLUS_BLOC_WIDGET_BUILDER_HH
#define LVGL_PLUS_BLOC_WIDGET_BUILDER_HH

#include "bloc/bloc.hh"
#include "bloc/bloc_base.hh"
#include "communication/streams/include/stream_controller.hh"
#include "stream_widget_builder.hh"
#include "widget.hh"

namespace softeq
{
namespace lvglplus
{
using BlocWidgetBuilderParamsBuilder = StreamWidgetBuilderParamsBuilder;

template <typename BlocT, typename State,
          typename = typename std::enable_if<std::is_base_of<BlocBase<State>, BlocT>::value>::type>
class BlocWidgetBuilder : public StreamWidgetBuilder<std::shared_ptr<State>>
{
public:
    BlocWidgetBuilder(std::shared_ptr<BlocT> bloc,
                      std::function<std::shared_ptr<Widget>(std::shared_ptr<State>)> builderFunc,
                      const BlocWidgetBuilderParamsBuilder &params)
        : StreamWidgetBuilder<std::shared_ptr<State>>(((BlocBase<State> *)(&(*bloc)))->state(),
                                                      ((BlocBase<State> *)(&(*bloc)))->getStateStream(), builderFunc,
                                                      params)
    {
        _bloc = bloc;
    }
    BlocWidgetBuilder(std::shared_ptr<BlocT> bloc,
                      std::function<std::shared_ptr<Widget>(std::shared_ptr<State>)> builderFunc)
        : BlocWidgetBuilder(bloc, builderFunc, BlocWidgetBuilderParamsBuilder())
    {
    }
    ~BlocWidgetBuilder() override = default;

private:
    std::shared_ptr<BlocT> _bloc;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_PLUS_BLOC_WIDGET_BUILDER_HH
