#pragma once

template <class T>
T CalcNumberOfDigits(T num)
{
    T result = 0;
    do
    {
        result++;
    } while ((num /= 10));

    return result;
}