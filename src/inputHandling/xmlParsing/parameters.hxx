// Copyright (c) 2005-2014 Code Synthesis Tools CC
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// Furthermore, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

#ifndef PARAMETERS_HXX
#define PARAMETERS_HXX

#ifndef XSD_CXX11
#define XSD_CXX11
#endif

#ifndef XSD_USE_CHAR
#define XSD_USE_CHAR
#endif

#ifndef XSD_CXX_TREE_USE_CHAR
#define XSD_CXX_TREE_USE_CHAR
#endif

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/config.hxx>

#if (XSD_INT_VERSION != 4000000L)
#error XSD runtime version mismatch
#endif

#include <xsd/cxx/pre.hxx>

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/types.hxx>

#include <xsd/cxx/xml/error-handler.hxx>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

#include <xsd/cxx/tree/parsing.hxx>
#include <xsd/cxx/tree/parsing/byte.hxx>
#include <xsd/cxx/tree/parsing/unsigned-byte.hxx>
#include <xsd/cxx/tree/parsing/short.hxx>
#include <xsd/cxx/tree/parsing/unsigned-short.hxx>
#include <xsd/cxx/tree/parsing/int.hxx>
#include <xsd/cxx/tree/parsing/unsigned-int.hxx>
#include <xsd/cxx/tree/parsing/long.hxx>
#include <xsd/cxx/tree/parsing/unsigned-long.hxx>
#include <xsd/cxx/tree/parsing/boolean.hxx>
#include <xsd/cxx/tree/parsing/float.hxx>
#include <xsd/cxx/tree/parsing/double.hxx>
#include <xsd/cxx/tree/parsing/decimal.hxx>

namespace xml_schema
{
  // anyType and anySimpleType.
  //
  typedef ::xsd::cxx::tree::type type;
  typedef ::xsd::cxx::tree::simple_type< char, type > simple_type;
  typedef ::xsd::cxx::tree::type container;

  // 8-bit
  //
  typedef signed char byte;
  typedef unsigned char unsigned_byte;

  // 16-bit
  //
  typedef short short_;
  typedef unsigned short unsigned_short;

  // 32-bit
  //
  typedef int int_;
  typedef unsigned int unsigned_int;

  // 64-bit
  //
  typedef long long long_;
  typedef unsigned long long unsigned_long;

  // Supposed to be arbitrary-length integral types.
  //
  typedef long long integer;
  typedef long long non_positive_integer;
  typedef unsigned long long non_negative_integer;
  typedef unsigned long long positive_integer;
  typedef long long negative_integer;

  // Boolean.
  //
  typedef bool boolean;

  // Floating-point types.
  //
  typedef float float_;
  typedef double double_;
  typedef double decimal;

  // String types.
  //
  typedef ::xsd::cxx::tree::string< char, simple_type > string;
  typedef ::xsd::cxx::tree::normalized_string< char, string > normalized_string;
  typedef ::xsd::cxx::tree::token< char, normalized_string > token;
  typedef ::xsd::cxx::tree::name< char, token > name;
  typedef ::xsd::cxx::tree::nmtoken< char, token > nmtoken;
  typedef ::xsd::cxx::tree::nmtokens< char, simple_type, nmtoken > nmtokens;
  typedef ::xsd::cxx::tree::ncname< char, name > ncname;
  typedef ::xsd::cxx::tree::language< char, token > language;

  // ID/IDREF.
  //
  typedef ::xsd::cxx::tree::id< char, ncname > id;
  typedef ::xsd::cxx::tree::idref< char, ncname, type > idref;
  typedef ::xsd::cxx::tree::idrefs< char, simple_type, idref > idrefs;

  // URI.
  //
  typedef ::xsd::cxx::tree::uri< char, simple_type > uri;

  // Qualified name.
  //
  typedef ::xsd::cxx::tree::qname< char, simple_type, uri, ncname > qname;

  // Binary.
  //
  typedef ::xsd::cxx::tree::buffer< char > buffer;
  typedef ::xsd::cxx::tree::base64_binary< char, simple_type > base64_binary;
  typedef ::xsd::cxx::tree::hex_binary< char, simple_type > hex_binary;

  // Date/time.
  //
  typedef ::xsd::cxx::tree::time_zone time_zone;
  typedef ::xsd::cxx::tree::date< char, simple_type > date;
  typedef ::xsd::cxx::tree::date_time< char, simple_type > date_time;
  typedef ::xsd::cxx::tree::duration< char, simple_type > duration;
  typedef ::xsd::cxx::tree::gday< char, simple_type > gday;
  typedef ::xsd::cxx::tree::gmonth< char, simple_type > gmonth;
  typedef ::xsd::cxx::tree::gmonth_day< char, simple_type > gmonth_day;
  typedef ::xsd::cxx::tree::gyear< char, simple_type > gyear;
  typedef ::xsd::cxx::tree::gyear_month< char, simple_type > gyear_month;
  typedef ::xsd::cxx::tree::time< char, simple_type > time;

  // Entity.
  //
  typedef ::xsd::cxx::tree::entity< char, ncname > entity;
  typedef ::xsd::cxx::tree::entities< char, simple_type, entity > entities;

  typedef ::xsd::cxx::tree::content_order content_order;
  // Flags and properties.
  //
  typedef ::xsd::cxx::tree::flags flags;
  typedef ::xsd::cxx::tree::properties< char > properties;

  // Parsing/serialization diagnostics.
  //
  typedef ::xsd::cxx::tree::severity severity;
  typedef ::xsd::cxx::tree::error< char > error;
  typedef ::xsd::cxx::tree::diagnostics< char > diagnostics;

  // Exceptions.
  //
  typedef ::xsd::cxx::tree::exception< char > exception;
  typedef ::xsd::cxx::tree::bounds< char > bounds;
  typedef ::xsd::cxx::tree::duplicate_id< char > duplicate_id;
  typedef ::xsd::cxx::tree::parsing< char > parsing;
  typedef ::xsd::cxx::tree::expected_element< char > expected_element;
  typedef ::xsd::cxx::tree::unexpected_element< char > unexpected_element;
  typedef ::xsd::cxx::tree::expected_attribute< char > expected_attribute;
  typedef ::xsd::cxx::tree::unexpected_enumerator< char > unexpected_enumerator;
  typedef ::xsd::cxx::tree::expected_text_content< char > expected_text_content;
  typedef ::xsd::cxx::tree::no_prefix_mapping< char > no_prefix_mapping;

  // Error handler callback interface.
  //
  typedef ::xsd::cxx::xml::error_handler< char > error_handler;

  // DOM interaction.
  //
  namespace dom
  {
    // Automatic pointer for DOMDocument.
    //
    using ::xsd::cxx::xml::dom::unique_ptr;

#ifndef XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
#define XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
    // DOM user data key for back pointers to tree nodes.
    //
    const XMLCh* const tree_node_key = ::xsd::cxx::tree::user_data_keys::node;
#endif
  }
}

// Forward declarations.
//
class outputParamsType;
class simulationParamsType;
class vector3DType;
class cuboidType;
class sphereType;
class parameters;

#include <memory>    // ::std::unique_ptr
#include <limits>    // std::numeric_limits
#include <algorithm> // std::binary_search
#include <utility>   // std::move

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/containers.hxx>
#include <xsd/cxx/tree/list.hxx>

#include <xsd/cxx/xml/dom/parsing-header.hxx>

class outputParamsType: public ::xml_schema::type
{
  public:
  // baseName
  //
  typedef ::xml_schema::string baseName_type;
  typedef ::xsd::cxx::tree::traits< baseName_type, char > baseName_traits;

  const baseName_type&
  baseName () const;

  baseName_type&
  baseName ();

  void
  baseName (const baseName_type& x);

  void
  baseName (::std::unique_ptr< baseName_type > p);

  // writeFrequency
  //
  typedef ::xml_schema::int_ writeFrequency_type;
  typedef ::xsd::cxx::tree::traits< writeFrequency_type, char > writeFrequency_traits;

  const writeFrequency_type&
  writeFrequency () const;

  writeFrequency_type&
  writeFrequency ();

  void
  writeFrequency (const writeFrequency_type& x);

  // Constructors.
  //
  outputParamsType (const baseName_type&,
                    const writeFrequency_type&);

  outputParamsType (const ::xercesc::DOMElement& e,
                    ::xml_schema::flags f = 0,
                    ::xml_schema::container* c = 0);

  outputParamsType (const outputParamsType& x,
                    ::xml_schema::flags f = 0,
                    ::xml_schema::container* c = 0);

  virtual outputParamsType*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  outputParamsType&
  operator= (const outputParamsType& x);

  virtual 
  ~outputParamsType ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< baseName_type > baseName_;
  ::xsd::cxx::tree::one< writeFrequency_type > writeFrequency_;
};

class simulationParamsType: public ::xml_schema::type
{
  public:
  // tEnd
  //
  typedef ::xml_schema::double_ tEnd_type;
  typedef ::xsd::cxx::tree::traits< tEnd_type, char, ::xsd::cxx::tree::schema_type::double_ > tEnd_traits;

  const tEnd_type&
  tEnd () const;

  tEnd_type&
  tEnd ();

  void
  tEnd (const tEnd_type& x);

  // deltaT
  //
  typedef ::xml_schema::double_ deltaT_type;
  typedef ::xsd::cxx::tree::traits< deltaT_type, char, ::xsd::cxx::tree::schema_type::double_ > deltaT_traits;

  const deltaT_type&
  deltaT () const;

  deltaT_type&
  deltaT ();

  void
  deltaT (const deltaT_type& x);

  // cutOfRadius
  //
  typedef ::xml_schema::double_ cutOfRadius_type;
  typedef ::xsd::cxx::tree::traits< cutOfRadius_type, char, ::xsd::cxx::tree::schema_type::double_ > cutOfRadius_traits;

  const cutOfRadius_type&
  cutOfRadius () const;

  cutOfRadius_type&
  cutOfRadius ();

  void
  cutOfRadius (const cutOfRadius_type& x);

  // boundaryConditions
  //
  typedef ::xml_schema::string boundaryConditions_type;
  typedef ::xsd::cxx::tree::traits< boundaryConditions_type, char > boundaryConditions_traits;

  const boundaryConditions_type&
  boundaryConditions () const;

  boundaryConditions_type&
  boundaryConditions ();

  void
  boundaryConditions (const boundaryConditions_type& x);

  void
  boundaryConditions (::std::unique_ptr< boundaryConditions_type > p);

  // domainDimensions
  //
  typedef ::vector3DType domainDimensions_type;
  typedef ::xsd::cxx::tree::traits< domainDimensions_type, char > domainDimensions_traits;

  const domainDimensions_type&
  domainDimensions () const;

  domainDimensions_type&
  domainDimensions ();

  void
  domainDimensions (const domainDimensions_type& x);

  void
  domainDimensions (::std::unique_ptr< domainDimensions_type > p);

  // Constructors.
  //
  simulationParamsType (const tEnd_type&,
                        const deltaT_type&,
                        const cutOfRadius_type&,
                        const boundaryConditions_type&,
                        const domainDimensions_type&);

  simulationParamsType (const tEnd_type&,
                        const deltaT_type&,
                        const cutOfRadius_type&,
                        const boundaryConditions_type&,
                        ::std::unique_ptr< domainDimensions_type >);

  simulationParamsType (const ::xercesc::DOMElement& e,
                        ::xml_schema::flags f = 0,
                        ::xml_schema::container* c = 0);

  simulationParamsType (const simulationParamsType& x,
                        ::xml_schema::flags f = 0,
                        ::xml_schema::container* c = 0);

  virtual simulationParamsType*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  simulationParamsType&
  operator= (const simulationParamsType& x);

  virtual 
  ~simulationParamsType ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< tEnd_type > tEnd_;
  ::xsd::cxx::tree::one< deltaT_type > deltaT_;
  ::xsd::cxx::tree::one< cutOfRadius_type > cutOfRadius_;
  ::xsd::cxx::tree::one< boundaryConditions_type > boundaryConditions_;
  ::xsd::cxx::tree::one< domainDimensions_type > domainDimensions_;
};

class vector3DType: public ::xml_schema::type
{
  public:
  // x
  //
  typedef ::xml_schema::double_ x_type;
  typedef ::xsd::cxx::tree::traits< x_type, char, ::xsd::cxx::tree::schema_type::double_ > x_traits;

  const x_type&
  x () const;

  x_type&
  x ();

  void
  x (const x_type& x);

  // y
  //
  typedef ::xml_schema::double_ y_type;
  typedef ::xsd::cxx::tree::traits< y_type, char, ::xsd::cxx::tree::schema_type::double_ > y_traits;

  const y_type&
  y () const;

  y_type&
  y ();

  void
  y (const y_type& x);

  // z
  //
  typedef ::xml_schema::double_ z_type;
  typedef ::xsd::cxx::tree::traits< z_type, char, ::xsd::cxx::tree::schema_type::double_ > z_traits;

  const z_type&
  z () const;

  z_type&
  z ();

  void
  z (const z_type& x);

  // Constructors.
  //
  vector3DType (const x_type&,
                const y_type&,
                const z_type&);

  vector3DType (const ::xercesc::DOMElement& e,
                ::xml_schema::flags f = 0,
                ::xml_schema::container* c = 0);

  vector3DType (const vector3DType& x,
                ::xml_schema::flags f = 0,
                ::xml_schema::container* c = 0);

  virtual vector3DType*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  vector3DType&
  operator= (const vector3DType& x);

  virtual 
  ~vector3DType ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< x_type > x_;
  ::xsd::cxx::tree::one< y_type > y_;
  ::xsd::cxx::tree::one< z_type > z_;
};

class cuboidType: public ::xml_schema::type
{
  public:
  // position
  //
  typedef ::vector3DType position_type;
  typedef ::xsd::cxx::tree::traits< position_type, char > position_traits;

  const position_type&
  position () const;

  position_type&
  position ();

  void
  position (const position_type& x);

  void
  position (::std::unique_ptr< position_type > p);

  // velocity
  //
  typedef ::vector3DType velocity_type;
  typedef ::xsd::cxx::tree::traits< velocity_type, char > velocity_traits;

  const velocity_type&
  velocity () const;

  velocity_type&
  velocity ();

  void
  velocity (const velocity_type& x);

  void
  velocity (::std::unique_ptr< velocity_type > p);

  // dimensions
  //
  typedef ::vector3DType dimensions_type;
  typedef ::xsd::cxx::tree::traits< dimensions_type, char > dimensions_traits;

  const dimensions_type&
  dimensions () const;

  dimensions_type&
  dimensions ();

  void
  dimensions (const dimensions_type& x);

  void
  dimensions (::std::unique_ptr< dimensions_type > p);

  // mass
  //
  typedef ::xml_schema::double_ mass_type;
  typedef ::xsd::cxx::tree::traits< mass_type, char, ::xsd::cxx::tree::schema_type::double_ > mass_traits;

  const mass_type&
  mass () const;

  mass_type&
  mass ();

  void
  mass (const mass_type& x);

  // meshWidth
  //
  typedef ::xml_schema::double_ meshWidth_type;
  typedef ::xsd::cxx::tree::traits< meshWidth_type, char, ::xsd::cxx::tree::schema_type::double_ > meshWidth_traits;

  const meshWidth_type&
  meshWidth () const;

  meshWidth_type&
  meshWidth ();

  void
  meshWidth (const meshWidth_type& x);

  // sigma
  //
  typedef ::xml_schema::double_ sigma_type;
  typedef ::xsd::cxx::tree::traits< sigma_type, char, ::xsd::cxx::tree::schema_type::double_ > sigma_traits;

  const sigma_type&
  sigma () const;

  sigma_type&
  sigma ();

  void
  sigma (const sigma_type& x);

  // epsilon
  //
  typedef ::xml_schema::double_ epsilon_type;
  typedef ::xsd::cxx::tree::traits< epsilon_type, char, ::xsd::cxx::tree::schema_type::double_ > epsilon_traits;

  const epsilon_type&
  epsilon () const;

  epsilon_type&
  epsilon ();

  void
  epsilon (const epsilon_type& x);

  // Constructors.
  //
  cuboidType (const position_type&,
              const velocity_type&,
              const dimensions_type&,
              const mass_type&,
              const meshWidth_type&,
              const sigma_type&,
              const epsilon_type&);

  cuboidType (::std::unique_ptr< position_type >,
              ::std::unique_ptr< velocity_type >,
              ::std::unique_ptr< dimensions_type >,
              const mass_type&,
              const meshWidth_type&,
              const sigma_type&,
              const epsilon_type&);

  cuboidType (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  cuboidType (const cuboidType& x,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  virtual cuboidType*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  cuboidType&
  operator= (const cuboidType& x);

  virtual 
  ~cuboidType ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< position_type > position_;
  ::xsd::cxx::tree::one< velocity_type > velocity_;
  ::xsd::cxx::tree::one< dimensions_type > dimensions_;
  ::xsd::cxx::tree::one< mass_type > mass_;
  ::xsd::cxx::tree::one< meshWidth_type > meshWidth_;
  ::xsd::cxx::tree::one< sigma_type > sigma_;
  ::xsd::cxx::tree::one< epsilon_type > epsilon_;
};

class sphereType: public ::xml_schema::type
{
  public:
  // center_position
  //
  typedef ::vector3DType center_position_type;
  typedef ::xsd::cxx::tree::traits< center_position_type, char > center_position_traits;

  const center_position_type&
  center_position () const;

  center_position_type&
  center_position ();

  void
  center_position (const center_position_type& x);

  void
  center_position (::std::unique_ptr< center_position_type > p);

  // velocity
  //
  typedef ::vector3DType velocity_type;
  typedef ::xsd::cxx::tree::traits< velocity_type, char > velocity_traits;

  const velocity_type&
  velocity () const;

  velocity_type&
  velocity ();

  void
  velocity (const velocity_type& x);

  void
  velocity (::std::unique_ptr< velocity_type > p);

  // mass
  //
  typedef ::xml_schema::double_ mass_type;
  typedef ::xsd::cxx::tree::traits< mass_type, char, ::xsd::cxx::tree::schema_type::double_ > mass_traits;

  const mass_type&
  mass () const;

  mass_type&
  mass ();

  void
  mass (const mass_type& x);

  // radius
  //
  typedef ::xml_schema::double_ radius_type;
  typedef ::xsd::cxx::tree::traits< radius_type, char, ::xsd::cxx::tree::schema_type::double_ > radius_traits;

  const radius_type&
  radius () const;

  radius_type&
  radius ();

  void
  radius (const radius_type& x);

  // meshWidth
  //
  typedef ::xml_schema::double_ meshWidth_type;
  typedef ::xsd::cxx::tree::traits< meshWidth_type, char, ::xsd::cxx::tree::schema_type::double_ > meshWidth_traits;

  const meshWidth_type&
  meshWidth () const;

  meshWidth_type&
  meshWidth ();

  void
  meshWidth (const meshWidth_type& x);

  // sigma
  //
  typedef ::xml_schema::double_ sigma_type;
  typedef ::xsd::cxx::tree::traits< sigma_type, char, ::xsd::cxx::tree::schema_type::double_ > sigma_traits;

  const sigma_type&
  sigma () const;

  sigma_type&
  sigma ();

  void
  sigma (const sigma_type& x);

  // epsilon
  //
  typedef ::xml_schema::double_ epsilon_type;
  typedef ::xsd::cxx::tree::traits< epsilon_type, char, ::xsd::cxx::tree::schema_type::double_ > epsilon_traits;

  const epsilon_type&
  epsilon () const;

  epsilon_type&
  epsilon ();

  void
  epsilon (const epsilon_type& x);

  // Constructors.
  //
  sphereType (const center_position_type&,
              const velocity_type&,
              const mass_type&,
              const radius_type&,
              const meshWidth_type&,
              const sigma_type&,
              const epsilon_type&);

  sphereType (::std::unique_ptr< center_position_type >,
              ::std::unique_ptr< velocity_type >,
              const mass_type&,
              const radius_type&,
              const meshWidth_type&,
              const sigma_type&,
              const epsilon_type&);

  sphereType (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  sphereType (const sphereType& x,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  virtual sphereType*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  sphereType&
  operator= (const sphereType& x);

  virtual 
  ~sphereType ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< center_position_type > center_position_;
  ::xsd::cxx::tree::one< velocity_type > velocity_;
  ::xsd::cxx::tree::one< mass_type > mass_;
  ::xsd::cxx::tree::one< radius_type > radius_;
  ::xsd::cxx::tree::one< meshWidth_type > meshWidth_;
  ::xsd::cxx::tree::one< sigma_type > sigma_;
  ::xsd::cxx::tree::one< epsilon_type > epsilon_;
};

class parameters: public ::xml_schema::type
{
  public:
  // outputParameters
  //
  typedef ::outputParamsType outputParameters_type;
  typedef ::xsd::cxx::tree::traits< outputParameters_type, char > outputParameters_traits;

  const outputParameters_type&
  outputParameters () const;

  outputParameters_type&
  outputParameters ();

  void
  outputParameters (const outputParameters_type& x);

  void
  outputParameters (::std::unique_ptr< outputParameters_type > p);

  // simulationParameters
  //
  typedef ::simulationParamsType simulationParameters_type;
  typedef ::xsd::cxx::tree::traits< simulationParameters_type, char > simulationParameters_traits;

  const simulationParameters_type&
  simulationParameters () const;

  simulationParameters_type&
  simulationParameters ();

  void
  simulationParameters (const simulationParameters_type& x);

  void
  simulationParameters (::std::unique_ptr< simulationParameters_type > p);

  // cuboids
  //
  typedef ::cuboidType cuboids_type;
  typedef ::xsd::cxx::tree::sequence< cuboids_type > cuboids_sequence;
  typedef cuboids_sequence::iterator cuboids_iterator;
  typedef cuboids_sequence::const_iterator cuboids_const_iterator;
  typedef ::xsd::cxx::tree::traits< cuboids_type, char > cuboids_traits;

  const cuboids_sequence&
  cuboids () const;

  cuboids_sequence&
  cuboids ();

  void
  cuboids (const cuboids_sequence& s);

  // spheres
  //
  typedef ::sphereType spheres_type;
  typedef ::xsd::cxx::tree::sequence< spheres_type > spheres_sequence;
  typedef spheres_sequence::iterator spheres_iterator;
  typedef spheres_sequence::const_iterator spheres_const_iterator;
  typedef ::xsd::cxx::tree::traits< spheres_type, char > spheres_traits;

  const spheres_sequence&
  spheres () const;

  spheres_sequence&
  spheres ();

  void
  spheres (const spheres_sequence& s);

  // Constructors.
  //
  parameters (const outputParameters_type&,
              const simulationParameters_type&);

  parameters (::std::unique_ptr< outputParameters_type >,
              ::std::unique_ptr< simulationParameters_type >);

  parameters (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  parameters (const parameters& x,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  virtual parameters*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  parameters&
  operator= (const parameters& x);

  virtual 
  ~parameters ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< outputParameters_type > outputParameters_;
  ::xsd::cxx::tree::one< simulationParameters_type > simulationParameters_;
  cuboids_sequence cuboids_;
  spheres_sequence spheres_;
};

#include <iosfwd>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

// Parse a URI or a local file.
//

::std::unique_ptr< ::parameters >
parameters_ (const ::std::string& uri,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::parameters >
parameters_ (const ::std::string& uri,
             ::xml_schema::error_handler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::parameters >
parameters_ (const ::std::string& uri,
             ::xercesc::DOMErrorHandler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse std::istream.
//

::std::unique_ptr< ::parameters >
parameters_ (::std::istream& is,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::parameters >
parameters_ (::std::istream& is,
             ::xml_schema::error_handler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::parameters >
parameters_ (::std::istream& is,
             ::xercesc::DOMErrorHandler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::parameters >
parameters_ (::std::istream& is,
             const ::std::string& id,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::parameters >
parameters_ (::std::istream& is,
             const ::std::string& id,
             ::xml_schema::error_handler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::parameters >
parameters_ (::std::istream& is,
             const ::std::string& id,
             ::xercesc::DOMErrorHandler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse xercesc::InputSource.
//

::std::unique_ptr< ::parameters >
parameters_ (::xercesc::InputSource& is,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::parameters >
parameters_ (::xercesc::InputSource& is,
             ::xml_schema::error_handler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::parameters >
parameters_ (::xercesc::InputSource& is,
             ::xercesc::DOMErrorHandler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse xercesc::DOMDocument.
//

::std::unique_ptr< ::parameters >
parameters_ (const ::xercesc::DOMDocument& d,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::parameters >
parameters_ (::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // PARAMETERS_HXX
