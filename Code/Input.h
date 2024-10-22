#pragma once

#include <unordered_map>

enum KeyState
{
    None,
    Pressed,
    Held,
    Released
};

class Input
{
public:
    static void PrepareKeyStates()
    {
        for (auto& key : keyStateMap)
        {
            if (key.second == Pressed)
                key.second = Held;
            else if (key.second == Released)
                key.second = None;
        }
    }

    static bool GetKey(int key)
    {
        if (keyStateMap.find(key) == keyStateMap.end())
            return false;

        return keyStateMap[key] == Pressed || keyStateMap[key] == Held;
    }

    static bool GetKeyDown(int key)
    {
        if (keyStateMap.find(key) == keyStateMap.end())
            return false;

        return keyStateMap[key] == Pressed;
    }

    static bool GetKeyUp(int key)
    {
        if (keyStateMap.find(key) == keyStateMap.end())
            return false;

        return keyStateMap[key] == Released;
    }

    static void Press(int key)
    {
        keyStateMap[key] = Pressed;
    }

    static void Release(int key)
    {
        keyStateMap[key] = Released;
    }

private:
    inline static std::unordered_map<int, KeyState> keyStateMap;
};
