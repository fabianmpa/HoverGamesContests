#ifndef FILTER_HPP
#define FILTER_HPP
#include<array>
#include <algorithm>
#define FILTER_ORDER 17U
template<typename T>
class filter
{
public:
    typedef T value_type;
    filter(const value_type val = 0) : result(val)
    {
        /*Initialize values array to the first value*/
        std::fill(values.begin(), values.end(), val);
    }
    void new_sample(const std::array<value_type, 17U>& b,const value_type& val)
    {
        result = 0;
        /* Shift the delay line */
        for(size_t t=0; t<16; t++)
        {
            values[t] = values[t+1];
        }
        values[16] = val;
        /* Calculate the FIR algorithm */
        for(size_t t=1; t<(FILTER_ORDER +1); t++)
        {
            result = result + (b[t-1] * values[FILTER_ORDER - t]);
        }
    }
    const value_type& get_result() const
    {
        return result;
    }
private:
    value_type result;
    std::array<value_type, 17U> values;
};

#endif
