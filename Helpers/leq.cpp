#include "leq.hpp"
#include <cmath>

void LEQ::AddLaeq(float laeq)
{
    _tmp += std::pow(10.0f, (laeq/10.0f));
    _counter++;
}

float LEQ::Calculate()
{
    return 10* std::log10(_tmp/_counter);
}

void LEQ::Reset()
{
    _tmp=0;
    _laeq=0;
    _counter=0;
}
