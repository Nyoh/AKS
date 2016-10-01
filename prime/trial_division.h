#ifndef TRIAL_DIVISION_H
#define TRIAL_DIVISION_H

namespace Prime
{
    template <typename T>
    bool IsPrimeTrialDivision(const Num<T>& value)
    {
        if (value < 2) return false;
        if (value < 4) return true;
        if (value.Bit(0) == false) return false; // Even number

        const limit = value.SquareRoot();
        for (Num<T> i = Num<T>(2); i != limit; ++i)
        {
            if (value % i == 0)
                return false;
        }

        return true;
    }
}

#endif // TRIAL_DIVISION_H
