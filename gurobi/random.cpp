

#include<iostream>
#include<cstdlib>

using namespace std;

int main()
{
   int n, max, num, c;

   //cout << "Enter the number of random numbers you want ";
   //cin >> n;
   //cout << "Enter the maximum value of random number ";
   //cin >> max;

   //cout << "random numbers from 0 to " << max << " are :-" << endl;


	srand(static_cast<unsigned int>(time(0)));
   for ( c = 1 ; c <= 10 ; c++ )
   {
      num = random()%10;
      cout << num << endl;
   }

   return 0;
}
