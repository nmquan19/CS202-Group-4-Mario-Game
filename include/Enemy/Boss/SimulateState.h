#pragma once
#pragma once
#include <memory>
class BaseSimState {
public:
    virtual ~BaseSimState() = default;
    virtual std::unique_ptr<BaseSimState> clone() const = 0;
};
