#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

class CTimeTester;
#endif /* __PROGTEST__ */

class CTime
{
private:
    int m_Hour;
    int m_Minute;
    int m_Second;

    int toSeconds() const { return 3600 * m_Hour + 60 * m_Minute + m_Second; }
    int secondsToDay ( int sec ) const { return sec % 86400; }
    int getHours( int sec ) const { return (sec / 3600); }
    int getMinutes( int sec ) const { sec = sec % 3600; return (sec / 60); }
    int getSeconds( int sec ) const { return sec % 60; }

public:
    // constructor, destructor

    CTime ()
    {
    }

    CTime ( int hour, int minute, int second = 0 ) 
    {
        if ( hour < 0 || hour >= 24 || minute < 0 || minute >= 60 || second < 0 || second >= 60 )
        {
            throw std::invalid_argument("Wrong time format");
        }
        m_Hour = hour;
        m_Minute = minute;
        m_Second = second;
    }
    // arithmetic operators
    CTime operator + ( int sec ) const
    {
        sec += toSeconds();
        sec = secondsToDay( sec );
        return CTime( getHours( sec ), getMinutes( sec ), getSeconds( sec ));          
    }

    CTime & operator += ( int sec ) 
    {
        sec += toSeconds();
        sec = secondsToDay( sec );
        m_Hour = getHours( sec );
        m_Minute = getMinutes( sec );
        m_Second = getSeconds( sec );
        return *this;          
    }

    CTime operator - ( int sec ) const
    {
        sec = toSeconds() - sec ;
        sec = secondsToDay( sec );
        return CTime( getHours( sec ), getMinutes( sec ), getSeconds( sec ));          
    }

    CTime & operator -= ( int sec ) 
    {
        sec = toSeconds() - sec ;
        sec = secondsToDay( sec );
        m_Hour = getHours( sec );
        m_Minute = getMinutes( sec );
        m_Second = getSeconds( sec );
        return *this;          
    }

    int operator - ( const CTime rhs ) const
    {
        return abs ( toSeconds() - rhs.toSeconds() );
    }

    CTime & operator ++ ( void )
    {
        int sec = toSeconds() + 1;
        sec = secondsToDay( sec );
        m_Hour = getHours( sec );
        m_Minute = getMinutes( sec );
        m_Second = getSeconds( sec );
        return *this;
    }

    CTime operator ++ ( int )
    {
        CTime res ( *this );
        int sec = toSeconds() + 1;
        sec = secondsToDay( sec );
        m_Hour = getHours( sec );
        m_Minute = getMinutes( sec );
        m_Second = getSeconds( sec );
        return res;
    }

    CTime & operator -- ( void )
    {
        int sec = toSeconds() - 1;
        sec = secondsToDay( sec );
        m_Hour = getHours( sec );
        m_Minute = getMinutes( sec );
        m_Second = getSeconds( sec );
        return *this;
    }

    CTime operator -- ( int )
    {
        CTime res ( *this );
        int sec = toSeconds() - 1;
        sec = secondsToDay( sec );
        m_Hour = getHours( sec );
        m_Minute = getMinutes( sec );
        m_Second = getSeconds( sec );
        return res;
    }

    // comparison operators
    bool operator < ( const CTime rhs ) const 
    {
        return toSeconds() < rhs.toSeconds();
    }
    bool operator <= ( const CTime rhs ) const 
    {
        return toSeconds() <= rhs.toSeconds();
    }
    bool operator > ( const CTime rhs ) const 
    {
        return toSeconds() > rhs.toSeconds();
    }
    bool operator >= ( const CTime rhs ) const 
    {
        return toSeconds() >= rhs.toSeconds();
    }
    bool operator == ( const CTime rhs ) const 
    {
        return toSeconds() == rhs.toSeconds();
    }
    bool operator != ( const CTime rhs ) const 
    {
        return toSeconds() != rhs.toSeconds();
    }

    // output operator

    friend ostream & operator << ( ostream & os, const CTime & time );
    friend class ::CTimeTester;
};

    ostream & operator << ( ostream & os, const CTime & time )
    {
        return ( os << setw(2) << setfill(' ') << time.m_Hour << ':'
                    << setw(2) << setfill('0') << time.m_Minute << ':'
                    << setw(2) << setfill('0') << time.m_Second );      
    }

#ifndef __PROGTEST__
struct CTimeTester
{
    static bool test ( const CTime & time, int hour, int minute, int second )
    {
        return time.m_Hour == hour 
            && time.m_Minute == minute
            && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main ()
{
    CTime a { 12, 30 };
    assert( CTimeTester::test( a, 12, 30, 0 ) );

    CTime b { 13, 30 };
    assert( CTimeTester::test( b, 13, 30, 0 ) );

    assert( b - a == 3600 );
    
    assert( CTimeTester::test( a + 60, 12, 31, 0 ) );
    assert( CTimeTester::test( a - 60, 12, 29, 0 ) );

    assert( a < b );
    assert( a <= b );
    assert( a != b );
    assert( !( a > b ) );
    assert( !( a >= b ) );
    assert( !( a == b ) );

    while ( ++a != b );
    assert( a == b );

    std::ostringstream output;
    assert( static_cast<bool>( output << a ) );
    assert( output.str() == "13:30:00" );

    assert( a++ == b++ );
    assert( a == b );

    assert( --a == --b );
    assert( a == b );

    assert( a-- == b-- );
    assert( a == b );

    return 0;
}
#endif /* __PROGTEST__ */
