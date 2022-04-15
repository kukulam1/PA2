#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDataType
{
  public:
    CDataType () {}
    virtual ~CDataType ( void ) noexcept = default;
    virtual int  getSize()                         const = 0;
    virtual bool operator == ( const CDataType & ) const = 0;
    virtual bool operator != ( const CDataType & ) const = 0;
    virtual CDataType * clone ()                   const = 0;
    virtual ostream & print ( ostream & os )       const = 0;
};

ostream & operator << ( ostream & os, const CDataType & data )
{
  return data.print( os );  
}

class CDataTypeInt : public CDataType
{
  public:
    CDataTypeInt() {}
    int  getSize()                              const override { return 4; }
    bool operator == ( const CDataType & rhs )  const override
    { return dynamic_cast<const CDataTypeInt*>(&rhs) != nullptr; }
    bool operator != ( const CDataType & rhs )  const override
    { return dynamic_cast<const CDataTypeInt*>(&rhs) == nullptr; }
    CDataTypeInt * clone ()                     const override 
    { return new CDataTypeInt( *this ); }
    ostream & print ( ostream & os )            const override
    { return os << "int ";}
  private:
    int m_X;
};
class CDataTypeDouble : public CDataType
{
  public:
    CDataTypeDouble() {}
    int  getSize()                             const override { return 8; }
    bool operator == ( const CDataType & rhs ) const override 
    { return dynamic_cast<const CDataTypeDouble*>(&rhs) != nullptr; }
    bool operator != ( const CDataType & rhs ) const override 
    { return dynamic_cast<const CDataTypeDouble*>(&rhs) == nullptr; }
    CDataTypeDouble * clone ()                 const override 
    { return new CDataTypeDouble( *this ); }
    ostream & print ( ostream & os )           const override
    { return os << "double "; }
  private:
    double m_X;
};
class CDataTypeEnum : public CDataType
{
  public:
    CDataTypeEnum() {}
    CDataTypeEnum( const CDataTypeEnum & src )
    {
      if ( &src == this )
        return;
      for ( size_t i = 0; i < src.m_Enum.size(); ++i )
        m_Enum.push_back( src.m_Enum.at( i ) );     
    }
    int  getSize()                             const override { return 4; }
    CDataTypeEnum & add ( const string & s );
    bool operator == ( const CDataType & rhs ) const override 
    { 
      const CDataTypeEnum * tmp = dynamic_cast<const CDataTypeEnum*>(&rhs);
      if ( tmp == nullptr )
        return false;
      if ( tmp->m_Enum.size() != m_Enum.size() )
        return false;
      for ( size_t i = 0; i < m_Enum.size(); ++i )
        if ( m_Enum.at( i ) != tmp->m_Enum.at( i ) )
               return false;
      return true;
    }
    bool operator != ( const CDataType & rhs ) const override 
    { return !(*this == rhs); }
    CDataTypeEnum * clone ()                   const override 
    { return new CDataTypeEnum( *this ); }
    ostream & print ( ostream & os )           const override 
    {
      os << "enum" << endl << "  " << '{' << endl;
      for ( size_t i = 0; i < m_Enum.size() - 1; ++i )
        os << "  " << m_Enum[ i ] << ',' << endl;
      os << "  " << m_Enum[ m_Enum.size() - 1 ] <<  endl;
      return os << "  } ";
    }
  private:
    vector<string> m_Enum;
};
class CDataTypeStruct : public CDataType
{
  public:
    CDataTypeStruct() {}
    CDataTypeStruct( const CDataTypeStruct & src ) 
    {
      if ( &src == this )
        return;
      for ( size_t i = 0; i < src.m_Struct.size(); ++i )
        m_Struct.push_back( src.m_Struct.at( i ) );     
    }
    CDataTypeStruct & operator = ( CDataTypeStruct src )
    {
      std::swap( m_Struct, src.m_Struct);
      return *this;
    } 
    int  getSize()                             const override 
    {
      size_t res = 0;
      for ( size_t i = 0; i < m_Struct.size(); ++i )
        res += m_Struct.at( i ).data->getSize();
      return res;       
    }
    CDataTypeStruct & addField ( const string & name,const CDataType & type );
    const CDataType & field ( const string & name ) const;
    bool operator == ( const CDataType & rhs ) const override
    {
      const CDataTypeStruct * tmp = dynamic_cast<const CDataTypeStruct*>(&rhs);
      if ( tmp == nullptr )
        return false;
      if ( tmp->m_Struct.size() != m_Struct.size() )
        return false;
      for ( size_t i = 0; i < m_Struct.size(); ++i )
        if ( *(m_Struct.at( i ).data) != *(tmp->m_Struct.at( i ).data) )
               return false;
      return true;
    }
    bool operator != ( const CDataType & rhs ) const override 
    { return !(*this == rhs); }
    CDataTypeStruct * clone ()                   const override 
    { return new CDataTypeStruct( *this ); }
    ostream & print ( ostream & os )           const override 
    {
      os << "struct" << endl << '{' << endl;
      for ( size_t i = 0; i < m_Struct.size(); ++i )
      {
        os << "  ";
        m_Struct[ i ].data->print( os );
        os << m_Struct[ i ].name <<  ';' << endl;
      }
      return os << '}' << endl;
    }
  private:
    class CValue {
      public:
      string name;
      const CDataType * data;
      CValue ( const string & nm, const CDataType & type )
      :name ( nm )
      {
        data = type.clone();
      }
      CValue ( const CValue & src )
      {
        if ( &src == this )
          return;
        name = src.name;
        data = src.data->clone();                
      }
      CValue & operator = ( CValue src )
      {
        std::swap( name, src.name );
        std::swap( data, src.data );
        return *this;
      }
      ~CValue ()
      {
        delete data;
      }

    };    
    vector<CValue> m_Struct;
};

CDataTypeEnum & CDataTypeEnum::add ( const string & s )
{ 
  for ( size_t i = 0; i < m_Enum.size(); ++i )
    if ( m_Enum[ i ] == s )
      throw invalid_argument( "Duplicate enum value: " + s ); 
  m_Enum.push_back( s );
  return *this;  
}

CDataTypeStruct & CDataTypeStruct::addField ( const string & name, const CDataType & type )
{
  for ( size_t i = 0; i < m_Struct.size(); ++i )
    if ( m_Struct[ i ].name == name )
      throw invalid_argument( "Duplicate field: " + name );
  m_Struct.push_back( CValue{ name, type } );
  return *this; 
}
const CDataType & CDataTypeStruct::field ( const string & name ) const
{
  for ( size_t i = 0; i < m_Struct.size(); ++i )
    if ( m_Struct[ i ].name == name )
      return *m_Struct[ i ].data;  
  throw invalid_argument( "Unknown field: " + name );
}


#ifndef __PROGTEST__
static bool        whitespaceMatch                         ( const string    & a,
                                                             const string    & b )
{
  return true;
}
template <typename T_>
static bool        whitespaceMatch                         ( const T_        & x,
                                                             const string    & ref )
{
  ostringstream oss;
  oss << x;
  return whitespaceMatch ( oss . str (), ref );
}
int main ( void )
{

  CDataTypeStruct  a = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
  CDataTypeStruct b = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "READY" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
  CDataTypeStruct c =  CDataTypeStruct () .
                        addField ( "m_First", CDataTypeInt () ) .
                        addField ( "m_Second", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Third", CDataTypeDouble () );

  CDataTypeStruct  d = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeInt () );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );
    /*
    a.print( cout );
    b.print( cout );
    c.print( cout );
    d.print( cout );
    */
  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );
  assert ( whitespaceMatch ( c, "struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "}") );

  assert ( whitespaceMatch ( d, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  int m_Ratio;\n"
    "}") );

  assert ( a != b );
  assert ( a == c );
  assert ( a != d );
  assert ( a . field ( "m_Status" ) == CDataTypeEnum () . add ( "NEW" ) . add ( "FIXED" ) . add ( "BROKEN" ) . add ( "DEAD" ) );
  assert ( a . field ( "m_Status" ) != CDataTypeEnum () . add ( "NEW" ) . add ( "BROKEN" ) . add ( "FIXED" ) . add ( "DEAD" ) );
  assert ( a != CDataTypeInt() );
  assert ( whitespaceMatch ( a . field ( "m_Status" ), "enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  DEAD\n"
    "}") );

  CDataTypeStruct aOld = a;
  b . addField ( "m_Other", CDataTypeDouble ());

  a . addField ( "m_Sum", CDataTypeInt ());

  assert ( a != aOld );
  assert ( a != c );
  assert ( aOld == c );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  int m_Sum;\n"
    "}") );

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  double m_Other;\n"
    "}") );

  c . addField ( "m_Another", a . field ( "m_Status" ));

  assert ( whitespaceMatch ( c, "struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Another;\n"
    "}") );

  d . addField ( "m_Another", a . field ( "m_Ratio" ));

  assert ( whitespaceMatch ( d, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  int m_Ratio;\n"
    "  double m_Another;\n"
    "}") );

  assert ( a . getSize () == 20 );
  assert ( b . getSize () == 24 );
/*
  try
  {
    a . addField ( "m_Status", CDataTypeInt () );
    assert ( "addField: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( strcmp (e . what (), "Duplicate field: m_Status") );
  }

  try
  {
    cout << a . field ( "m_Fail" ) << endl;
    assert ( "field: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( strcmp (e . what (),"Unknown field: m_Fail" ));
  }

  try
  {
    CDataTypeEnum en;
    en . add ( "FIRST" ) .
         add ( "SECOND" ) .
         add ( "FIRST" );
    assert ( "add: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( strcmp (e . what (),"Duplicate enum value: FIRST" ));
  }
*/
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
