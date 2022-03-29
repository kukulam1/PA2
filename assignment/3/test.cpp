#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

//=================================================================================================
// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument
{
  public:
    InvalidDateException ( )
      : invalid_argument ( "invalid date or format" )
    {
    }
};
//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base & ( * date_format ( const char * fmt ) ) ( ios_base & x )
{
  return [] ( ios_base & ios ) -> ios_base & { return ios; };
}
//=================================================================================================
class CDate
{
  public:
    CDate ( int y, int m, int d)
    {
      if ( !validDate( d, m, y) )
        throw InvalidDateException();
      day = d;
      month = m;
      year = y;
    }
    CDate operator + ( int days ) const
    {
      if ( days < 0)
        return *this - -days;
      int res = dateToDays( *this ) + days;
      return daysToDate( res );
    }
    CDate operator - ( int days ) const
    {
      if ( days < 0)
        return *this + -days;
      int res = dateToDays( *this ) - days;
      return daysToDate( res );
    } 
    int operator - ( CDate & date ) const
    {
      return dateToDays( *this ) - dateToDays( date );
    }
    // prefix
    CDate & operator ++ ( void )
    {
       int res = dateToDays( *this ) + 1;
       *this = daysToDate( res );
       return *this;
    }
    CDate & operator -- ( void )
    {
       int res = dateToDays( *this ) - 1;
       *this = daysToDate( res );
       return *this;
    }
    // postfix
    CDate operator ++ ( int )
    {
      CDate date ( *this );
      int res = dateToDays( *this ) + 1;
      *this = daysToDate( res );
      return date;
    }
    CDate operator -- ( int )
    {
      CDate date ( *this );
      int res = dateToDays( *this ) - 1;
      *this = daysToDate( res );
      return date;
    }
    bool operator < ( const CDate & rhs ) const
    {
      return dateToDays( *this ) < dateToDays( rhs );
    }
    bool operator <= ( const CDate & rhs ) const
    {
      return dateToDays( *this ) <= dateToDays( rhs );
    }
    bool operator == ( const CDate & rhs ) const
    {
      return dateToDays( *this ) == dateToDays( rhs );
    }
    bool operator != ( const CDate & rhs ) const
    {
      return dateToDays( *this ) != dateToDays( rhs );
    }
    bool operator >= ( const CDate & rhs ) const
    {
      return dateToDays( *this ) >= dateToDays( rhs );
    }
    bool operator > ( const CDate & rhs ) const
    {
      return dateToDays( *this ) > dateToDays( rhs );
    }
    friend ostream & operator << ( ostream & os, const CDate & date);
    friend istream & operator >> ( istream & is, CDate & date); 

  private:
    int day, month, year;

    static bool isLeap( int year )
    {
      return  ( ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
    }
    
    static bool validDate( int d, int m, int y)
    {
      if ( y < 2000 || y > 2030 )
        return false;
      if ( m < 1 || m > 12 )
        return false;
      if ( d < 1 || d > 31 )
        return false;
      if ( m == 2 )
      {
        if ( isLeap( y ) )
          return d <= 29;
        else
          return d <= 28;
      }
      if ( m == 4 || m == 6 || m == 9 || m == 11 )
        return d <= 30;
      return true;
    }

    static int daysInMonth( int month, int year )
    {
      if ( !isLeap( year ) )
      switch ( month ) {
        case 1:
          return 31;
        case 2:
          return 28;
        case 3:
          return 31;
        case 4:
          return 30;
        case 5:
          return 31;
        case 6:
          return 30;
        case 7:
          return 31;
        case 8:
          return 31;
        case 9:
          return 30;
        case 10:
          return 31;
        case 11:
          return 30;
        case 12:
          return 31;
      } else
        switch ( month ) {
        case 1:
          return 31;
        case 2:
          return 29;
        case 3:
          return 31;
        case 4:
          return 30;
        case 5:
          return 31;
        case 6:
          return 30;
        case 7:
          return 31;
        case 8:
          return 31;
        case 9:
          return 30;
        case 10:
          return 31;
        case 11:
          return 30;
        case 12:
          return 31;      
      }
      throw InvalidDateException();
    }

    static int daysInYear( int year )
    {
      if ( !isLeap( year) )
        return 365;
      return 366;
    }

    static CDate daysToDate( int days )
    {
      int d = 0;
      int m = 1;
      int y = 1;
      while ( ( isLeap( y ) && days > 366 ) || ( !isLeap( y ) && days > 365 ) )
      {
        days -= daysInYear( y );
        y++;
      }
      while ( days > daysInMonth( m, y) )
      {
        days -= daysInMonth( m, y);
        m++;
      }
      d += days;
      return CDate( y, m, d);
    }

    static int dateToDays ( const CDate & date )
    {
      int days = 0;
      for ( int y = 1; y < date.year; y++ )
        days += daysInYear( y );
      for ( int m = 1; m < date.month; m++ )
        days += daysInMonth( m, date.year );
      return days + date.day;
    }
};

ostream & operator << ( ostream & os, const CDate & date) 
{
  return os << date.year << '-' << setfill('0') << setw(2) << date.month << '-' << setfill('0') << setw(2) << date.day;
}

istream & operator >> ( istream & is, CDate & date)
{
  int y, m , d;
  char c1, c2;
  if ( !( is >> y >> c1 >> m >> c2 >> d )
       || c1 != '-' || c2 != '-' 
       || !CDate::validDate( d, m, y) )
  {
    is.setstate( ios::failbit );
    return is;
  }
  date = CDate( y, m, d);
  return is;
} 

#ifndef __PROGTEST__
int main ( void )
{
  ostringstream oss;
  istringstream iss;

  CDate a ( 2000, 1, 2 );
  CDate b ( 2010, 2, 3 );
  CDate c ( 2004, 2, 10 );
  /*
  for ( int i = - 100; i < 10; i++ )
  {
    cout << b + i << endl;
  }
  for ( int i = 0; i < 100; i++ )
  {
    cout << b - i << endl;
  }
  */
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2000-01-02" );
  oss . str ("");
  oss << b;
  assert ( oss . str () == "2010-02-03" );
  oss . str ("");
  oss << c;
  assert ( oss . str () == "2004-02-10" );
  a = a + 1500;
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2004-02-10" );
  b = b - 2000;
  oss . str ("");
  oss << b;
  assert ( oss . str () == "2004-08-13" );
  assert ( b - a == 185 );
  assert ( ( b == a ) == false );
  assert ( ( b != a ) == true );
  assert ( ( b <= a ) == false );
  assert ( ( b < a ) == false );
  assert ( ( b >= a ) == true );
  assert ( ( b > a ) == true );
  assert ( ( c == a ) == true );
  assert ( ( c != a ) == false );
  assert ( ( c <= a ) == true );
  assert ( ( c < a ) == false );
  assert ( ( c >= a ) == true );
  assert ( ( c > a ) == false );
  a = ++c;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-11 2004-02-11" );
  a = --c;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-10 2004-02-10" );
  a = c++;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-10 2004-02-11" );
  a = c--;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-11 2004-02-10" );
  iss . clear ();
  iss . str ( "2015-09-03" );
  assert ( ( iss >> a ) );
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2015-09-03" );
  a = a + 70;
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2015-11-12" );

  CDate d ( 2000, 1, 1 );
  try
  {
    CDate e ( 2000, 32, 1 );
    assert ( "No exception thrown!" == nullptr );
  }
  catch ( ... )
  {
  }
  iss . clear ();
  iss . str ( "2000-12-33" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-01-01" );
  iss . clear ();
  iss . str ( "2000-11-31" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-01-01" );
  iss . clear ();
  iss . str ( "2000-02-29" );
  assert ( ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-02-29" );
  iss . clear ();
  iss . str ( "2001-02-29" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-02-29" );
  //-----------------------------------------------------------------------------
  // bonus test examples
  //-----------------------------------------------------------------------------
  /*
  CDate f ( 2000, 5, 12 );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  oss . str ("");
  oss << date_format ( "%Y/%m/%d" ) << f;
  assert ( oss . str () == "2000/05/12" );
  oss . str ("");
  oss << date_format ( "%d.%m.%Y" ) << f;
  assert ( oss . str () == "12.05.2000" );
  oss . str ("");
  oss << date_format ( "%m/%d/%Y" ) << f;
  assert ( oss . str () == "05/12/2000" );
  oss . str ("");
  oss << date_format ( "%Y%m%d" ) << f;
  assert ( oss . str () == "20000512" );
  oss . str ("");
  oss << date_format ( "hello kitty" ) << f;
  assert ( oss . str () == "hello kitty" );
  oss . str ("");
  oss << date_format ( "%d%d%d%d%d%d%m%m%m%Y%Y%Y%%%%%%%%%%" ) << f;
  assert ( oss . str () == "121212121212050505200020002000%%%%%" );
  oss . str ("");
  oss << date_format ( "%Y-%m-%d" ) << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-01-1" );
  assert ( ! ( iss >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-1-01" );
  assert ( ! ( iss >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-001-01" );
  assert ( ! ( iss >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-01-02" );
  assert ( ( iss >> date_format ( "%Y-%m-%d" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2001-01-02" );
  iss . clear ();
  iss . str ( "05.06.2003" );
  assert ( ( iss >> date_format ( "%d.%m.%Y" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2003-06-05" );
  iss . clear ();
  iss . str ( "07/08/2004" );
  assert ( ( iss >> date_format ( "%m/%d/%Y" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2004-07-08" );
  iss . clear ();
  iss . str ( "2002*03*04" );
  assert ( ( iss >> date_format ( "%Y*%m*%d" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2002-03-04" );
  iss . clear ();
  iss . str ( "C++09format10PA22006rulez" );
  assert ( ( iss >> date_format ( "C++%mformat%dPA2%Yrulez" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2006-09-10" );
  iss . clear ();
  iss . str ( "%12%13%2010%" );
  assert ( ( iss >> date_format ( "%%%m%%%d%%%Y%%" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2010-12-13" );

  CDate g ( 2000, 6, 8 );
  iss . clear ();
  iss . str ( "2001-11-33" );
  assert ( ! ( iss >> date_format ( "%Y-%m-%d" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "29.02.2003" );
  assert ( ! ( iss >> date_format ( "%d.%m.%Y" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "14/02/2004" );
  assert ( ! ( iss >> date_format ( "%m/%d/%Y" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "2002-03" );
  assert ( ! ( iss >> date_format ( "%Y-%m" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "hello kitty" );
  assert ( ! ( iss >> date_format ( "hello kitty" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "2005-07-12-07" );
  assert ( ! ( iss >> date_format ( "%Y-%m-%d-%m" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "20000101" );
  assert ( ( iss >> date_format ( "%Y%m%d" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-01-01" );
  */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
