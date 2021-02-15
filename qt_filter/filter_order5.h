#ifndef FILTER_ORDER5_H
#define FILTER_ORDER5_H
#include <array>
#include <algorithm>
template<const std::size_t resol=4U, typename sample_t=std::uint32_t, typename value_t=sample_t, typename result_t=sample_t>
class filter_order_5
{
public:
        typedef sample_t sample_type;
        typedef value_t value_type;
        typedef result_t result_type;

        filter_order_5(const sample_type& x = 0) : result(value_type(x) * resol)
        {
            /*Initialize values array to the first value*/
            std::fill(data.begin(), data.end(), result);
        }

        template<const std::int_fast16_t B0, const std::int_fast16_t B1, const std::int_fast16_t B2, const std::int_fast16_t B3,
                 const std::int_fast16_t B4, const std::int_fast16_t B5>

        void new_sample(const sample_type& x)
        {
            /* Shift the delay line */
            std::copy(data.begin() + 1U, data.end(), data.begin());
            /* Store the new sample at top of delay line. */
            *(data.end() - 1U) = value_type(x) * resol;
            /* Calculate the FIR algorithm */
            const value_type new_val = value_type(data[0U] * B0)
            + value_type(data[1U] * B1)
            + value_type(data[2U] * B2)
            + value_type(data[3U] * B3)
            + value_type(data[4U] * B4)
            + value_type(data[5U] * B5);

            constexpr std::int_fast16_t weight = B0 + B1 + B2 + B3 + B4 + B5;
            result = (new_val + (weight / 2)) / weight;
        }
        result_type get_result() const
        {
            return (result + (resol / 2U)) / resol;
        }
private:
        result_type result;
        std::array<value_type, 6U> data;
};
#endif
