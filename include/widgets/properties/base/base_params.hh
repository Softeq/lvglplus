#ifndef LVGL_PLUS_BASE_PARAMS_HH
#define LVGL_PLUS_BASE_PARAMS_HH

#include "lvgl.h"
#include <list>

namespace softeq
{
namespace lvglplus
{
struct BaseParams
{
    std::shared_ptr<std::list<std::function<void(lv_obj_t *obj)>>> instructions =
        std::make_shared<std::list<std::function<void(lv_obj_t *obj)>>>();
};

template <typename T>
class BaseParamsBuilder
{
private:
    BaseParams _baseBuilderParams;

protected:
    T &retVal()
    {
        return *(dynamic_cast<T *>(this));
    }

public:
    void addInstruction(std::function<void(lv_obj_t *obj)> instruction)
    {
        _baseBuilderParams.instructions->push_back(instruction);
    }

    std::shared_ptr<std::list<std::function<void(lv_obj_t *obj)>>> instructions()
    {
        return _baseBuilderParams.instructions;
    }

    virtual ~BaseParamsBuilder() = default;
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_BASE_PARAMS_HH
