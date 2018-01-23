#include<bitset>
#include<iostream>

using namespace std;

unsigned int INT_TO_GRAY (int X){
        return (unsigned int)(X^((unsigned int)X >> 1));
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
                const bitset<N>&    Bits() 
                                        {return m_value;};
                int& Int();

                bool                isOdd() const 
                                        { return Bits().to_ulong() % 2; }       
                bool                isEven() const 
                                        { return !isOdd(); };

                
                private:
                GRAY_INT(int);
                bitset<N> m_value;

};
template<size_t N>
unsigned int GRAY_TO_INT( GRAY_INT<N> X ){
        unsigned int a = X.Bits().to_ulong();
        unsigned int digit = 1;
        unsigned int tmp;
        while (tmp = (a >> digit)){
                a ^= tmp;
                digit <<= 1;
        }
        return a;

}

int main() {
  GRAY_INT<10> a;
  for (int i = 0; i < 10; i++) {
          a.SetInt(i);
          int j = GRAY_TO_INT(a);
          cout << j << " " ;
          a.print();

  }
  return 0;

}


std::string g(std::string s) {
        int i,z;
        if(s=="1") return"11";
        for(i=z=0;i<s.length();i++)
                if(s[i]=='1') 
                        z++;
        i--;
        if(z%2)
        {
                char c=s[i];
                s.erase(i);
                s=g(s);
                s+=c;
        }   else    {
                s[i]=s[i]==49?48:49;
        }
        return s;
}
