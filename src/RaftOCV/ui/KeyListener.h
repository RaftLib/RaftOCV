#pragma once

struct KeyListener_t;
void Register(KeyListener_t& listener);
void Deregister(KeyListener_t& listener);
struct KeyListener_t {
    virtual ~KeyListener_t() {
        Deregister(*this);
    }
    KeyListener_t() {
        Register(*this);
    }
    virtual void OnKey(int key) = 0;
};