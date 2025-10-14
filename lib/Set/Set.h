#pragma once

#include <Arduino.h>

template <typename T, size_t N>
class Set {
  private:
    T data[N];
    size_t count = 0;

  public:
    Set() = default;
    template <size_t M>
    Set(const T (&arr)[M])
    {
        for (int i = 0; i < M && i < N; i++)
            add(arr[i]);
    }

    bool add(const T& value)
    {
        if (contains(value) || count >= N)
            return false;
        data[count++] = value;
        return true;
    }
    bool erase(const T& value)
    {
        for (int i = 0; i < count; i++) {
            if (data[i] == value) {
                data[i] = data[--count];
                return true;
            }
        }
        return false;
    }
    bool contains(const T& value)
    {
        for (int i = 0; i < count; i++) {
            if (data[i] == value)
                return true;
        }
        return false;
    }
    size_t size() const { return count; }
};