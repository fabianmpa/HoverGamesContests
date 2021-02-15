#ifndef FILTER_ORDER1_H
#define FILTER_ORDER1_H
#include<array>
#include <algorithm>

template<const std::size_t resol = 4U,typename sample_t = std::uint16_t,typename value_t = sample_t,typename result_t= sample_t>
class filter_order1
{
public:
    typedef sample_t          sample_type;
    typedef value_t           value_type;
    typedef result_t          result_type;
    typedef std::int_fast16_t weight_type;
    filter_order1(const sample_type& val = 0U) : result(val * resol)
    {
        /*Initialize values array to the first value*/
        std::fill(values.begin(), values.end(), result);
    }

    template<const weight_type B0, const weight_type B1>
    void new_sample(const sample_type& val)
    {
        values[0U] = values[1U];
        values[1U] = val * static_cast<value_type>(resol);
        value_type new_val = (B0 * values[0U]) + (B1 * values[1U]);
        result = (new_val + ((B0 + B1) / 2)) / (B0 + B1);
    }
    result_type get_result() const
    {
        return (result + (resol / 2U)) / resol;
    }

private:
    result_type result;
    std::array<value_type, 2U> values;
};
#endif
