#include <iostream>
#include <array>

namespace fizz
{
    template<size_t size>
    using array = std::array<char, size>;

    template <size_t size = 1>
    using array = std::array<char, size>;

    using number_t = std::size_t;
}

template <class InputIter, class DestIter>
constexpr void copy(InputIter first, InputIter last, DestIter dest) noexcept
{
    while (first != last)
        *dest++ = *first++;
}

template <size_t A, size_t B>
constexpr auto concatenate(const fizz::array<A>& a, const fizz::array<B>& b) noexcept
{
    fizz::array<A+B> arr {};
    copy(a.begin(), a.end(), arr.begin());
    copy(b.begin(), b.end(), arr.begin()+A);
    return arr;
}

template <fizz::number_t N>
constexpr auto getFizzNumber() noexcept
{
    constexpr char value = '0' + N % 10; 
    if constexpr (N < 10)   // constexpr necessary to remove different return value type for else
    {
        return fizz::array<>{value};
    }
    else
    {
        return concatenate(getFizzNumber<N/10>(), fizz::array<>{value});
    }
}

template <fizz::number_t N>
constexpr auto fizz_buzz()
{
    if constexpr (N % 3 == 0)
        return fizz::array<3>{'f','i','z'};
    else if constexpr (N % 5 == 0)
        return fizz::array<3>{'b','u','z'};
    else
        return getFizzNumber<N>();
}

template <unsigned N>
constexpr auto n_fizz_buzz() noexcept
{
    if constexpr (N > 1)
    {
        constexpr std::array<char, 1> separator{','};
        return concatenate(concatenate(n_fizz_buzz<N-1>(),separator), fizz_buzz<N>());

    }
    else
    {
        return fizz_buzz<N>();
    }
    
}

int main()
{
    constexpr auto fizzbuzzes = n_fizz_buzz<20>();

    std::cout << std::string_view(fizzbuzzes.data(), fizzbuzzes.size()) << std::endl;

    return 0;
}
