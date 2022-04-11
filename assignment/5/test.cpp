#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDate
{
  public:
    CDate ( int y, int m, int d ) 
    : year ( y ), month ( m ), day ( d )
    {}
    bool operator == ( const CDate & rhs ) const 
    {
      return year == rhs.year && month == rhs.month && day == rhs.day;
    }
    bool operator < ( const CDate & rhs ) const 
    {
      if ( year < rhs.year )
        return true;
      if ( year == rhs.year )
      {
        if ( month < rhs.month )
          return true;
        if ( month == rhs.month )
          return day < rhs.day;        
      }
      return false;
    }

  private:
    int year, month, day;
};

class CSupermarket
{
  public:
    CSupermarket ()
    {}
    /**
     * @brief 
     * Store product in stock
     * 
     * @param name 
     * @param exp_date 
     * @param count 
     * @return CSupermarket& 
     */
    CSupermarket & store ( const string & name, const CDate & exp_date, const int count )
    {
      // product not in stock yet
      if ( stock.find( name ) == stock.end() )
      {
        vector<pair<CDate,int>> date_count;
        date_count.push_back( make_pair( exp_date, count ));
        stock.emplace( name, date_count );
        return *this;
      }
      // product already in stock
      for ( auto it = stock[ name ].begin(); it != stock[ name ].end(); ++it )
      {
        // same expire date -> just increase count
        if ( exp_date == (*it).first )
        {
          (*it).second += count;
          return *this;
        }
      }
      // differenct expiry date
      // sorted vector by expiry date
      pair<CDate,int> n_pair = make_pair( exp_date, count);
      auto it = upper_bound( stock[ name ].begin(), stock[ name ].end(), n_pair,
            [] ( const pair<CDate,int> & product1, const pair<CDate,int> & product2 )
            {
              return product1.first < product2.first;                            
            });
      stock[ name ].emplace( it, n_pair);
      return *this;
    }

    list<pair<string,int>> sell ( list<pair<string,int>> & shoppingList ) 
    {
      // vector of elements that are string that matches actual names for typos and "" for items not in stock
      vector<string> to_sell;
      // find products and match typos with their actual name
      for ( auto it = shoppingList.begin(); it != shoppingList.end(); ++it )
        to_sell.push_back( findProduct( it->first ) );
      
      auto shop_it = shoppingList.begin();
      auto sell_it = to_sell.begin();
      //iterate through shop_list and sell items, delete from list if sold all
      while ( shop_it != shoppingList.end() )
      {
        if ( productSold( (*sell_it), shop_it->second ) )
        {
          shop_it = shoppingList.erase( shop_it );
        } else
            ++shop_it;
        ++sell_it;          
      }
      return shoppingList;
    }

    list<pair<string,int>> expired ( const CDate & date ) const 
    {
      list<pair<string,int>> exp;
      // iterate through stock and if item is expired add it to list exp
      for ( auto it = stock.begin(); it != stock.end(); ++it )
      {
        //count of expired of 1 product 
        size_t count = 0;
        for ( auto it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2 )
        {
          if ( (*it2).first < date )
            count += (*it2).second; 
        }
        if ( count > 0 )
          exp.emplace_back( (*it).first, count );
      }
      //sort exp list from highest number of items ( O(N*log(N)))
      exp.sort( [] ( const pair<string,int> & product1, const pair<string,int> & product2 )
              {
                return product1.second > product2.second;
              });

      return exp;
    }

  private:
    /*
    database of our products available
    first -> name, second -> sorted vector < pair ( expiry date, amount ) >
    vector sorted by expiry date from oldest to newest  
    */
    map<string,vector<pair<CDate,int>>> stock;

    /**
     * @brief 
     * Sell product
     * @param product 
     * @return true if sold amount demanded
     * @return false otherwise
     */
    bool productSold ( const string & product, int & amount )
    {
      //product not in stock
      if ( product == "" )
        return false;
      //iterate through product vector
      auto it = stock[ product ].begin();
      while ( it != stock[ product ].end() )
      {
        //have more or same amount in stock as selling
        if ( it->second >= amount )
        {
          it->second -= amount;
          amount = 0;
          return true;
        }
        //sell all product with exact exp_date
        amount -= it->second;
        it = stock[ product ].erase( it );                        
      }
      //if out of product delete it
      if ( stock[ product ].empty() )
        stock.erase( product );     
      return false;
    }

    /**
     * @brief 
     * Find product
     * @param product 
     * @return true if product found
     * @return false otherwise
     */
    const string & findProduct ( const string & product ) const
    {
      //product in stock
      if ( stock.find( product ) != stock.end() )
        return product;
      //product not in stock
      return findTypo ( product );
    }

    /**
     * @brief 
     * Find product containing typo
     * @param product 
     * @return true if typo found
     * @return false otherwise
     */
    const string & findTypo ( const string & product ) const
    {
      static const string empty = "";
      auto pos = stock.end();
      for ( auto it = stock.begin(); it != stock.end(); ++it )
      {
        if ( isTypo( product, (*it).first ))
        {
          // first typo -> save position
          if ( pos == stock.end() )
            pos = it;
          // 2 or more products with typo -> cant choose 
          else 
            return empty;
        }
      }
      if ( pos == stock.end() )
        return empty;
      return pos->first;
    }

    /**
     * @brief 
     * Check if there is a 1 different char in 2 strings, order matters
     * @param a 
     * @param b 
     * @return true there is exactly 1 differenct char 
     * @return false different length or 0 or more than 1 differenct chars
     */
    bool isTypo ( const string & a, const string & b ) const
    {
      if ( a.size() != b.size() )
        return false;
      bool found = false;
      for ( size_t i = 0; i < a.size(); ++i )
      {
        if ( a[ i ] != b[ i ] )
        {
          if ( found )
            return false;
          else
            found = true;           
        }
      }
      return found;
    }
};

#ifndef __PROGTEST__
int main ( void )
{
  CSupermarket s;
  s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
    . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
    . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
    . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
    . store ( "okey", CDate ( 2016, 7, 18 ), 5 );


  list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
  assert ( l0 . size () == 4 );
  assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );

  list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
  s . sell ( l1 );
  assert ( l1 . size () == 2 );
  assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );

  list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
  assert ( l2 . size () == 1 );
  assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );

  list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
  assert ( l3 . size () == 1 );
  assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );

  list<pair<string,int> > l4 { { "bread", 105 } };
  s . sell ( l4 );
  assert ( l4 . size () == 0 );
  assert ( ( l4 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l5 . size () == 3 );
  assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );

  s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );

  list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
  s . sell ( l6 );
  assert ( l6 . size () == 3 );
  assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );

  list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l7 . size () == 4 );
  assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );

  s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );

  list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
  s . sell ( l8 );
  assert ( l8 . size () == 2 );
  assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );

  list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l9 . size () == 5 );
  assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );

  list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
  s . sell ( l10 );
  assert ( l10 . size () == 2 );
  assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );

  list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l11 . size () == 4 );
  assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );

  list<pair<string,int> > l12 { { "Cake", 4 } };
  s . sell ( l12 );
  assert ( l12 . size () == 0 );
  assert ( ( l12 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l13 . size () == 4 );
  assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );

  list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
  s . sell ( l14 );
  assert ( l14 . size () == 1 );
  assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );

  s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
    . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
    . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );

  list<pair<string,int> > l15 { { "ccccc", 10 } };
  s . sell ( l15 );
  assert ( l15 . size () == 1 );
  assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
