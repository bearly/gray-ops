#include<bitset>
#include<iostream>
#include <cassert>
using namespace std;
#define LUCAL
#if 0
/** bit masks */
const uintmax_t
ALL_ONES = ~(uintmax_t)0,
         O_ONES = ALL_ONES/0xff, //0x...10101
         FIVES  = ALL_ONES/3,    //0x...33333
         THREES = ALL_ONES/5,    //0x...55555
         OFOFS  = ALL_ONES/17;   //0x...F0F0F

/** return number of bits set to one in <code>bits</code> */
// fast multiplication variant
int popCount(uintmax_t bits) {
        bits -= (bits >> 1) & FIVES;
        bits = (bits & THREES) + ((bits >> 2) & THREES);
        // of course, O7O7s would do - but 255 doesn't divide by 7
        bits = bits + (bits >> 4) & OFOFS;
        // needs one more "fold operation" and "ones spaced 16"
        //  for 256 <= #bits < 65536
        return (bits * O_ONES) >> (sizeof bits * 8);
}

/** return (odd) parity of <code>bits</code> */
int parity(uintmax_t bits) {
        uintmax_t shifted;
        for (int shift = 4 ; 0 < ((shifted = bits) >>= shift) ; shift *= 2)
                bits ^= shifted;
        return (0x6996 >> ((int)bits & 0xf)) & 1;
}
#endif


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
                

                
                GRAY_INT add_lucal(const GRAY_INT& rhs) {

                        bitset<N> rbits = rhs.Bits();
                        bitset<N> lbits = this->Bits();
                        // DEBUG ASSERT
                        if(this->Int()  > rhs.Int()) {
                                //                           cout <<  lbits.to_ulong() << endl;
                                //                           cout <<  rbits.to_ulong() << endl;
                                //                           assert(0);
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
                }

                GRAY_INT add_japan(const GRAY_INT& rhs) {
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


                }

                GRAY_INT add_loop (const GRAY_INT& rhs) {
                        Unsigned bit = 1;
                        for ( ; bit <= rhs && bit != 0 ; bit <<= 1) {
                                long both = lhs_p&rhs_p;
                                res ^= both;
                                lhs_p = (lhs_p ^ both ^ lhs) << 1;
                                rhs_p = ((rhs_p ^ both ^ rhs) & bit) << 1;  
                        }
                        if (0 == rhs_p)
                                return res;
                        lhs_p &= bit;
                        if (0 != lhs_p)
                                return res ^= lhs_p;
                        bit = lhs ^ (lhs & (lhs-bit));
                        return res ^ (bit << 1);
                
                }

                GRAY_INT add_recirsive ( const GRAY_INT& lhs, const GRAY_INT& rhs) {
                
                }
                GRAY_INT operator+(const GRAY_INT& rhs)  {
                }

                GRAY_INT<N> twice() const{
                        GRAY_INT<N> ret;
                        bitset<N> parity (this->isOdd()?1ul:0ul);
                        ret.SetBits(  ( 
                                      (this->Bits() << 1) | parity 
                                                                   ).to_ulong());
                        return ret;
                }

                private:
                GRAY_INT(int);
                bitset<N> m_value;

};


int main() {
  bitset<sizeof(int)*8> x,y;

  GRAY_INT<10> a, b;
  a.SetInt(1);
  for (int i = 0; i < 20; i++) {
          a++;
          int j = a.Int();
          cout << j << " " ;
          a.print();
          a.twice().print();


  }
  return 0;

}

