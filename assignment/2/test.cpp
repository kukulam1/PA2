#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CVATRegister
{
  public:
                  CVATRegister   ( void );
                  ~CVATRegister  ( void );
    bool          newCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID );
    bool          cancelCompany  ( const string    & name,
                                   const string    & addr );
    bool          cancelCompany  ( const string    & taxID );
    bool          invoice        ( const string    & taxID,
                                   unsigned int      amount );
    bool          invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount );
    bool          audit          ( const string    & name,
                                   const string    & addr,
                                   unsigned int    & sumIncome ) const;
    bool          audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const;
    bool          firstCompany   ( string          & name,
                                   string          & addr ) const;
    bool          nextCompany    ( string          & name,
                                   string          & addr ) const;
    unsigned int  medianInvoice  ( void ) const;
  private:
    struct company {
      string name;
      string addr;
      string taxID;
      unsigned int income;

      company ( const string & name, const string & addr, const string & taxID = "") 
              : name ( name ), addr ( addr ), taxID ( taxID ), income ( 0 )
      {}
      company ( const string & taxID ) 
              : taxID ( taxID ), income ( 0 )
      {}
    };

    vector<company> na_data;
    vector<company> id_data;
    vector<unsigned int> invoices;

    int findCompany ( const string & name, const string & addr) const;
    int findCompany ( const string & taxID) const;
    int binSearch ( int l, int r, const string & name, const string & addr) const;
    int binSearch ( int l, int r, const string & taxID) const;
    void na_insert ( const string & name, const string & addr, const string & taxID );
    void id_insert ( const string & name, const string & addr, const string & taxID );
    static string toLower ( const string & s1 );
    static bool caseInsCmp( const string & s1, const string & s2);  
    static bool cmpID ( const company & c1, const company & c2 )
    {
      return c1.taxID < c2.taxID;
    }
    static bool cmpNA ( const company & c1, const company & c2 )
    {
      if ( toLower( c1.name ) == toLower( c2.name ) )
        return toLower( c1.addr ) < toLower( c2.addr );
      return toLower( c1.name ) < toLower( c2.name );
    }
};

CVATRegister::CVATRegister ( void )
{
}
CVATRegister::~CVATRegister ( void )
{
  id_data.clear();
  na_data.clear();
  invoices.clear();
}
bool CVATRegister::newCompany ( const string    & name, const string    & addr, const string    & taxID ) 
{
  if ( findCompany( taxID ) >= 0 ||  findCompany( name, addr) >= 0 )
    return false;
  na_insert( name, addr, taxID);
  id_insert( name, addr, taxID);
  return true;
}
bool CVATRegister::cancelCompany  ( const string    & name, const string    & addr )
{
  int pos = findCompany( name, addr);
  if ( pos < 0 )
    return false;
  const string taxID = na_data.at( pos ).taxID;  
  na_data.erase( na_data.begin() + pos );
  pos = findCompany( taxID );
  id_data.erase( id_data.begin() + pos ); 
  return true;
}
bool CVATRegister::cancelCompany  ( const string & taxID )
{
  int pos = findCompany( taxID );
  if ( pos < 0 )
    return false;
  const string name = id_data.at( pos ).name;
  const string addr = id_data.at( pos ).addr;  
  id_data.erase( id_data.begin() + pos );
  pos = findCompany( name, addr);
  na_data.erase( na_data.begin() + pos );
  return true;
}
bool CVATRegister::invoice ( const string    & taxID, unsigned int      amount ) 
{
    int pos = findCompany( taxID );
    if ( pos < 0 )
      return false;
    id_data.at( pos ).income += amount;
    string name = id_data.at( pos ).name;
    string addr = id_data.at( pos ).addr;
    pos = findCompany( name, addr);
    na_data.at( pos ).income += amount;
    invoices.push_back( amount );
    return true;
}
bool CVATRegister::invoice ( const string    & name, const string    & addr, unsigned int      amount )
{
  int pos = findCompany( name, addr);
  if ( pos < 0 )
    return false;
  string taxID = na_data.at( pos ).taxID;
  na_data.at( pos ).income += amount;
  pos = findCompany( taxID );
  id_data.at( pos ).income += amount;
  invoices.push_back( amount );
  return true;
}
bool CVATRegister::audit ( const string    & name, const string    & addr, unsigned int    & sumIncome ) const 
{
  int pos = findCompany( name, addr);
  if ( pos < 0 )
    return false;
  sumIncome = na_data.at( pos ).income;
  return true;
}
bool CVATRegister::audit ( const string    & taxID, unsigned int    & sumIncome ) const 
{
  int pos = findCompany( taxID );
  if ( pos < 0 )
    return false;
  sumIncome = id_data.at( pos ).income;
  return true;
}
bool CVATRegister::firstCompany ( string & name, string & addr ) const 
{
    if ( na_data.empty() )
      return false;
    name = na_data.front().name;
    addr = na_data.front().addr;
    return true;
}
bool CVATRegister::nextCompany ( string & name, string & addr ) const 
{
  int pos = findCompany( name, addr);
  if ( pos < 0 || (int)na_data.size() <= pos + 1 )
    return false;
  name = na_data.at( pos + 1 ).name;
  addr = na_data.at( pos + 1 ).addr;
  return true;
}
unsigned int  CVATRegister::medianInvoice  ( void ) const  
{
    if ( invoices.empty() )
      return 0;
    vector<int> sorted;
    for ( size_t i = 0; i < invoices.size(); i++)
      sorted.push_back( invoices[i] );
    sort( sorted.begin(), sorted.end());
    return sorted[ sorted.size() / 2 ];
}

int CVATRegister::findCompany ( const string & name, const string & addr ) const
{
    auto it = lower_bound( na_data.begin(), na_data.end(), company( name, addr ), &cmpNA );
    if ( it == na_data.end() )
      return -1;
    int pos = it - na_data.begin(); 
    if ( caseInsCmp( na_data[ pos ].name, name ) && caseInsCmp( na_data[ pos ].addr, addr ))
      return pos;
    return -1;
}
int CVATRegister::findCompany ( const string & taxID ) const
{
    auto it = lower_bound( id_data.begin(), id_data.end(), company( taxID ), &cmpID );
    if ( it == id_data.end() )
      return -1;
    int pos = it - id_data.begin();
    if ( id_data[ pos ].taxID  == taxID )
      return pos;
    return -1;
}

void CVATRegister::na_insert ( const string & name, const string & addr, const string & taxID )
{
    auto it = lower_bound( na_data.begin(), na_data.end(), company( name, addr ), &cmpNA );
    if ( it == na_data.end() )
      na_data.push_back( company( name, addr, taxID ));
    else
      na_data.insert( it, company( name, addr, taxID));
}
void CVATRegister::id_insert ( const string & name, const string & addr, const string & taxID )
{
    auto it = lower_bound( id_data.begin(), id_data.end(), company( taxID ), &cmpID );
    if ( it == id_data.end() )
      id_data.push_back( company( name, addr, taxID ));
    else
      id_data.insert( it, company( name, addr, taxID));
}

string CVATRegister::toLower( const string & s1 )
{
    string s;
    for ( size_t i = 0; i < s1.length(); i++)
      s.push_back( tolower( s1[i] ) );
    return s;
}
bool CVATRegister::caseInsCmp( const string & s1, const string & s2 )
{
    if ( s1.length() != s2.length() )
       return false;
    for ( size_t i = 0; i < s1.length(); i++ )
      if ( tolower( s1.at( i )) != tolower( s2.at( i )) )
        return false;
    return true;
}


#ifndef __PROGTEST__
int               main           ( void )
{
  string name, addr;
  unsigned int sumIncome;

  CVATRegister b1;
  assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
  assert ( b1 . invoice ( "666/666", 2000 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "666/666/666", 3000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 4000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
  assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
  assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
  assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . cancelCompany ( "666/666" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . invoice ( "123456", 100 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 300 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 230 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 830 ) );
  assert ( b1 . medianInvoice () == 830 );
  assert ( b1 . invoice ( "123456", 1830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 3200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "123456" ) );
  assert ( ! b1 . firstCompany ( name, addr ) );

  CVATRegister b2;
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
  assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
  assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
  assert ( b2 . medianInvoice () == 0 );
  assert ( b2 . invoice ( "ABCDEF", 1000 ) );
  assert ( b2 . medianInvoice () == 1000 );
  assert ( b2 . invoice ( "abcdef", 2000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . invoice ( "1234567", 100 ) );
  assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
  assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
  assert ( ! b2 . audit ( "1234567", sumIncome ) );
  assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
  assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
  assert ( ! b2 . cancelCompany ( "1234567" ) );
  assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
  assert ( b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
