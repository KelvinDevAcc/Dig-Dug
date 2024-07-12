#pragma once
class Observer {
public:
    virtual ~Observer() = default;

    virtual void OnNotify() = 0;
};
