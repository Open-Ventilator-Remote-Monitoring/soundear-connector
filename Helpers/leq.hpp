#ifndef LEQ_HPP
#define LEQ_HPP


class LEQ
{
private:
    int _counter=0;
    float _laeq=0;
    float _tmp=0;
public:
    void AddLaeq(float laeq);
    float Calculate();
    void Reset();
};

#endif // LEQ_HPP
