#include<bitset>
#include<iostream>
#include <cassert>
using namespace std;
//#define LUCAL



unsigned int INT_TO_GRAY (int X){
        return (unsigned int)(X^((unsigned int)X >> 1));
}


template<size_t N> bitset<N> 
reflect( const bitset<N>& b ) {
   unsigned int t = b.to_ulong();
   bitset<N> ret(-t);
   return ret;
}


template< size_t N > class GRAY_INT {
        public:
                GRAY_INT() {} ;

                void                print() const  
                                        {cout << m_value << endl;} 

                GRAY_INT&           SetBits( unsigned int X ) 
                                        { m_value = X; };
                GRAY_INT&           SetInt( unsigned int X) 
                                        { m_value = INT_TO_GRAY(X); }; 
                const bitset<N>&    Bits() const
                                        {return m_value;};
                const int          Int() const {
                        unsigned int a = Bits().to_ulong();
                        unsigned int digit = 1;
                        unsigned int tmp;
                        while (tmp = (a >> digit)){
                                a ^= tmp;
                                digit <<= 1;
                        }
                        return a;
                }

                bool                isOdd() const 
                                        {  return static_cast<bool>(__builtin_parity(m_value.to_ulong())); }       
                bool                isEven() const 
                                        { return !isOdd(); };


                GRAY_INT& operator++(int) {
                        if(this->isOdd()) {
                                bitset<N> t = Bits() & reflect(Bits());
                                m_value ^= (t << 1);
                        } else {
                                m_value ^= 1;
                        }
                        return *this;
                }

                GRAY_INT& operator--(int) {
                        if(this->isOdd()) {
                                m_value ^=1;
                        } else {
                           if (m_value == 0) {
                                   // We do not deal with negative gray codes, sorry
                                throw(std::exception());
                           } else {
                                bitset<N> t = Bits() & reflect(Bits());
                                m_value ^= (t << 1);
                           }
                        }
                                                        }
                

                
                
                GRAY_INT operator+(const GRAY_INT& rhs)  {
#ifdef LUCAL
                        bitset<N> rbits = rhs.Bits();
                        bitset<N> lbits = this->Bits();
                        // DEBUG ASSERT
                        if(this->Int()  > rhs.Int()) {
                           cout <<  lbits.to_ulong() << endl;
                           cout <<  rbits.to_ulong() << endl;
                           assert(0);
                        }

                        // parity of lhs and rhs
                        bool lhs_p = this->isOdd();
                        bool rhs_p = rhs.isOdd();

                        bitset<N>  res(lbits ^ rbits);
                        for (size_t i=0; i < N; i++) {
                                bool lhs_i = lbits[i];
                                bool rhs_i = rbits[i];
                                bool lhs_p_cpy = lhs_p;
                                bool rhs_p_cpy = rhs_p;

                                unsigned int res_i = lhs_p_cpy & rhs_p_cpy;
                                res ^= res_i << i;

                                lhs_p = (lhs_p_cpy & (!rhs_p_cpy)) ^ lhs_i;
                                rhs_p = ((!lhs_p_cpy) & rhs_p_cpy) ^ rhs_i;
                        }
                        GRAY_INT<N> r;
                        r.SetBits(res.to_ulong());
                        return r;
#else
                        bool v0 = false;
                        bool v1 = false;
                        bool prev_bit = 0;
                        int prebit = N-2;

                        bitset<N> rbits = rhs.Bits();
                        bitset<N> lbits = this->Bits();
                        bitset<N>  res(lbits ^ rbits);
                        for (size_t bit = N-1; bit >= 0; bit--){
                                if (lbits.test(bit)) {
                                        v0 = !v0;
                                        if (rbits.test(bit)) {
                                                v1 = !v1;
                                                if (v0 == v1) {
                                                        res.flip(bit + 1);
                                                        prev_bit = v0;
                                                        prebit = bit;
                                                }
                                        }
                                        else {
                                                if (prev_bit == v0){
                                                        res.flip(prebit + 1);
                                                }
                                                prev_bit = v0;
                                                prebit = bit;
                                        }
                                }
                                else if (rbits.test(bit)) {
                                        v1 = !v1;
                                        if (prev_bit == v1){
                                                res.flip(prebit + 1);
                                        }
                                        prev_bit = v1;
                                        prebit = bit;

                                }
                        }


                        if ((v0 || v1)& !prev_bit){
                                res.flip(prebit + 1);
                        }
                        else if (v0 && v1){
                                res.flip(1);
                        }

                        GRAY_INT<N> r;
                        r.SetBits(res.to_ulong());
                        return r;


#endif
                }

                GRAY_INT<N> twice() const{
                        GRAY_INT<N> ret;
                        ret.SetBits((this->Bits() << 1) & this->isOdd());
                        return ret;
                }

                private:
                GRAY_INT(int);
                bitset<N> m_value;

};


int main() {
  bitset<sizeof(int)*8> x,y;

  GRAY_INT<10> a, b;
  a.SetInt(4);
  b.SetInt(5);
  GRAY_INT<10> c = a + b;
  c.print();
  for (int i = 0; i < 10; i++) {
          a++;
          int j = a.Int();
          cout << j << " " ;
          a.print();
          a.twice().print();


  }
  return 0;

}

